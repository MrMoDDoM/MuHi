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

void *stepCall(void *threadid){
	while(!fin){
		//cout<<system_clock<<endl;
		//waitKey(1000);
		stepWriter();
	}
}

int init(){
	//webcamWorker
	initCamWorker();

	//Istanzia memoria per la matrice dell'HUD
	HUD = Mat::zeros(Y_RESOLUTION, X_RESOLUTION,  CV_8UC3);

	//Inizializza il Writer(alfabeto e memoria)
	initWriter();

	//All went ok!
	return 0;
}

//We need to ensure that all memory&devices are proprely released
int exit(){
	fin = true;
	//cam.release();

	//Waitin for all sub-process to finish
	//BlinkDetector.join();

}

int main( int argc, char** argv ){

	init();
	int step = 0;
	noError = true;
	fin = false;

	while(!fin){

		//For this probably is better use an enum...
		// 0 - both open
		// 1 - right blink
		// 2 - left blink
		// 3 - both blink
		int blinkStatus = 0;

		getFrame(&frame);
		blinkStatus = detectBlink(&frame);

		switch (blinkStatus){
		case 0:
			break;
		case 1:
			cout<<"DESTRO"<<endl;
			break;
		case 2:
			cout<<"SINISTRO"<<endl;
			break;
		case 3:
			cout<<"ENTRAMBI"<<endl;
			break;
		}

		if(step >= STEP_WAIT){
			step = 0;
			stepWriter();
		}

		//Refersh HUD from Writer
		drawHUD(&HUD);

		//Passare al Writer lo stato del blinink (blinkRigth, blinkLeft) e farlo agire di conseguenza
		if(blinkStatus)
			checkBlink(blinkStatus);

		//Controllo tastiera
		int c = waitKey(10);
		if( (char)c == 27 ) { fin = true; }
		//if( (char)c == 'a' ) { stepWriter(void); }
		if( (char)c == 'b' ) { click(); step = 0;}
		if( (char)c == '+' ) { STEP_WAIT = STEP_WAIT + 10; }
		if( (char)c == '-' ) { STEP_WAIT = STEP_WAIT - 10; }
		if( (char)c == 'd' ) { debug = !debug; }


		//SHOW HUD
		imshow(MAIN_WIN_TITLE, HUD);

		if(debug){
			imshow("WebCam", frame);
		}

		step++;
	}

	exit();
	return 0;
}
