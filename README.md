# <t>MuHi
                     
##<t>Multifunctionals E(/Hi)yes

[![MuHi - Test](http://img.youtube.com/vi/SLFAlyaToa4/0.jpg)](https://www.youtube.com/watch?v=SLFAlyaToa4 "MuHi - Beta Test")

   @author: Daniele Barattieri di San Pietro<br>
   @email : danielebarattieri[at]gmail.com<br>
   @copyright: 2016<br>
   @version: 1.0 - public beta
   
# README UNDER COSTRUCTION!

  ----------------------------------------------------------
  This project is hosted and supported by the hackersapece MuHack,<br>
  with love from Brescia, Italy ( www.muhack.org ) ;)<br>
  ----------------------------------------------------------

  This Software is distribuited under the GNU/GPL license.<br>
  Fot more informations, please see the LICENSE and README file attached to the source code.

## Aim
The aim of this project is to provide an interaction with coputers to people with reducted movment ( tetraplegic patients ) using common webcam to detect eyes' blinks; not only to be able again to comunicate with other, but to generally improve their quality of life.

### Games and Applications
**This is a call for you, disigners!**
I'm looking for (game) designers and developers to re-think common application and game focusing the input by one or two signal.
For example, take the calssical Sudoku game using a input system similar to the one used to write in this version.<br>
Now, let's think about a program for drawing o something to read a ebook...<br>
In future version, I'm thinking to implement some sort of system to interact with mouse and keyboard, but I am not sure about the usability for the final user: remeber that mouse and keyboard was designed to be used with hands, not eyes..<br>

With the hackerspace MuHack, we are hosting a separate repo for all your ideas and code: it is not important that you can or cannot code something, we need ideas!

## Installation & Usange
**Requirements**
- opencv >= 2.4
- (Linux) xdo >=3

### Linux
We need OpenCV develompment library and XDoTool to compile and execute the program:
```
sudo apt-get install libopencv-dev python-opencv libxdo-dev
git clone https://github.com/MrMoDDoM/MuHi.git
cd MuHi/
./compile.sh
```

**To compile the Writer test app**
```
cd src/Writer/
./compile_writer.sh
```

...and now to lunch a program, for example the **writer** use:
```
./MuHi -p ./src/Writer/writer
```

### Windows
Just donwload & extract the zip file from [this link](https://github.com/MrMoDDoM/MuHi/releases/download/v1.1/MuHi-v1.1.zip)

### Basic usange
With the version 1.0, MuHi is now a stand alone application intended to be the "caller" of other application.
It sends keystroke based on eye status: the key is presed and released quikly, and not hold down.
As example application, the "Writer" is now a stand alone application too. It behaves exactly like in the 0.5 version, with the difference that now you have to different thread running.

There also some command-line options:
- -h                 Print this help and exit
- -p PATH_TO_PROGRAM The path to the target program: the system will lunch and attach to it
- -nologo            Lunch the system without printing the logo
- -i                 Send only eye's close state: the system will react only when only one or both eye are close
- -l                 Long press the key: the virtual keystroke is released only on change
- -d                 Activate the debug mode: in this mode the source frame and some information are displayed
- -s                 Activate the streaimg mode: this way MuHi will continuosly output the status of eyes, and not only on change
- -c NUM             Select the webcam index. 0 is normally the internal/default and 1 is the first USB cam connected.
- -k XXXXXX          SIX and ONLY SIX charater to set as custom key sent on event. They stand for:
--         FIRST:          Both eye are open
--         SECOND:         Rigth eye is close, left is open
--         THIRD:          Rigth eye is open, left is close
--         FOURTH:         Both eye are close
--         FIFTH:          Temporary error (e.g. face not found, wating frame, ecc)
--         SIXTH:          Critical error, shut everything down.

##EXAMPLE:
```
./MuHi -c 1 -p [PATH/TO/]notepad.exe
```
This will use the webcam with index 1 and activate the debug mode,lunching notepad.exe to work with
```
./MuHi -s -k asdfgh game.exe
```
This will set custom keys to "asdfgh", activate the streaming mode and lunch game.exe

### Writer test app
The system uses the LEFT eye's blink to change the selector's direction, and the RIGHT eye's blink to make a "click"<br>
To pause or resume the writing process "click" on the "PAUSA" button<br>
To delete the last charater, use "CANC" and to empty the output use "INVIO"<br>


**Tips & Tricks: stay in a well lighted room, with no makeup or object (hair or glasses) covering your face.** 

## To-Do
- [x] Basic detection algorithm
- [x] Code structure object-oriented
- [ ] Documentations
- [ ] Code refactoring
- [ ] API or a system to let other application/system to interact with (done in Linux)

# Changelog 

### v1.1
- Thread support: MuHi is now able to lunch a target application and detect when it closes to shut down itself; this prevent MuHi to stay active and send random keystroke to random windows.
- Reimplemented Windows support, but at this point MuHi doesn't support custom keybinding on Windows
- With the "-i" switch is now possible to select only the "important" state changes (when from and open state the eye closes), which means the MuHi output only the second, third and fourth state.
- Correct some bugs that prevent MuHi to sustain "long blinks".
- With the "-l" switch the virtual key is held down insted of a quick press-and-release, even with "long blink".
- The "-nologo" switch prevents MuHi to print the opening logo in the terminal
- Bug fixes and stability improvments


### v1.0 - public beta
- MuHi is now a stand alone application: it now sends keystroke (0-1-2-3-4) to the focused window
- Temporally removed Windows support
- Added threshold auto-adjusting
- Moved Writer as a stand alone application
- Moved the cascade classifier to a separate folder

### v0.5
- Public beta
- Added windows support
- Bug fix
- Speed improvment

### v0.4
- Added "YES/NO" button
- Three different type of selector

### v0.3
- Project started

# Contact
```
danielebarattieri#gmail.com
```

# Copyright


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
