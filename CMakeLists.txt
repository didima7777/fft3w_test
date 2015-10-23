cmake_minimum_required(VERSION 3.0)
project(test_dima)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -lfftw3 -lm -lfftw3f")

find_package (Threads)

set(SOURCE_FILES main.cpp)

add_executable(test_dima ${SOURCE_FILES})
target_link_libraries(test_dima  ${MYFLAGS} ${CMAKE_THREAD_LIBS_INIT} )