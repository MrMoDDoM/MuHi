///////////////////////////////////////////////////////////////
//                     MuHi
//         Multifunctionals E(/Hi)yes
//
//   @author: Daniele Barattieri di San Pietro
//   @email : danielebarattieri[at]gmail.com
//   @copyright: 2016
//
//  ----------------------------------------------------------
//  This project is hosted and supported by the µHack assosiation,
//  with love from Brescia, Italy ( www.muhack.org ) ;)
//  The aim of this project is to provide a software assistance for
//  tetraplegic, using a webcam to detect the eyes' blinks.
//  ----------------------------------------------------------
//
//  This Software is distribuited under the GNU/GPL license
//  Fot more informations, please see the LICENSE and README
//  file attached to the source code
//
////////////////////////////////////////////////////////////////

//============================================================================
// Name        : Writer.h
// Author      : MrMoDDoM
// Version     : 0.5
// Copyright   : GNU/GPL
// Description : MuHi in C++, Ansi-style
//============================================================================

/*
    This file is part of MuHi.
    Copyright (C) 2016  Daniele Barattieri di San Pietro

    MuHi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MuHi.  If not, see <http://www.gnu.org/licenses/> or
    <http://www.muhack.org/> for futher datails.

*/

#ifndef WRITER_H_
#define WRITER_H_


#include <iostream>
#include <string.h>
#include <stdio.h>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
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
	static const double char_fontScale = 1.5;

public:
	Action *getNext();
	Action *getBefore();
	void draw(Mat *in);
	bool selected;
	float x,y,h,w;
	char text[100];
	Action *next;
	Action *before;
	int id;

	enum Types {ACTION, LETTER, SPACER};
	int type;

};

//Mi serve ancora questa sotto-classe?
class Letter : public Action{

public:
	void draw(Mat *in);
	/*Letter next;
	bool selected;
	float x,y,h,w;*/
	char c;
};

#endif /* WRITER_H_ */
