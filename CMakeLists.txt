cmake_minimum_required(VERSION 3.16.3)
project(Octagon)

set(CMAKE_CXX_STANDARD 17)

include_directories(Octagon_SOURCE_DIR)

set(${EXECUTABLE_OUTPUT_PATH} ${Octagon_SOURCE_DIR}/bin)

add_executable(Octagon main.cpp)