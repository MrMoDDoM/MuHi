///////////////////////////////////////////////////////////////
//                     MuHi -
//   Multifunctionals E(/Hi)yes traking
//
//   @author: Daniele Barattieri di San Pietro
//   @email : danielebarattieri[at]gmail.com
//
//  ----------------------------------------------------------
//  This project is hosted and supported by the µHack assosiation,
//  with love from Brescia, Italy ( www.muhack.org ) ;)
//  The aim of this project is to provide a software assistance for
//  tetraplegic, using a webcam to track the eyes moviments.
//  The first part, where the sw find-track pupils, take care of
//  "trigger areas", so when the user look outside the screen, the sw
//  sets to true the corrispondig area-trigger.
//
//  The API provided for futher develop are composed only by these
//  triggers and some functions for quality check.
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
// Version     :
// Copyright   : GNU/GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "MuHi.h"
#include "Writer.h"
#include "WebcamWorker.h"

int init(){
	cout<<"Apertura stream webcam..."<<endl;
	//webcamWorker
	if(initCamWorker())
		return -1;

	//Istanzia memoria per la matrice dell'HUD
	HUD = Mat::zeros(Y_RESOLUTION, X_RESOLUTION,  CV_8UC3);

	cout<<"Inizializzazione interfaccia..."<<endl;
	//Inizializza il Writer(alfabeto e memoria)
	if(initWriter())
		return -1;

	cout<<"Tutto ok! Si parte!"<<endl;
	//All went ok!
	return 0;
}

//We need to ensure that all memory&devices are proprely released
int exit(){
	cout<<"Chiusura in corso..."<<endl;
	fin = true;
	destroyCamWorker();
	return 0;
}

int exitWithError(std::string msg){
	cout<<msg<<endl;
	return exit();
}

int main( int argc, char** argv ){

	int step = 0;
	noError = true;
	fin = false;
	int thres = 50;

	int old_status = 0, countingSameStatus = 0;

	int blinkTresh = 10;

	int blinkStatus = 0;

	if(init())
		exitWithError("Error initializing the system! STOP!");

	//clock_t te;

	while(!fin){
		//For this probably is better use an enum...
		// 0 - both open
		// 1 - right blink
		// 2 - left blink
		// 3 - both blink

		blinkStatus = 0;

		if(!getFrame(&frame))
			exitWithError("Error getting frame! STOP!");

		blinkStatus = detectBlink(&frame, blinkTresh, debug, thres);

		if(blinkStatus == old_status){
			countingSameStatus++;
		} else {
			old_status = blinkStatus;
			countingSameStatus = 0;
		}

		//Passare al Writer lo stato del blinink (blinkRigth, blinkLeft) e farlo agire di conseguenza
		if(countingSameStatus >= 3){
			countingSameStatus = 0;
			checkBlink(blinkStatus);
		}

		if(step >= STEP_WAIT){
			step = 0;
			stepWriter();
		}

		//Refersh HUD from Writer
		drawHUD(&HUD);

		//Controllo tastiera
		int c = waitKey(10);
		if( (char)c == 27 ) { fin = true; }
		if( (char)c == 'u' ) { stepWriter(); }
		if( (char)c == 'b' ) { click(); step = 0;}
		if( (char)c == '+' ) { STEP_WAIT = STEP_WAIT + 2; }
		if( (char)c == '-' ) { STEP_WAIT = STEP_WAIT - 2; }
		if( (char)c == 'a' ) { blinkTresh--; }
		if( (char)c == 's' ) { blinkTresh++; }
		if( (char)c == 'c' ) { thres--; }
		if( (char)c == 'v' ) { thres++; }
		if( (char)c == 'd' ) {
			if(debug){
				debug = false;
				destroyWindow("Webcam");
			} else debug = true;

		}

		//SHOW HUD
		imshow(MAIN_WIN_TITLE, HUD);

		if(debug){
			stringstream out;
			out<<"Status: "<<blinkStatus<<" - Velocity: "<<STEP_WAIT<<" - Sensibility: "<<blinkTresh<<" - Soglia occhio: "<<thres;
			cout<<out.str()<<endl;
			//cout<< "[INFO] detectBlink ci ha messo: "<< std::fixed << std::setw( 11 ) << std::setprecision( 6 )<<( te / CLOCKS_PER_SEC ) / 1000<<" millisecondi"<<endl;
			putText(frame, out.str(), Point(10,15), FONT_HERSHEY_TRIPLEX, 0.5, Scalar::all(255), 1, 1, false);
			imshow("WebCam", frame);
		}

		step++;
	}

	exit();
	cout<<"Grazie per aver usato MuHi! Alla prossima!"<<endl;
	getchar();
	return 0;
}
