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
// Name        : Settings.cpp
// Author      : MrMoDDoM
// Version     : 1.0
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

#include "Settings.h"

void Settings::set_default(){
	//MuHi.h
	this->MAIN_WIN_TITLE = "MuHi";
        this->X_RESOLUTION = 640;
        this->Y_RESOLUTION = 480;
	//MuHi.cpp
	this->debug = false;
	this->thres = 350;
	this->blinkTresh = 10;
	this->countSameStatus = 1;
	//WebcamWorker.cpp
	this->defaultCam = 0;
	this->face_cascade_path = "cascade/lbpcascade_frontalface.xml";
	this->eyes_left_cascade_path = "cascade/haarcascade_mcs_lefteye.xml";
	this->eyes_right_cascade_path = "cascade/haarcascade_mcs_righteye.xml";
	this->kEyePercentTop = 31;
        this->kEyePercentSide = 15;
        this->kEyePercentHeight = 23;
        this->kEyePercentWidth = 30;
	this->STABILITY_THRESHOLD = 5.0;
	this->blinkThresh = 7; //MAYBE THIS IS NOT IMPORTANT - REDUNDANT VALUE! -->blinkTresh
	this->eyeArrayDim = 6;
	this->left_thresh = 80;
	this->right_thresh = 80;
	//OutputWorker.cpp
	this->streamer = false;
	this->keyBinding = "012345";
	this->longPress = false;
	this->onlyClosedStates = false;
}


//char * Settings::get_name(){
//	return name;
//}
