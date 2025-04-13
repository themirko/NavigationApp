cmake_minimum_required(VERSION 3.30)
project(NavigationApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NavigationApp
        main.cpp
        src/MapClass.cpp
        src/NodeClass.cpp
        src/EdgeClass.cpp
)

