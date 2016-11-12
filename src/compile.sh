#!/bin/bash
clear && python -c 'print "#"*80' && g++ -o eyeTraking *.cpp `pkg-config --cflags --libs opencv` -fpermissive
