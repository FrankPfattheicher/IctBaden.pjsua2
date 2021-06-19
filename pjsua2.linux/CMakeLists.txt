
cmake_minimum_required(VERSION 3.19)

project(pjsua2_linux VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        "../pjproject/pjlib/include"
        "../pjproject/pjlib-util/include"
        "../pjproject/pjmedia/include"
        "../pjproject/pjnath/include"
        "../pjproject/pjsip/include"
        "../pjproject/third_party/include"
)

add_library(pjsua2_linux SHARED
        library.cpp
        library.h
        pjsua2.i
        pjsua2_wrap.cpp
        pjsua2_wrap.h
        pj-media-neg.cpp
        pj-media-neg.h
        )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(INC_DIRS
        "../pjproject/pjlib/include"
        "../pjproject/pjlib/include/pj"
        )

find_library(pjsua2_linux
        ../pjproject/pjlib/lib/libpj-x86_64-unknown-linux-gnu.a
        ../pjproject/pjlib-util/lib/libpjlib-util-x86_64-unknown-linux-gnu.a
        ../pjproject/pjmedia/lib/libpjmedia-audiodev-x86_64-unknown-linux-gnu.a
        ../pjproject/pjmedia/lib/libpjmedia-codec-x86_64-unknown-linux-gnu.a
        ../pjproject/pjmedia/lib/libpjmedia-videodev-x86_64-unknown-linux-gnu.a
        ../pjproject/pjmedia/lib/libpjmedia-x86_64-unknown-linux-gnu.a
        ../pjproject/pjmedia/lib/libpjsdp-x86_64-unknown-linux-gnu.a
        ../pjproject/pjnath/lib/libpjnath-x86_64-unknown-linux-gnu.a
        ../pjproject/pjsip/lib/libpjsip-x86_64-unknown-linux-gnu.a
        ../pjproject/pjsip/lib/libpjsua-x86_64-unknown-linux-gnu.a
        ../pjproject/pjsip/lib/libpjsua2-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libg7221codec-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libgsmcodec-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libilbccodec-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libresample-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libspeex-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libsrtp-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libwebrtc-x86_64-unknown-linux-gnu.a
        ../pjproject/third_party/lib/libyuv-x86_64-unknown-linux-gnu.a
        )