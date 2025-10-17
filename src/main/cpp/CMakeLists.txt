cmake_minimum_required(VERSION 3.10)
project(nativechunks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

add_library(Nativechunks SHARED Nativechunks.cpp
        worldParts/tree/Tree.cpp
        worldParts/tree/TreeFactory.cpp
        worldParts/tree/types/OakTree.h
        worldParts/tree/types/OakTree.cpp
        worldParts/tree/types/MediumOakTree.cpp
        worldParts/tree/types/MediumOakTree.h)

set_target_properties(Nativechunks PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)
