cmake_minimum_required(VERSION 3.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Od")
set(CMAKE_CXX_FLAGS_DEBUG "-Od")
set(CMAKE_CXX_FLAGS_RELEASE "-Od")

add_definitions(-DUNICODE -D_UNICODE)

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
