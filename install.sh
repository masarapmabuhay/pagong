#!/bin/bash

# Usbong Pagong System
#
# C/C++ Computer Language and Simple Directmedia Layer (SDL) + SDL Image + Open Graphics Library (OpenGL)
#
# Auto-install Application (with required libraries)
#
# @company: USBONG
# @author: SYSON, MICHAEL B.
# @date created: 20211115
# @last updated: 20220310; 20220102
# @website address: http://www.usbong.ph
#
# Additional Notes:
# 1) If asked by Computer what to do with the executable file, select: "Execute in Terminal"
# 

sudo apt-get update

#added by Mike, 20220102
sudo apt-get install -y g++ 

sudo apt-get install -y libsdl2-dev
sudo apt-get install -y libsdl2-image-dev
sudo apt-get install -y libsdl2-mixer-dev

sudo apt-get install -y libsdl2-mixer-dev

#added by Mike, 20220310
sudo apt-get install mesa-utils
sudo apt-get install freeglut3-dev

#added by Mike, 20211122
echo "--";
echo "Building Executable Application File...";
sudo ./make.sh
echo "Executable Application File now in output folder.";

echo "--";
echo "Usbong Pagong System: Installation Complete.";

sudo ./execute.sh
