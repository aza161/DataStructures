cmake_minimum_required(VERSION 3.30)
project(DataStructures C)

set(CMAKE_C_STANDARD 23)

add_library(DataStructures STATIC library.c
        "Lists/Dynamic Array.c"
        "Lists/Dynamic Array.h")
