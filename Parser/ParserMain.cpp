#include "include/Utilities.hpp"
#include "include/ObjectExtraction.hpp"
#include "include/ParserHandler.hpp"

#include <iostream>
#include <thread>

int main()
{
    const char* filePath = R"(C:\Users\Dusan\CLionProjects\NavigationApp\Parser\OSM\input.osm)";
    std::thread thread1(&OSMParser::ObjectExtraction::nodes, filePath);
    std::thread thread2(&OSMParser::ObjectExtraction::ways, filePath);

    thread1.join();
    thread2.join();

    return 0;
}
