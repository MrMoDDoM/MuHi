# <t>MuHi
                     
##<t>Multifunctionals E(/Hi)yes

   @author: Daniele Barattieri di San Pietro<br>
   @email : danielebarattieri[at]gmail.com<br>
   @copyright: 2016<br>
   @version: 0.5 - open beta

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
**Requirements** : opencv >= 2.4

### Linux
We need OpenCV develompment library to compile and execute the program:
```
sudo apt-get install libopencv-dev python-opencv
git clone https://github.com/MrMoDDoM/MuHi.git
cd MuHi/
./compile.sh
./muhi
```

**At this moment the program uses only the first camer found (/dev/video0). You can swhitch camer with this (change X with the desire id):**
```
sudo mv /dev/video0 /dev/videoTEMP
sudo mv /dev/videoX /dev/video0
sudo mv /dev/videoTEMP /dev/videoX
```
### Windows
Just donwload & extract the zip file from [this link](http://www.muhack.org/muhi.zip) (we will upload soon the new complied version..!)

### Basic usange
There are only few commands implemented so far:
- ESC to esc the program
- D to activate the debug system, usefull to understand what the application sees
- +/- to increase or decrease the speed
- A/S to increase or decrease the sensibility to trigger 
- C/V to increase or decrease the eye detection area threshold

The system uses the LEFT eye's blink to change the selector's direction, and the RIGHT eye's blink to make a "click"<br>
To pause or resume the writing process "click" on the "PAUSA" button<br>
To delete the last charater, use "CANC" and to empty the output use "INVIO"<br>

**Tips & Tricks: stay in a well lighted room, with no makeup or object covering your face.** 

## To-Do
- [x] Basic detection algorithm
- [x] Code structure object-oriented
- [ ] Documentations
- [ ] Code refactoring
- [ ] API or a system to let other application/system to interact with



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
