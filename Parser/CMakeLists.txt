cmake_minimum_required(VERSION 3.10)
project(Parser)

set( CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Specify the Vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")


add_executable(ParserMain
        ParserMain.cpp
        src/ObjectExtraction.cpp
        src/ParserHandler.cpp
)

# Include libosmium headers
target_include_directories(ParserMain PUBLIC
        C:/OSMlib/include/
)

# Find and link Boost libraries
find_package(Boost REQUIRED system filesystem)
find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)
find_package(BZip2 REQUIRED)

target_link_libraries(ParserMain PUBLIC
        Boost::system
        Boost::filesystem
        ZLIB::ZLIB
        EXPAT::EXPAT
        BZip2::BZip2
)