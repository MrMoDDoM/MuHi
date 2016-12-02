///////////////////////////////////////////////////////////////
//                     MuHi
//         Multifunctionals E(/Hi)yes
//
//   @author: Daniele Barattieri di San Pietro
//   @email : danielebarattieri[at]gmail.com
//   @copyright: 2016
//
//  ----------------------------------------------------------
//  This project is hosted and supported by the µHack assosiation,
//  with love from Brescia, Italy ( www.muhack.org ) ;)
//  The aim of this project is to provide a software assistance for
//  tetraplegic, using a webcam to detect the eyes' blinks.
//  ----------------------------------------------------------
//
//  This Software is distribuited under the GNU/GPL license
//  Fot more informations, please see the LICENSE and README
//  file attached to the source code
//
////////////////////////////////////////////////////////////////

//============================================================================
// Name        : MuHi.cpp
// Author      : MrMoDDoM
// Version     : 1.0
// Copyright   : GNU/GPL
// Description : MuHi in C++, Ansi-style
//============================================================================

/*
    This file is part of MuHi.
    Copyright (C) 2016  Daniele Barattieri di San Pietro

    MuHi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MuHi.  If not, see <http://www.gnu.org/licenses/> or
    <http://www.muhack.org/> for futher datails.

*/


#include "MuHi.h"
#include "WebcamWorker.h"
#include "OutputWorker.h"

int init(){

	cout<<"Setting variables..."<<endl;
	//xdo = xdo_new(":0.0");
	debug = true;
	noError = true;
	fin = false;

	cout<<"Opening webcam stream..."<<endl;
	//webcamWorker
	if(initCamWorker())
		return -1;

	cout<<"Setting up output stream..."<<endl;
	if(initOutputWorker())
		return -1;

	//Istanzia memoria per la matrice dell'HUD
//	HUD = Mat::zeros(Y_RESOLUTION, X_RESOLUTION,  CV_8UC3);

//	cout<<"Inizializzazione interfaccia..."<<endl;
	//Inizializza il Writer(alfabeto e memoria)
//	if(initWriter())
//		return -1;

	cout<<"All ok! Let's go!"<<endl;
	//All went ok!
	return 0;
}

//We need to ensure that all memory&devices are proprely released
int exit(){
	cout<<"Closing..."<<endl;
	fin = true;
	destroyCamWorker();
	return 0;
}

int exitWithError(std::string msg){
	cout<<msg<<endl;
	return exit();
}

//This function procede to open an external application
int open_program( FILE *f, std::string path){
    	f = popen (path.c_str(), "w");
	if (!f){
		perror ("Error on popen!!!");
		return 1;
	}
	else return 0;
}

int main( int argc, char** argv ){

	if (argc > 50) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        	std::cout << "Usage is \"MuHi [-v|-d|-h|-s] PROGRAM\"\n"<<endl; // Inform the user of how to use the program
        	exit(0);
    	} else { // if we got enough parameters...
        	char *program ;
        	//std::cout << argv[0];
        	for (int i = 1; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
                	                          * Note that we're starting on 1 because we don't need to know the 
                        	                  * path of the program, which is stored in argv[0] */
            		if (i + 1 > argc){ // Check that we haven't finished parsing already
                		if (argv[i] == "-p") {
                    			// We know the next argument *should* be the filename:
                    			program = argv[i + 1];
                		} else if (argv[i] == "-d") {
                    			debug = true;
                		} else if ( strcmp(argv[i], "-h") != 0) {
                    			cout<<"./MuHi - Multi-Functional (/Hi)eye"<<endl;
					cout<<"Usange: ./MuHi [-v|-d|-h|-s] PROGRAM"<<endl;
					cout<<endl;
					cout<<"\t -v: \tPrint this help and exit"<<endl;
					exit(0);
                		} else {
                    			std::cout << "Not enough or invalid arguments, please try again.\n";
                   			exit(0);
            			}
			}

           	 	std::cout << argv[i] << " ";
        	}
    	}

	int thres = 350;
	int step = 0;
	int old_status = 0, countingSameStatus = 0;
	int blinkTresh = 10;
	int blinkStatus = 0;

	if(init())
		exitWithError("Error initializing the system! STOP!");

	//debug = true; //Just for testing pourpose

	while(!fin){
		//For this probably is better use an enum...
		// 0 - both open
		// 1 - right blink
		// 2 - left blink
		// 3 - both blink
		// 4 - no face
		// 5 - critcal error! Shut everything down, in the name of the Great Spaghetti Monster!!!

		blinkStatus = 0;

		if(!getFrame(&frame))
			exitWithError("Error getting frame! STOP!");

		blinkStatus = detectBlink(&frame, blinkTresh, debug, thres);

		//Some anti bounce check
		if(blinkStatus == old_status){
			countingSameStatus++;
		} else {
			old_status = blinkStatus;
			countingSameStatus = 0;
		}

		//Passare al Writer lo stato del blinink (blinkRigth, blinkLeft) e farlo agire di conseguenza
		if(countingSameStatus >= 1){
			countingSameStatus = 0;
//			checkBlink(blinkStatus);
			//Here we will pass the status's value to the active program
			//fprintf (program, "%i\n", blinkStatus);
			sendKeyboardKey(blinkStatus);
		}

//		if(step >= STEP_WAIT){
//			step = 0;
//			stepWriter();
//		}

		//Refersh HUD from Writer
//		drawHUD(&HUD);

		//Keyboard controls
		int c = waitKey(10);
		if( (char)c == 27 ) { fin = true; }
//		if( (char)c == 'u' ) { stepWriter(); }
//		if( (char)c == 'b' ) { click(); step = 0;}
//		if( (char)c == '+' ) { STEP_WAIT = STEP_WAIT + 2; }
//		if( (char)c == '-' ) { STEP_WAIT = STEP_WAIT - 2; }
		if( (char)c == 'a' ) { blinkTresh--; }
		if( (char)c == 's' ) { blinkTresh++; }
		if( (char)c == 'c' ) { thres--; }
		if( (char)c == 'v' ) { thres++; }
/*		if( (char)c == 'd' ) {
			if(debug){
				debug = false;
				destroyWindow("Webcam");
			} else debug = true;

		}*/

		//If the debug is activated, write HUD with usefull informations
		if(debug){
			system("clear");
			stringstream out;
			out<<"Status: "<<blinkStatus<<" - Sensibility: "<<blinkTresh<<" - Eyes threshold: "<<thres;
			cout<<out.str()<<endl;
			putText(frame, out.str(), Point(10,15), FONT_HERSHEY_TRIPLEX, 0.5, Scalar::all(255), 1, 1, false);
			imshow("WebCam", frame);
		}

		//step++;
	}

	exit();
	cout<<"Thankyou for using MuHi! See you next time!"<<endl;
	return 0;
}
