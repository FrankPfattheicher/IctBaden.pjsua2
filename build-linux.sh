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
./configure
echo

echo "Make"
make dep
make
echo

echo "Copy artifact (pjsua2.so) to packets directory"
#cp pjsip/lib/libpjsua2.so ../packets/linux-x64/pjsua2.so
#cp pjlib/lib/*.so.2 ../packets/linux-x64/
#cp pjlib-util/lib/*.so.2 ../packets/linux-x64/
#cp pjmedia/lib/*.so.2 ../packets/linux-x64/
#cp pjnath/lib/*.so.2 ../packets/linux-x64/
#cp pjsip/lib/*.so.2 ../packets/linux-x64/
#cp third_party/lib/*.so.2 ../packets/linux-x64/
cd ..
echo

