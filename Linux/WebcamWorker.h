/*
 * WebcamWorker.h
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#ifndef WEBCAMWORKER_H_
#define WEBCAMWORKER_H_

#include <string.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <highgui.h>

using namespace cv;

int initCamWorker();
void getFrame(Mat *in);
Rect detectFace(Mat *in);
int detectBlink(Mat *in, int _blinkThresh, bool _debug);
void checkStability(Rect *newer, Rect *old);
float searchFront(Mat *in, int *arr);
int findTheStatus(float shifLeft, float shiftRight);
Rect detectCascade(Mat *in, CascadeClassifier *cc, Size _size);

//---------UNUSED FUNCTION BUT MAYBE USEFULL ONE DAY------------
int movedDiff(Mat *backgroundImage, Mat *currentImage);

#endif /* WEBCAMWORKER_H_ */
