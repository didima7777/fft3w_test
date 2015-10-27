cmake_minimum_required(VERSION 3.0)
project(test_fft)

SET(CMAKE_CXX_FLAGS "-std=gnu++11 -lfftw3 -lm -pthread")

set(SOURCE_FILES main.cpp)

add_executable(test_fft ${SOURCE_FILES})

target_link_libraries(test_fft)