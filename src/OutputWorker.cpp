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
// Name        : OutputWorker.cpp
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

#include "OutputWorker.h"

Settings *OWsetting;
int old_status_sent = 0;

#ifdef __linux__
xdo_t *xdo; //Xdo struct for serving virtual keystroke
#endif

#ifdef _WIN32
// This structure will be used to create the keyboard input event.
INPUT ip;
#endif

//Setting up evn
int initOutputWorker(Settings * _set){
	//Copy the setting pointer
	OWsetting = _set;

	//Init for linux
	#ifdef __linux__
	xdo = xdo_new(NULL);
	#endif
	//Init for win
	#ifdef _WIN32
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	#endif

	return 0; //All went ok!
}


int sendKeyboardKey(int blkSts){

	//Here we check if we are streaming the eye status or just want to send it if change
	if(!OWsetting->streamer && old_status_sent == blkSts)
		return 1; //Key not sent, but it's ok because we are rejecting same status when we are streaming

	#ifdef __linux__
	////Need to remake something better than std::to_string(blkSts,c_str()
//	xdo_send_keysequence_window(xdo, CURRENTWINDOW, std::to_string(blkSts).c_str(), 0);
	xdo_send_keysequence_window(xdo, CURRENTWINDOW, OWsetting->keyBinding.substr(blkSts,1).c_str(), 0); //Maybe we should check if the string is long enough
	#endif


	//F**CK WINDOWS! WHY THIS VIRTUALKEY STUFF NEEDS TO BE SO F***CKIN COMPLICATED?!?!?!?!?
	//LOOK AT THIS MESS! COM'ON! R U KIDDING ME?!
	#ifdef _WIN32
	// Press the key
	//char *key = OWsetting->keyBinding.data();
	ip.ki.wVk = 0x30 + blkSts; // virtual-key code - 0x30 is the '0' (zero) char, then we add the blkSts to get the right key-code
	//ip.ki.wVk = hex<<(int)key[blkSts]; // virtual-key code
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
	#endif

	old_status_sent = blkSts; //Update the last status sent

	return 0; //Key send!
}
