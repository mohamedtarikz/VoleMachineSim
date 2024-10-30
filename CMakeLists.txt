cmake_minimum_required(VERSION 3.27)
project(VoleMachineSim)

set(CMAKE_CXX_STANDARD 14)

include_directories(Src)

add_executable(VoleMachineSim
        Src/ALU.cpp
        Src/CPU.cpp
        Src/CU.cpp
        Src/MainUI.cpp
        Src/Memory.cpp
        Src/Register.cpp
        Main.cpp
        Src/Machine.cpp)
