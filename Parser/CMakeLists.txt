cmake_minimum_required(VERSION 3.10)
project(Parser)

set( CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Specify the Vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Add the executable
add_executable(ParserMain ParserMain.cpp)


# Include libosmium headers
target_include_directories(ParserMain PUBLIC
        C:/OSMlib/include/
)

# Find and link Boost libraries
find_package(Boost REQUIRED COMPONENTS system filesystem)
target_link_libraries(ParserMain PUBLIC Boost::boost Boost::system Boost::filesystem)

# Find and link zlib and expat (required by libosmium for some features)
find_package(ZLIB REQUIRED)
target_link_libraries(ParserMain PUBLIC ZLIB::ZLIB)

find_package(EXPAT REQUIRED)
target_link_libraries(ParserMain PUBLIC EXPAT::EXPAT)

find_package(BZip2 REQUIRED)
target_link_libraries(ParserMain PUBLIC BZip2::BZip2)

