cmake_minimum_required(VERSION 3.19)
project(pjsua2_linux)

set(CMAKE_CXX_STANDARD 14)

add_library(pjsua2_linux SHARED library.cpp library.h)