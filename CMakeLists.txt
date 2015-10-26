cmake_minimum_required(VERSION 3.0)
project(test_fft)

SET(CMAKE_CXX_FLAGS "-O3 -std=gnu++11 -lfftw3f -lm -pthread -ffloat-store -ffp-contract=fast")

set(SOURCE_FILES main.cpp)

add_executable(test_fft ${SOURCE_FILES})

target_link_libraries(test_fft)