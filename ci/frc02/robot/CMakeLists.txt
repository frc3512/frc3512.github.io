cmake_minimum_required(VERSION 2.8)

set(NAME "FRCUserProgram")

project(${NAME})

set(CMAKE_C_FLAGS "-O3 -Wall -s -std=c11")
set(CMAKE_CXX_FLAGS "-O3 -Wall -s -std=c++14")

set(DIRS
    src/*
)

file(GLOB SRC *.c *.cpp)

foreach(ELEM ${DIRS})
    file(GLOB_RECURSE FILES ${ELEM}.c ${ELEM}.cpp)
    list(APPEND SRC ${FILES})
endforeach(ELEM)

include_directories(src src/WPILib)
include_directories($ENV{HOME}/wpilib/cpp/current/include
                    $ENV{HOME}/wpilib/user/cpp/include)
link_directories($ENV{HOME}/wpilib/cpp/current/reflib/linux/athena/shared
                 $ENV{HOME}/wpilib/common/current/lib/linux/athena/shared
                 $ENV{HOME}/wpilib/user/cpp/lib)
add_executable(${NAME} ${SRC})
target_link_libraries(${NAME} "wpi" "pthread")
