
cmake_minimum_required(VERSION 3.16)

project(pjsua2_linux VERSION 1.0.1)

set (_lib_name "pjsua2")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)

add_compile_options(-fPIC)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib-util/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjnath/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/include"
)

add_library(${_lib_name} SHARED
        library.cpp
        library.h
        pj-media-neg.cpp
        pj-media-neg.h
        pjsua2_wrap.cpp
        pjsua2_wrap.h
        pjsua2.i
        )
set_target_properties(${_lib_name} PROPERTIES OUTPUT_NAME ${_lib_name})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(INC_DIRS
        "../pjproject/pjlib/include"
        "../pjproject/pjlib/include/pj"
        )

target_link_libraries(${_lib_name}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib/lib/libpj-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib-util/lib/libpjlib-util-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsip-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsua-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsua2-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjnath/lib/libpjnath-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-audiodev-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-codec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-videodev-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjsdp-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libg7221codec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libgsmcodec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libilbccodec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libresample-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libspeex-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libsrtp-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libwebrtc-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libyuv-x86_64-unknown-linux-gnu.a"

        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib/lib/libpj-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib-util/lib/libpjlib-util-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsip-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsua-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsua2-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjnath/lib/libpjnath-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-audiodev-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-codec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-videodev-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjsdp-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libg7221codec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libgsmcodec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libilbccodec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libresample-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libspeex-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libsrtp-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libwebrtc-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libyuv-x86_64-unknown-linux-gnu.a"

        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib/lib/libpj-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjlib-util/lib/libpjlib-util-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsip-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsua-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjsip/lib/libpjsua2-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjnath/lib/libpjnath-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-audiodev-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-codec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-videodev-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjmedia-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/pjmedia/lib/libpjsdp-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libg7221codec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libgsmcodec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libilbccodec-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libresample-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libspeex-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libsrtp-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libwebrtc-x86_64-unknown-linux-gnu.a"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../pjproject/third_party/lib/libyuv-x86_64-unknown-linux-gnu.a"
        )