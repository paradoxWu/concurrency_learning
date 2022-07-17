#!/bin/bash
if [ ! -d  "build" ];then
   mkdir build
else
   rm -rf build
   mkdir build
fi
cd build
echo "Begin compiling"
cmake  -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../ \
         -D OpenCV_DIR=D:/program/opencv/opencv/mingw_build/  \
         -DDEBUG=ON .. 
make
make install
cd ..

#D:\program files\opencv\opencv\build\x64\vc15\lib
