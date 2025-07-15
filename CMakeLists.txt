cmake_minimum_required(VERSION 3.30)
project(DataStructures C)

set(CMAKE_C_STANDARD 23)

# Add static library
add_library(DataStructures STATIC
        lists/dynamic_array.c
        lists/dynamic_array.h
        lists/singly_linked_list.h
        lists/singly_linked_list.c
)

# Add executable
add_executable(DataStructuresE main.c)

# Link the executable with the DataStructures static library
target_link_libraries(DataStructuresE PRIVATE DataStructures)