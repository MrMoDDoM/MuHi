/*
 * Writer.h
 *
 *  Created on: Apr 28, 2016
 *      Author: mrmoddom
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <string.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <highgui.h>

using namespace cv;

void resetSelect();
void checkBlink(int blinkStatus);
int initWriter();
void drawHUD(Mat *in);
void stepWriter();
void click();

/*struct Action{
	double char_fontScale = 1.5;
	bool selected;
	float x,y,h,w;
	char text[100];
	Action next;
};*/

class Action{
private:
	double char_fontScale = 1.5;

public:
	Action *getNext();
	Action *getBefore();
	void draw(Mat *in);
	bool selected;
	float x,y,h,w;
	char text[100];
	Action *next;
	Action *before;

	enum Types {ACTION, LETTER, SPACER};
	int type;

};

class Letter : public Action{


public:
	void draw(Mat *in);
	/*Letter next;
	bool selected;
	float x,y,h,w;*/
	char c;
};

#endif /* WRITER_H_ */
