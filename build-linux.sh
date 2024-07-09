#!/bin/sh
# requirements:
#   git, gcc, g++, cmake

echo "Directory mounted from Windows"
cd /mnt/c/Repos/IctBaden.pjsua2
echo

echo "Cleanup existing PJSIP sources"
rm -rf pjproject/*
rm -rf pjproject/.git*
rm -rf pjproject/.editorconfig*
rm -f pjsua2.linux/*.so
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

echo "Apply media streaming extention"
cat "../media_ext/media_ext.hpp" >> "pjsip/include/pjsua2/media.hpp"
cat "../media_ext/media_ext.cpp" >> "pjsip/src/pjsua2/media.cpp"
echo

echo "Configure and create makefiles"
./configure --disable-ssl CFLAGS="-fPIC -Wno-unused-variable -Wno-unused-function -Wno-unused-value -Wno-unused-but-set-variable"
echo

echo "Make pjproject"
make dep
make
cd ..
echo

echo "CMake pjsua2.linux x64"
cp pjproject/pjlib/include/pj/limits.h pjproject/pjlib/include/pj/compat/limits.h
cd pjsua2.linux
cmake --configure .
cmake --build .
cd ..


echo "Copy artifact (pjsua2.so) to packets directory"
cp pjsua2.linux/libpjsua2.so packets/linux-x64/
echo


