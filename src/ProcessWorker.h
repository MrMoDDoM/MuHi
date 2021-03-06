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

#ifndef PROCESSWORKER_H_
#define PROCESSWORKER_H_

#define VERSION 1.1

/*#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>*/

#include "Settings.h"

//Linux switch
#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#ifdef _WIN32
#define WINVER 0x0500
#include <windows.h>
#endif

////////////////////////////////////
//  FUNCTIONS
////////////////////////////////////
int initProcessWorker(Settings * _set);
int targetIsStillRunning();
////////////////////////////////////
#endif /* PROCESSWORKER_H_ */
