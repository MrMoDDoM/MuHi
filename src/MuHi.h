/*
 * MiHi.h
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#ifndef MUHI_H_
#define MUHI_H_

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <exception>
#include <opencv/cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

//GLOBAL
char *MAIN_WIN_TITLE = "MuHi";
int X_RESOLUTION = 640;
int Y_RESOLUTION = 480;

////////////////////////////////////
//  VARIABLES
////////////////////////////////////
Mat frame, HUD;
bool blinkRigth, blinkLeft, noError, fin;
char key;
int STEP_WAIT = 50;
bool debug = false;
////////////////////////////////////
////////////////////////////////////
//  FUNCTIONS
////////////////////////////////////
void *stepCall(void *threadid);
int init();
int exit();
////////////////////////////////////


#endif /* MUHI_H_ */
