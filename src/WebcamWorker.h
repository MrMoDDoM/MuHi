/*
 * WebcamWorker.h
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#ifndef WEBCAMWORKER_H_
#define WEBCAMWORKER_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int initCamWorker();
int destroyCamWorker();
bool getFrame(Mat *in);
Rect detectFace(Mat *in);
int detectBlink(Mat *in, int _blinkThresh, bool _debug, int _thresh);
void checkStability(Rect *newer, Rect *old);
float searchFront(Mat *in, int *arr);
int findTheStatus(float shifLeft, float shiftRight);
Rect detectCascade(Mat *in, CascadeClassifier *cc, Size _size);

//---------UNUSED FUNCTION BUT MAYBE USEFULL ONE DAY------------
int movedDiff(Mat *backgroundImage, Mat *currentImage);

#endif /* WEBCAMWORKER_H_ */
