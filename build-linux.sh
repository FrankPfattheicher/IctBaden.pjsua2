#!/bin/sh

# requirements:
#   git, gcc, g++

echo "Directory mounted from Windows"
cd /mnt/c/Repos/IctBaden.pjsua2
echo

echo "Cleanup existing PJSIP sources"
rm -rf pjproject/*
rm -rf pjproject/.git*
echo

echo "Get current version of PJSIP sources"
git clone https://github.com/pjsip/pjproject.git
echo

echo "Set config_site.h"
cat config_site.h | tr -d '\r' > pjproject/pjlib/include/pj/config_site.h
echo

echo "Change to project directory"
cd pjproject
echo

echo "Configure and create makefiles"
./configure --enable-static --disable-ssl CFLAGS="-fPIC"
echo

echo "Make pjproject"
make dep
make
cd ..
echo

echo "CMake pjsua2.linux"
cd pjsua2.linux
cmake --configure .
cmake --build .
cd ..
echo


echo "Copy artifact (pjsua2.so) to packets directory"
cp pjsua2.linux/libpjsua2.so ../packets/linux-x64/
echo

