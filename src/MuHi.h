/*
 * MiHi.h
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#ifndef MUHI_H_
#define MUHI_H_

#include <iostream>
#include <stdio.h>
#include <iostream>
#include <exception>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//GLOBAL
static const char *MAIN_WIN_TITLE = "MuHi";
int X_RESOLUTION = 640;
int Y_RESOLUTION = 480;

////////////////////////////////////
//  VARIABLES
////////////////////////////////////
Mat frame, HUD;
bool blinkRigth, blinkLeft, noError, fin;
char key;
int STEP_WAIT = 30;
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
