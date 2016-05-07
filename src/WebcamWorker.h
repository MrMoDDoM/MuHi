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
int detectBlink(Mat *in);
void checkStability(Rect *newer, Rect *old);
int movedDiff(Mat *backgroundImage, Mat *currentImage);
bool searchHis(Mat *in, int *arr);

//Prove
void calcHi(Mat *in);
Rect detectEye(Mat *in, CascadeClassifier *cc);

#endif /* WEBCAMWORKER_H_ */
