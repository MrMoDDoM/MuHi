/*
 * WebcamWorker.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#include "WebcamWorker.h"

VideoCapture cam;
String face_cascade_name = "lbpcascade_frontalface.xml";
String eyes_left_cascade_name = "haarcascade_mcs_lefteye.xml";
String eyes_right_cascade_name = "haarcascade_mcs_righteye.xml";
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

bool leftIsClosed, rightIsClosed;

int blinkThresh = 7;

const int eyeArrayDim = 6;
int leftVarianzaArray[eyeArrayDim], rightVarianzaArray[eyeArrayDim];
int leftShiftArray[eyeArrayDim], rightShiftArray[eyeArrayDim];

int initCamWorker() {

	//Qui dovrò preoccuparmi di gestire le webcam...
	cam.open(0);

	if(!cam.isOpened()){
		printf("No open webcam\n");
		return -1;
	}

	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading FACE CLASSIFIER\n"); return -1; };
	if( !eyes_cascade_left.load( eyes_left_cascade_name ) ){ printf("--(!)Error loading LEFT EYE CLASSIFIER\n"); return -1; };
	if( !eyes_cascade_right.load( eyes_right_cascade_name ) ){ printf("--(!)Error loading RIGHT EYE CLASSIFIER\n"); return -1; };

	for(int i = 0; i < eyeArrayDim; i++){
		leftVarianzaArray[i] = 0;
		rightVarianzaArray[i] = 0;
		leftShiftArray[i] = 0;
		rightShiftArray[i] = 0;
	}

	return 0;
}

int destroyCamWorker(){
	if(!cam.isOpened()){
		return -1;
	}

	cam.release();
	return 0;
}

bool getFrame(Mat *in){

	int trys = 0;

	if(!cam.isOpened()){
		printf("No open webcam\n");
		return false;
	}
	//Trys 10 times to get the frame
	while(in->empty() && trys<=10){
		trys++;
		cam.read(*in);
	}

	if(in->empty()){
		printf("No frame!\n");
		return false;
	}

	flip( *in, *in, 1);

	return true;
}

Rect detectCascade(Mat *in, CascadeClassifier *cc, Size _size){

	std::vector<Rect> results;
	cv::Rect out(0,0,0,0);

	//eyes_cascade_right.detectMultiScale( faceROI(leftEyeRegion), eyes, 1.1, 2, 0 , Size(20, 20), Size(50,50) );

	cc->detectMultiScale(*in, results, 1.1, 2, 0, _size);

	//We consider only ONE face, the biggest one...
	if(results.size() != 0){
		for(unsigned int i=0; i < results.size(); i++){
			if(results[i].area() >= out.area()){
				out = results[i];
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
//Return true when the eye is closed
float searchFront(Mat *in, int *arrVarianza, int *arrShift){
	//Mat his = Mat::zeros(in->rows, in->cols, CV_8UC1);

	//int index_left = -1, index_right = -1,
	int q;

	int *array = (int *)malloc( (in->cols) * sizeof (int));

	//int array[in->cols];

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

		if( accumulator >= max)
			max = accumulator;
	}

	media = media / i;

	//std::cout<<"Media: "<<media<<std::endl;

	//Varianza
	int varianza = 0;

	for(q = 0; q < in->cols; q++)
			varianza += (array[q] - media) * (array[q] - media);

	varianza = varianza / i;

	//Push bach varianza value in the array
	for(q = 0; q < eyeArrayDim - 1; q++){
		arrVarianza[q] = arrVarianza[q+1];
		arrShift[q] = arrShift[q+1];
	}

	arrVarianza[eyeArrayDim - 1] = varianza;

	//Calc the delta shifting
	float shift = (arrVarianza[eyeArrayDim - 1] - arrVarianza[0] ) / eyeArrayDim;

	//std::cout << std::fixed << std::setprecision(3) <<"SHIFT: "<< shift <<" -Media: "<< media << " -Varianza: "<< varianza << " -Max: "<< max <<std::endl;

	arrShift[eyeArrayDim - 1] = shift;

	free(array);

	return shift;

	//From now on, we have the vector 'array' that contains the data info about the 'histogram'

//	std::cout<<"Ext: "<< sum_ext << " - Int: " << sum_int<< std::endl;
//	std::cout<<"Index_lef: "<< index_left << " - Index_right: "<< index_right << std::endl;
}

int findTheStatus(){

	bool leftIsClosing = false, leftIsOpening = false, rightIsClosing = false, rightIsOpening = false;

	int leftCountClosing = 0, rightCountClosing = 0, leftCountOpening = 0, rightCountOpening = 0;

	for(int i = 0; i < eyeArrayDim; i++){

		//Couting left
		if(leftShiftArray[i] < -1)
			leftCountClosing += abs(leftShiftArray[i]);
		if(leftShiftArray[i] > 1)
			leftCountOpening += abs(rightShiftArray[i]);

		//Countig right
		if(rightShiftArray[i] < -1)
			rightCountClosing += abs(rightShiftArray[i]);
		if(rightShiftArray[i] > 1)
			rightCountOpening += abs(rightShiftArray[i]);
	}

//	std::cout<< std::fixed << std::setprecision(5)<<"LCC: "<<leftCountClosing;
//	std::cout<< std::fixed << std::setprecision(5)<<" -LCO: "<<leftCountOpening<<std::endl;
//
//	std::cout<< std::fixed << std::setprecision(5)<<"RCC: "<<rightCountClosing;
//	std::cout<< std::fixed << std::setprecision(5)<<" -RCO: "<<rightCountOpening<<std::endl;

	if(leftCountClosing > leftCountOpening + blinkThresh)
		leftIsClosing = true;
	else if (leftCountOpening < leftCountClosing + blinkThresh)
		leftIsOpening = true;


	if(rightCountClosing > rightCountOpening + blinkThresh )
		rightIsClosing = true;
	else if (rightCountOpening < rightCountClosing + blinkThresh )
		rightIsOpening = true;


	if(rightIsClosing && !rightIsClosed && !rightIsOpening)
		rightIsClosed = true;

	if(rightIsOpening && rightIsClosed && !rightIsClosing)
		rightIsClosed = false;

	if(leftIsClosing && !leftIsClosed && !leftIsOpening)
		leftIsClosed = true;

	if(leftIsOpening && leftIsClosed && !leftIsClosing)
		leftIsClosed = false;


	if((rightIsClosed && leftIsClosed) || (leftIsClosing && rightIsClosing))
		return 3;
	else if (!rightIsClosed && leftIsClosed)
		return 2;
	else if (rightIsClosed && !leftIsClosed)
		return 1;
	else return 0;
}

Mat frame_gray;

Rect left_cand(0,0,0,0), right_cand(0,0,0,0);

int detectBlink(Mat *in, int _blinkThresh, bool _debug, int _thresh){

	//calcHi(in);

	blinkThresh = _blinkThresh;

	Mat faceROI;
	Mat leftEyeROI;
	Mat rightEyeROI;

	//float rightShift = 0.0, leftShift = 0.0;

	cvtColor( *in, frame_gray, CV_BGR2GRAY );
	GaussianBlur( *in, *in, Size(3, 3), 1, 1 );
	//equalizeHist( frame_gray, frame_gray );

	//May be here we can use pointer for passing data from the detectFace function..
	Rect candidate = detectCascade(&frame_gray, &face_cascade, Size(80,80));//detectFace(&frame_gray); //Detect a face
	if(candidate.area() == 0) //No face found for this turn?
		return 0;

	if(face.area() == 0) //We found a face in this frame, but we don't have a candidate?
		face = candidate;

	//Now we must check the stability from the previous face
	checkStability(&candidate, &face); //This function update the Rect containing the face that we will work on

	//----From now on, we use 'face' as the working data, trustfully that contains the best face possible-------
	if(_debug)
		rectangle(*in, Point(face.x, face.y), Point(face.x + face.width, face.y + face.height), Scalar::all(255), 2, 1, 0);


	int eye_region_width = face.width * (kEyePercentWidth/100.0);
	int eye_region_height = face.width * (kEyePercentHeight/100.0);
	int eye_region_top = face.height * (kEyePercentTop/100.0);

	//Now we have a Mat within the face
	//faceROI = frame_gray( face );

	//Extrapolate the most probabilistic area where the eyes should be
	Rect leftEyeRegion = Rect(face.width*(kEyePercentSide/100.0), eye_region_top,eye_region_width,eye_region_height);
	Rect rightEyeRegion = Rect(face.width - eye_region_width - face.width*(kEyePercentSide/100.0), eye_region_top,eye_region_width,eye_region_height);

	//Transpose the coordinates to the face starting position
	leftEyeRegion.x += face.x;
	leftEyeRegion.y += face.y;
	rightEyeRegion.x += face.x;
	rightEyeRegion.y += face.y;

	Mat tempROI = frame_gray(leftEyeRegion);
	Rect temp = detectCascade(&tempROI, &eyes_cascade_left, Size(20,20));
	if(temp.area() != 0 ){

		//Translate to the right coordinates
		temp.x += leftEyeRegion.x;
		temp.y += leftEyeRegion.y;

		//Crop for 1/5
		temp.x += temp.width/5;
		temp.y += temp.width/5;
		temp.width -= temp.width/5;
		temp.height -= temp.height/5;

		if(left_cand.area() == 0) //We found the eye but we don't have a candidate?
				left_cand = temp;

			checkStability(&temp, &left_cand);

		leftEyeROI = frame_gray(temp);
		//imshow("Prova sinistra", leftEyeROI);

		if(_debug)
			rectangle(*in,
					Point(left_cand.x, left_cand.y),
					Point(left_cand.x + left_cand.width, left_cand.y + left_cand.height),
					Scalar::all(255), 2, 1, 0);

//			rectangle(*in,
//					Point(temp.x, temp.y),
//					Point(temp.x + temp.width, temp.y + temp.height),
//					Scalar::all(255), 2, 1, 0);
	} else {
		if(left_cand.area() != 0)
			leftEyeROI = frame_gray(left_cand);
	}

	tempROI = frame_gray(rightEyeRegion);
	temp = detectCascade(&tempROI, &eyes_cascade_right, Size(20,20));
	if(temp.area() != 0 ){

		temp.x += rightEyeRegion.x;
		temp.y += rightEyeRegion.y;

		temp.x += temp.width/5;
		temp.y += temp.width/5;
		temp.width -= temp.width/5;
		temp.height -= temp.height/5;

		if(right_cand.area() == 0) //We found the eye but we don't have a candidate?
			right_cand = temp;

		checkStability(&temp, &right_cand);


		rightEyeROI = frame_gray(temp);
		//imshow("Prova destra", rightEyeROI);

		if(_debug)
			rectangle(*in,
					Point(right_cand.x, right_cand.y),
					Point(right_cand.x + right_cand.width, right_cand.y + right_cand.height),
					Scalar::all(255), 2, 1, 0);
	} else {
		if(right_cand.area() != 0)
			rightEyeROI = frame_gray(right_cand);
	}

	if(_debug){
		rectangle(*in,
				Point(leftEyeRegion.x, leftEyeRegion.y),
				Point(leftEyeRegion.x + leftEyeRegion.width, leftEyeRegion.y + leftEyeRegion.height),
				Scalar::all(255), 2, 1, 0);
		rectangle(*in,
				Point(rightEyeRegion.x, rightEyeRegion.y),
				Point(rightEyeRegion.x + rightEyeRegion.width, rightEyeRegion.y + rightEyeRegion.height),
				Scalar::all(255), 2, 1, 0);
	}

	//---AND FINNALLY WE HAVE THE DAMN MAT EYES ROI !!! ----
	/* 0: Binary
	 1: Binary Inverted
	 2: Threshold Truncated
	 3: Threshold to Zero
	 4: Threshold to Zero Inverted
    */

	if(!leftEyeROI.empty()){
		Mat temp;
		threshold( leftEyeROI, leftEyeROI, _thresh, 255, 0 );
		resize(leftEyeROI, temp, Size(), 2, 2, INTER_NEAREST);
		GaussianBlur( temp, temp, Size(3, 3), 1, 1);
		threshold( temp, temp, 70, 255, 0 );
		//leftShift =
		searchFront(&temp, leftVarianzaArray, leftShiftArray);
		if(_debug)
			imshow("Threshold L", temp);
	}

	if(!rightEyeROI.empty()){
		Mat temp;
		threshold( rightEyeROI, rightEyeROI, _thresh, 255, 0 );
		resize(rightEyeROI, temp, Size(), 2, 2, INTER_NEAREST);
		GaussianBlur( temp, temp, Size(3, 3), 1, 1 );
		threshold( temp, temp, 70, 255, 0 );
		//rightShift =
		searchFront(&temp, rightVarianzaArray, rightShiftArray);
		if(_debug)
			imshow("Threshold R", temp);
	}

	//std::cout<<"Sinistro: "<<leftBlink<<" -Destro: "<<rightBlink<<std::endl;

	return findTheStatus();
}

//---------UNUSED FUNCTION BUT MAYBE USEFULL ONE DAY------------

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

Mat diffImg(Mat *t0, Mat *t1, Mat *t2){
	Mat d1;
	Mat d2;
	Mat out;
	absdiff(*t2, *t1, d1);
	absdiff(*t1, *t0, d2);
	bitwise_and(d1, d2, out);
	return out;
}

Rect detectFace( Mat *in ){
    std::vector<Rect> faces;
    cv::Rect out(0,0,0,0);

    //-- Detect face
    face_cascade.detectMultiScale( *in, faces, 1.1, 2, 0, Size(80, 80) );

    //We consider only ONE face, the biggest one...
    if(faces.size() != 0){
        for(unsigned int i=0; i< faces.size(); i++){
            if(faces[i].area() >= out.area()){
                out = faces[i];
            }
        }

        return out;
    }
    else
        return out;
}
