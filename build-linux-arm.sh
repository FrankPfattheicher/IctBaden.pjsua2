#!/bin/bash
cd /mnt/c/Repos/IctBaden.pjsua2
cd pjproject
./configure --enable-shared --host=arm-elf-linux
make dep
make


