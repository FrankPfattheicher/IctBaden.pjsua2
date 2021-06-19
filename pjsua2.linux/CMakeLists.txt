cmake_minimum_required(VERSION 3.19)
project(pjsua2_linux)

set(CMAKE_CXX_STANDARD 14)

add_library(pjsua2_linux SHARED
        library.cpp
        library.h
        pj-media-neg.cpp
        pj-media-neg.h
        pjsua2.i
        pjsua2_wrap.cpp
        pjsua2_wrap.h)
