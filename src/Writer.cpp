/*
 * Writer.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#include "Writer.h"

char text[100];

float X_STARTER = 30;
float Y_STARTER = 150;
float LETTER_DIM = 50;

int fontFace = FONT_HERSHEY_TRIPLEX;

//Letter alphabet[26];

Action alphabet[30]; //Plust dot, question mark, space and comma
Action actions[4];

Action *nowSelected;

//std::stringstream phrase;

std::string phrase;

bool forward = true;

bool leftClose, rightClose;

char *email = "danielebarattieri@gmail.com";

void resetSelect(){
	//We start from the first
	nowSelected->selected = false;
	nowSelected = &alphabet[0];
	nowSelected->selected = true;
	//phrase.str("");
}

void checkBlink(int blinkStatus){

	//Update the writer's eyes status
	switch (blinkStatus){
	case 0: //Both open
		leftClose = false;
		rightClose = false;
		break;
	case 1: //Right close - left open
		if(leftClose)
			leftClose = false;

		if(!rightClose){
			rightClose = true;
			click();
		}
		break;

	case 2: //Right open - left close
		if(!leftClose){
			leftClose = true;
			forward = !forward;
		}

		if(rightClose){
			rightClose = false;
		}
		break;

	case 3: //Both close
		leftClose = true;
		rightClose = true;
		break;
	}
}

void stepWriter(){
	nowSelected->selected = false;
	Action *temp;

	if(forward){

		temp = nowSelected->getNext();

	} else {
		temp = nowSelected->getBefore();
	}

	temp-> selected = true;

	nowSelected = temp;
}

void click(){

	//std::stringstream o;
	switch (nowSelected->type){
	case 0: //The enter command
		if(!strcmp(phrase.data(), "MRMODDOM"))
			phrase = "Daniele";
		else phrase = "";
		//o << "espeak -v italian -s 290 " <<phrase.str();

		//system(o.str());
		break;

	case 1: //Delete last char
		if (phrase.size () > 0) phrase.resize (phrase.size () - 1);
		break;

	case 2: //A normal char
		phrase.append(nowSelected->text);
		break;
	}

	resetSelect();
}

int initWriter(){

	leftClose = false;
	rightClose = false;


	int step = 0;

	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 6; j++){
			if(step < 26){
				alphabet[step].text[0] = (char) step + 65;
			} else if (step == 26){ //The question mark
				alphabet[step].text[0] = '?';
			} else if (step == 27){ //The space
				alphabet[step].text[0] = ' ';
			} else if (step == 28){ //The dot
				alphabet[step].text[0] = '.';
			} else if (step == 29){ //The comma
				alphabet[step].text[0] = ',';
			}
			alphabet[step].x = X_STARTER + ( j * LETTER_DIM);
			alphabet[step].y = Y_STARTER + ( i * LETTER_DIM);
			alphabet[step].h = LETTER_DIM;
			alphabet[step].w = LETTER_DIM;
			alphabet[step].selected = false;
			alphabet[step].type = 2;

			step++;
		}
	}

	//Action button
	for (int i = 0; i < 4; i++){
		actions[i].x = 400;
		actions[i].y = 200 + ( i * LETTER_DIM);
		actions[i].h = LETTER_DIM;
		actions[i].w = 200;
	}

	strcpy(actions[0].text, "Invio");
	actions[0].type = 0;
	strcpy(actions[1].text, "Canc");
	actions[1].type = 1;

	strcpy(actions[2].text, "SI ");
	actions[2].type = 2;
	strcpy(actions[3].text, "NO ");
	actions[3].type = 2;

	//Now we need to connect every Action to each next Action

	//FIRST TRY: we connect every letter without the selector

	for(int i = 0; i < 30; i++){
		alphabet[i].next = &alphabet[ i + 1];
	}

	alphabet[29].next = &actions[0];
	actions[0].next = &actions[1];
	actions[1].next = &actions[2];
	actions[2].next = &actions[3];
	actions[3].next = &alphabet[0];
	//actions[7].next = &alphabet[0];

	//AAAAAND BACKWARD! :D
	for(int i = 29; i > 0; i--){
		alphabet[i].before = &alphabet[i - 1];
	}

	alphabet[0].before = &actions[3];
	actions[3].before = &actions[2];
	actions[2].before = &actions[1];
	actions[1].before = &actions[0];
	actions[0].before = &alphabet[29];

	//Start from the "A"
	nowSelected = &alphabet[0];
	resetSelect();

	return 0;
}

void drawHUD(Mat *in){
	//*in = Scalar(59,61,40); //Background color

	*in = Scalar(245,200,64);

	//Phrase background
	rectangle(*in, Point(0,0), Point(in->cols, 100), Scalar::all(255), CV_FILLED ,1,0 );

	//Draw all the letter
	for (int i = 0 ; i < 30; i++){
		alphabet[i].draw(in);
	}

	for(int i = 0 ; i < 8; i++){
		actions[i].draw(in);
	}

	//Draw the phrase
	putText(*in, phrase, Point(40, 70), fontFace, 1, Scalar::all(0), 3, 1, false);

	//Draw eye status

	std::stringstream status;

	status.str("");

	if(leftClose)
		status<<"Left Closed";
	else
		status<<"Left Open";

	putText(*in, status.str(), Point(640 - 200, 480 - 400), fontFace, 1, Scalar::all(0), 1, 1, false);

	status.str("");

	if(rightClose)
		status<<"Right Closed";
	else
		status<<"Right Open";

	putText(*in, status.str(), Point(640 - 200, 480 - 450), fontFace, 1, Scalar::all(0), 1, 1, false);


	//Write email

	putText(*in, email, Point(640 - 600, 480 - 10), fontFace, 0.5, Scalar::all(0), 1, 1, false);
}

void Action::draw(Mat *in){
	int thickness;
	Scalar color;
	if(selected){
		thickness = 2;
		color = Scalar(0,0,255);
	} else {
		thickness = 1;
		color = Scalar(0,0,0);
	}

	//Draw the box
	rectangle(*in, Point(x,y), Point (x + w, y + h), color, thickness, 1, 0);

	putText(*in, text, Point(x + (w / 10) ,y + ( (h * 9) / 10 ) ), fontFace, char_fontScale, color, thickness, 1, false);


}

Action* Action::getNext(){
		return next;
}

Action* Action::getBefore(){
		return before;
}

void Letter::draw(Mat *in){

	//E' possibile chiamare il metodo della propria classe super?
	//Action::draw(in);

	int thickness;
	Scalar color;
	if(selected){
		thickness = 2;
		color = Scalar(0,0,255);
	} else {
		thickness = 1;
		color = Scalar(0,0,0);
	}

	//Draw the box
	rectangle(*in, Point(x,y), Point (x + w, y + h), color, thickness, 1, 0);
}


