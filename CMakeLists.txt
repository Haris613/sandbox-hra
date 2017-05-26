cmake_minimum_required(VERSION 3.7)
project(semestralka)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp enemy.h tiles.cpp tiles.h solid.h liquid.cpp liquid.h water.cpp water.h lava.cpp lava.h dirt.cpp dirt.h stone.cpp stone.h menu.h)
add_executable(semestralka ${SOURCE_FILES} enemy.h)