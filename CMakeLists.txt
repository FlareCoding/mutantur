cmake_minimum_required(VERSION 3.0)

project(mutantur)

include_directories(src)

add_library(
    ${PROJECT_NAME}

    src/utils.h
    src/utils.cpp
    src/mutantur.h
    src/mutantur.cpp
)

add_subdirectory(tests)
