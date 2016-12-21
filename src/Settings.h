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
// Name        : Settings.h
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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

//Setting class: this class serves and holds all the important data and variables of MuHi system
class Settings{

	public:
		//MuHi.h
		string MAIN_WIN_TITLE;
		int X_RESOLUTION;
		int Y_RESOLUTION;
		//MuHi.cpp
		bool debug;
		int thres;
		int blinkTresh;
		int countSameStatus;
		//WebcamWorker.cpp
		int defaultCam;
		string face_cascade_path;
		string eyes_left_cascade_path;
		string eyes_right_cascade_path;
		int kEyePercentTop;
		int kEyePercentSide;
		int kEyePercentHeight;
		int kEyePercentWidth;
		float STABILITY_THRESHOLD;
		int blinkThresh;
		int eyeArrayDim;
		int left_thresh; //Dinamic treshold, maybe is not important to place here in settings...
		int right_thresh; //Dinamic treshold, maybe is not important to place here in settings...
		//OutputWorker.cpp
		bool streamer;
		string keyBinding;
		bool longPress;
		bool onlyClosedStates;
		//Functions
		void set_default(); //Set default value
//		char* get_name();
};

#endif /* SETTINGS_H_ */
