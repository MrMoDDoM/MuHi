#!/bin/bash
clear && python -c 'print "#"*80' && g++ -o MuHi src/*.cpp `pkg-config --cflags --libs opencv` -fpermissive
