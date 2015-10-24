cmake_minimum_required(VERSION 3.0)
project(test_fft)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -lfftw3 -lm -lfftw3f")

find_package (Threads)

set(SOURCE_FILES main.cpp)

add_executable(test_fft ${SOURCE_FILES})

target_link_libraries(test_fft  ${MYFLAGS} ${CMAKE_THREAD_LIBS_INIT} )