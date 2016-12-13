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
// Name        : MuHi.cpp
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

#ifndef MUHI_H_
#define MUHI_H_

#define VERSION 1.0

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
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
bool blinkRigth, blinkLeft, fin;
char key;
FILE *program; //Maybe not usefull, but let's just leave that here...
////////////////////////////////////
////////////////////////////////////
//  FUNCTIONS
////////////////////////////////////
int init();
int exit();
int open_program( FILE *f, std::string path);
void print_logo();
//int sendKeyboardKey(int blkSts);
////////////////////////////////////
#endif /* MUHI_H_ */
