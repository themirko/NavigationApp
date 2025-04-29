cmake_minimum_required(VERSION 3.30)
project(NavigationApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DWITHOUT_NUMPY)


add_executable(NavigationApp
        main.cpp
        src/MapClass.cpp
        src/NodeClass.cpp
        src/EdgeClass.cpp
        src/KDTreeClass.cpp
)

find_path(MATPLOTLIB_CPP_INCLUDE_DIRS "matplotlibcpp.h")
target_include_directories(NavigationApp PRIVATE ${MATPLOTLIB_CPP_INCLUDE_DIRS})

include_directories("C:/Users/Dusan/AppData/Local/Programs/Python/Python313/include")
include_directories("C:/Users/Dusan/anaconda3/Lib/site-packages/numpy/core/include")

link_directories("C:/Users/Dusan/AppData/Local/Programs/Python/Python313/libs")

target_link_libraries(NavigationApp PRIVATE "C:/Users/Dusan/AppData/Local/Programs/Python/Python313/libs/python313.lib")
