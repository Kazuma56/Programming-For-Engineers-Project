cmake_minimum_required(VERSION 3.15)
project(Programming_For_Engineers_Project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(Programming_For_Engineers_Project
    main.c
    waveform.c
    io.c
)

target_link_libraries(Programming_For_Engineers_Project m)
