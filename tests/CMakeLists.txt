cmake_minimum_required(VERSION 3.0)

add_executable(demo demo.cpp)
target_link_libraries(demo PUBLIC mutantur)

add_executable(demo_clone demo_clone.cpp)
target_link_libraries(demo_clone PUBLIC mutantur)