cmake_minimum_required(VERSION 3.30)
project(NavigationApp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DWITHOUT_NUMPY)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(NavigationApp
  main.cpp
  src/map/Map.cpp
  src/map/Node.cpp
  src/map/Edge.cpp
  src/map/KDTree.cpp
  src/map/PathDataStruct.cpp
  src/utils/GeoUtils.cpp
  #src/utils/plotter.cpp
  src/gui/MapWindow.cpp
  include/gui/MapWindow.hpp
)

target_include_directories(NavigationApp PRIVATE
  ${Qt6Widgets_INCLUDE_DIRS}
)

#find_path(MATPLOTLIB_CPP_INCLUDE_DIRS "matplotlibcpp.h")
#target_include_directories(NavigationApp PRIVATE ${MATPLOTLIB_CPP_INCLUDE_DIRS})

include_directories(
  "C:/Users/Dusan/AppData/Local/Programs/Python/Python313/include"
  "C:/Users/Dusan/anaconda3/Lib/site-packages/numpy/core/include"
)

link_directories("C:/Users/Dusan/AppData/Local/Programs/Python/Python313/libs")

target_link_libraries(NavigationApp
  PRIVATE
  Qt6::Widgets
  "C:/Users/Dusan/AppData/Local/Programs/Python/Python313/libs/python313.lib"
)
