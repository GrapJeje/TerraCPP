cmake_minimum_required(VERSION 3.10)
project(nativechunks)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

add_library(nativechunks SHARED nativechunks.cpp)

set_target_properties(nativechunks PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)
