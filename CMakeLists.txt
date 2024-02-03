cmake_minimum_required(VERSION 3.10)

string(ASCII 27 Esc)
set(ColourReset "${Esc}[m")
set(Red         "${Esc}[31m")
set(Green       "${Esc}[32m")
set(Yellow      "${Esc}[33m")
set(Blue        "${Esc}[34m")

project(otus-prof-hw)

add_subdirectory(hw01)
add_subdirectory(hw02)
add_subdirectory(hw03)

