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
// Name        : ProcessWorker.h
// Author      : MrMoDDoM
// Version     : 1.1
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

#include "ProcessWorker.h"

//GLOBAL

////////////////////////////////////
//  VARIABLES
////////////////////////////////////
Settings *PWsetting;
////////////////////////////////////
//This function lunch the external program and return 0 on success
int initProcessWorker(Settings * _set){
	//Copy settings pointer
	PWsetting = _set;

	#ifdef __linux__
	//Need to use fork()-exec() to run process

	#endif

	//Guess what?... CODING FOR WINDOWS IS F***CKING IMPOSSIBLE!!!
	#ifdef _WIN32
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	//Yes...YES! This part is copied from Microsoft examples...!
	// Start the child process.
	if( !CreateProcess( NULL,   // No module name (use command line)
		PWsetting->pathToTargetProgram, // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
	){
		//printf( "CreateProcess failed (%d).\n", GetLastError() );
		return 1;
	}

	#endif


	cout<<"Lunching "<<PWsetting->pathToTargetProgram<<endl;
	return 0; //All went ok!
}

//This function return 0 if the target program is still running, 1 if not
int targetIsStillRunning(){

	#ifdef __linux__
	//Need to use kill(pid,0) to find if process had exited

	#endif

	#ifdef _WIN32
	DWORD exitCode = 0;
	if (GetExitCodeProcess(pi.hProcess, &exitCode) == FALSE){
		// Handle GetExitCodeProcess failure - something went wrong...
		return -1;
	}

	if (exitCode != STILL_ACTIVE){
		return 1; //The target program has stopped!
	}

	#endif

	return 0; //Ok, the target is still running
}


