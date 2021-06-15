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
./configure --enable-shared
echo

echo "Make"
make dep
make
echo

echo "Copy artifact (pjsua2.so) to packets directory"
cp pjsip/lib/libpjsua2.so ../packets/linux-x64/pjsua2.so
cd ..
echo
