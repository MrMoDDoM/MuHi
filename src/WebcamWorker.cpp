/*
 * WebcamWorker.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#include "WebcamWorker.h"

VideoCapture cam;
String face_cascade_name = "/usr/local/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml";
String eyes_left_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_mcs_lefteye.xml";
String eyes_right_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_mcs_righteye.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade_left;
CascadeClassifier eyes_cascade_right;

Rect face;

Mat leftBackground;
Mat rightBackground;

const int kEyePercentTop = 31;
const int kEyePercentSide = 15;
const int kEyePercentHeight = 23;
const int kEyePercentWidth = 30;

const float STABILITY_THRESHOLD = 5.0;

bool faceMoved = true;

int leftCount = 0, rightCount = 0, leftJustBlink = 0, rightJustBlink = 0;

bool leftIsClosed, rightIsClosed;

int initCamWorker() {

	//Qui dovrò preoccuparmi di gestire le webcam...
	cam.open(0);

	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	if( !eyes_cascade_left.load( eyes_left_cascade_name ) ){ printf("--(!)Error loading LEFT EYE\n"); return -1; };
	if( !eyes_cascade_right.load( eyes_right_cascade_name ) ){ printf("--(!)Error loading RIGHT EYE\n"); return -1; };

	return 0;
}

void getFrame(Mat *in){

	if(!cam.isOpened()){
		printf("No open webcam\n");
	}

	cam.read(*in);

	if(in->empty()){
		printf("No frame!\n");
	}

	flip( *in, *in, 1);
}

Rect detectFace( Mat *in ){
    std::vector<Rect> faces;
    cv::Rect out(0,0,0,0);

    //-- Detect face
    face_cascade.detectMultiScale( *in, faces, 1.1, 2, 0, Size(80, 80) );

    //We consider only ONE face, the biggest one...
    if(faces.size() != 0){
        for(int i=0; i< faces.size(); i++){
            if(faces[i].area() >= out.area()){
                out = faces[i];
            }
        }

        return out;
    }
    else
        return out;
}

void checkStability(Rect *newer, Rect *old){
	//Upper left corner
	float diff = sqrt( pow( ( newer->x - old->x) ,2) + pow( (newer->y - old->y ) ,2) ) ;
	if(diff > STABILITY_THRESHOLD){
		//If the diff is higher than the treshold, we update 'face' to move toward the new result
		old->x = (newer->x + old->x) / 2;
		old->y = (newer->y + old->y) / 2;
		//faceMoved = true;
	} else { //We must know when the face has moved
		//faceMoved = false;
	}

	//Bottom right corner (using h&w)
	diff = sqrt( pow( ( (newer->x + newer->width) - (old->x + old->width)) ,2) + pow( ( (newer->y + newer->height) - (old->y + old->height) ) ,2) ) ;
	if(diff > STABILITY_THRESHOLD){
		old->width = ( old->width + newer->width) / 2;
		old->height = ( old->height + newer->height) / 2;
		//faceMoved = true;
	} else { //Same as above
		//faceMoved = false;
	}
}

Mat diffImg(Mat *t0, Mat *t1, Mat *t2){
	Mat d1;
	Mat d2;
	Mat out;
	absdiff(*t2, *t1, d1);
	absdiff(*t1, *t0, d2);
	bitwise_and(d1, d2, out);
	return out;
}

int movedDiff(Mat *backgroundImage, Mat *currentImage){

	if(backgroundImage->empty()){
		backgroundImage = currentImage;
		return -1;
	}

	Mat diffImage;

	absdiff(*backgroundImage, *currentImage, diffImage);
	cv::imshow("diff", diffImage);
	//cv::imshow("back", *backgroundImage);


	/*
	Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);

	float threshold = 30.0f;
	float dist;*/

	int sum = 0;

	for(int j=0; j<diffImage.rows; ++j)
		for(int i=0; i<diffImage.cols; ++i){
			Vec3b pix = diffImage.at<cv::Vec3b>(j,i);
			if(pix[0] >= 125){
				sum++;
			}
		}


	if(sum >= ((diffImage.rows + diffImage.cols) / 100 ) * 23){
		std::cout<<"BLINK!"<<std::endl;
	}

	//std::cout<<"Sum: "<<sum<<std::endl;

	//cv::imshow("foregroundMas", foregroundMask);


	return 0;
}

void calcHi(Mat *in){

}

Rect detectEye(Mat *in, CascadeClassifier *cc){

	std::vector<Rect> results;
	cv::Rect out(0,0,0,0);

	//eyes_cascade_right.detectMultiScale( faceROI(leftEyeRegion), eyes, 1.1, 2, 0 , Size(20, 20), Size(50,50) );

	cc->detectMultiScale(*in,results,1.1,2,0,Size(20,20), Size(50,50));

	//We consider only ONE face, the biggest one...
	if(results.size() != 0){
		for(int i=0; i < results.size(); i++){
			if(results[i].area() >= out.area()){
				out = results[i];
			}
		}
		return out;
	}
	else
		return out;

}
//Return true when the eye is closed
bool searchHis(Mat *in){

	//Mat his = Mat::zeros(in->rows, in->cols, CV_8UC1);

	int index_left = -1, index_right = -1, q;

	int array[in->cols];

	for(q = 0; q < in->cols; q++)
		array[q] = 0;

	Scalar pixel;

	int accumulator = 0, max = 0, media = 0, i, j;
	for(i = 0; i< in->cols; i++){
		accumulator = 0;
		for(j = 0; j< in->rows; j++){
			pixel = in->at<uchar>(Point(i,j)); //Attenzione all'ordering!
			if(!pixel.val[0]){
				accumulator++;
			}
		}
		array[i] = accumulator;

		media += accumulator;

		if( accumulator <= max)
			max = accumulator;
	}

	media = media / i;

	//std::cout<<"Media: "<<media<<std::endl;

	//Mat his = Mat::zeros(max, in->cols,  CV_8UC3);

	//in->copyTo(his);

	/*for(int a = 0; a < in->cols; a++){
		std::cout<< vettore[i]<< "-";
	}

	for(i = 0; i< his.cols; i++)
	{
		for(j = 0; j< his.rows; j++)
		{
			if(vettore[i] >= 0)
			{
				his.at<uchar>(j,i) = 255;
				vettore[i]--;
			}
		}
	}*/

	//Varianza
	int varianza = 0;

	for(q = 0; q < in->cols; q++)
			varianza += (array[q] - media) * (array[q] - media);

	varianza = varianza / i;

	//std::cout<<"Media: "<< media << " -Varianza: "<< varianza << std::endl;

	//From now on, we have the vector 'array' that contains the data info about the 'histogram'
//	index_left = in->cols/3;
//	index_right = 2 * index_left;
//
//	int sum_ext = 0, sum_int = 0;
//
//	for(q = index_left; q < index_right; q++){
//		sum_int += array[q];
//	}
//
//	for(q = 5; q < index_left; q ++){
//		sum_ext += array[q];
//	}
//
//	for(q = index_right; q < in->cols-5; q ++){
//		sum_ext += array[q];
//	}

//	std::cout<<"Ext: "<< sum_ext << " - Int: " << sum_int<< std::endl;
//	std::cout<<"Index_lef: "<< index_left << " - Index_right: "<< index_right << std::endl;

	//False if OPEN
	//True if CLOSED
//	if(sum_int > sum_ext){
//		return false;
//	} else return true;


	if(varianza >= 15){
		return false;
	} else return true;

}

int detectBlink(Mat *in){

	//calcHi(in);

	Mat frame_gray;
	Mat faceROI;
	Mat leftEyeROI;
	Mat rightEyeROI;

	bool rightBlink = false;
	bool leftBlink = false;

	cvtColor( *in, frame_gray, CV_BGR2GRAY );
	GaussianBlur( *in, *in, Size(3, 3), 1, 1 );
	//equalizeHist( frame_gray, frame_gray );

	//May be here we can use pointer for passing data from the detectFace function..
	Rect candidate = detectFace(&frame_gray); //Detect a face
	if(candidate.area() == 0) //No face found for this turn?
		return 0;

	if(face.area() == 0) //We found a face in this frame, but we don't have a candidate?
		face = candidate;

	//Now we must check the stability from the previous face
	checkStability(&candidate, &face); //This function update the Rect containing the face that we will work on

	//----From now on, we use 'face' as the working data, trustfull that contain the best face possible-------

	rectangle(*in, Point(face.x, face.y), Point(face.x + face.width, face.y + face.height), Scalar::all(255), 2, 1, 0);

	int eye_region_width = face.width * (kEyePercentWidth/100.0);
	int eye_region_height = face.width * (kEyePercentHeight/100.0);
	int eye_region_top = face.height * (kEyePercentTop/100.0);

	//Now we have a Mat within the face
	//faceROI = frame_gray( face );

	//Extrapolate the most probabilisitc area where the eyes should be
	Rect leftEyeRegion = Rect(face.width*(kEyePercentSide/100.0), eye_region_top,eye_region_width,eye_region_height);
	Rect rightEyeRegion = Rect(face.width - eye_region_width - face.width*(kEyePercentSide/100.0), eye_region_top,eye_region_width,eye_region_height);

	//Transpose the coordinates to the face starting position
	leftEyeRegion.x += face.x;
	leftEyeRegion.y += face.y;
	rightEyeRegion.x += face.x;
	rightEyeRegion.y += face.y;

	Mat tempROI = frame_gray(leftEyeRegion);
	Rect temp = detectEye(&tempROI, &eyes_cascade_left);
	if(temp.area() != 0 ){

		temp.x += leftEyeRegion.x;
		temp.y += leftEyeRegion.y;

//		temp.width -= temp.width/5;
//		temp.height -= temp.height/5;

		leftEyeROI = frame_gray(temp);
		imshow("Prova sinistra", leftEyeROI);

		rectangle(*in,
				Point(temp.x, temp.y),
				Point(temp.x + temp.width, temp.y + temp.height),
				Scalar::all(255), 2, 1, 0);
	}

	tempROI = frame_gray(rightEyeRegion);
	temp = detectEye(&tempROI, &eyes_cascade_right);
	if(temp.area() != 0 ){
		temp.x += rightEyeRegion.x;
		temp.y += rightEyeRegion.y;
		rightEyeROI = frame_gray(temp);
		imshow("Prova destra", rightEyeROI);

		rectangle(*in,
				Point(temp.x, temp.y),
				Point(temp.x + temp.width, temp.y + temp.height),
				Scalar::all(255), 2, 1, 0);
	}

	rectangle(*in,
			Point(leftEyeRegion.x, leftEyeRegion.y),
			Point(leftEyeRegion.x + leftEyeRegion.width, leftEyeRegion.y + leftEyeRegion.height),
			Scalar::all(255), 2, 1, 0);
	rectangle(*in,
			Point(rightEyeRegion.x, rightEyeRegion.y),
			Point(rightEyeRegion.x + rightEyeRegion.width, rightEyeRegion.y + rightEyeRegion.height),
			Scalar::all(255), 2, 1, 0);


	//---AND FINNALLY WE HAVE THE DAMN MAT EYES ROI !!! ----
	/* 0: Binary
	 1: Binary Inverted
	 2: Threshold Truncated
	 3: Threshold to Zero
	 4: Threshold to Zero Inverted
    */

	if(!leftEyeROI.empty()){
		Mat temp;
		threshold( leftEyeROI, leftEyeROI, 50, 255, 0 );
		resize(leftEyeROI, temp, Size(), 2, 2, INTER_NEAREST);
		GaussianBlur( temp, temp, Size(3, 3), 1, 1 );
		threshold( temp, temp, 70, 255, 0 );
		leftBlink = searchHis(&temp);
		imshow("Threshold L", temp);
	}

	if(!rightEyeROI.empty()){
		Mat temp;
		threshold( rightEyeROI, rightEyeROI, 50, 255, 0 );
		resize(rightEyeROI, temp, Size(), 2, 2, INTER_NEAREST);
		GaussianBlur( temp, temp, Size(3, 3), 1, 1 );
		threshold( temp, temp, 70, 255, 0 );
		rightBlink = searchHis(&temp);
		imshow("Threshold R", temp);
	}

	int blinkThresh = 5;

	if(leftBlink && rightBlink){
		return 0; //For now, both blink are too bad
	} else if (leftBlink ){// && !rightBlink){
		leftJustBlink --;
		leftCount++;
		if(leftCount >= blinkThresh && leftJustBlink <= 0){
			leftCount = 0;
			leftJustBlink = blinkThresh;
			return 2;
		}
	} else if (rightBlink ){// && rightBlink){
		rightJustBlink --;
		rightCount++;
		if(rightCount >= blinkThresh && rightJustBlink <= 0){
			rightCount = 0;
			rightJustBlink = blinkThresh;
			return 1;
		}
	} else {
		leftCount = 0;
		rightCount = 0;
		return 0;
	}

}


