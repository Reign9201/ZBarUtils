cmake_minimum_required(VERSION 3.4.1)

set(APP_PLATFORM android-16)

set(LIBS_DIRECTORY ${CMAKE_SOURCE_DIR}/../../../libs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        ${ANDROID_NDK}/platforms/android-16/arch-arm/usr/include
        ${ANDROID_NDK}/platforms/android-16/arch-arm/usr/include/android
        include
        ZBar
        iconv/include
)



add_library(libiconv SHARED IMPORTED)
set_target_properties(libiconv PROPERTIES IMPORTED_LOCATION ${LIBS_DIRECTORY}/${ANDROID_ABI}/libiconv.a)

add_library(
        QRDecoder
        SHARED

        zbarjni.c
        ZBarDecoder.cpp
        zbar/img_scanner.c
        zbar/decoder.c
        zbar/image.c
        zbar/symbol.c
        zbar/convert.c
        zbar/config.c
        zbar/scanner.c
        zbar/error.c
        zbar/refcnt.c
        zbar/video.c
        zbar/video/null.c
        zbar/decoder/code128.c
        zbar/decoder/code39.c
        zbar/decoder/code93.c
        zbar/decoder/codabar.c
        zbar/decoder/databar.c
        zbar/decoder/ean.c
        zbar/decoder/i25.c
        zbar/decoder/qr_finder.c
        zbar/qrcode/bch15_5.c
        zbar/qrcode/binarize.c
        zbar/qrcode/isaac.c
        zbar/qrcode/qrdec.c
        zbar/qrcode/qrdectxt.c
        zbar/qrcode/rs.c
        zbar/qrcode/util.c

)

find_library(log-lib log)
find_library(stdc++-lib stdc++)
find_library(z-lib z)
find_library(jnigraphics-lib jnigraphics)

target_link_libraries(
        QRDecoder
        libiconv

        ${stdc++-lib}
        ${log-lib}
#        ${z-lib}
#        ${jnigraphics-lib}
)