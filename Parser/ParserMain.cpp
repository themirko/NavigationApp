#include <iostream>
#include <thread>

#include "src/ObjectExtraction.cpp"

int main()
{
    const char* filePath = R"(C:\Users\Dusan\CLionProjects\NavigationApp\Parser\OSM\input.osm)";
    // OSMParser::ObjectExtraction::ways(filePath);
    std::thread thread1(&OSMParser::ObjectExtraction::nodes, filePath);
    std::thread thread2(&OSMParser::ObjectExtraction::ways, filePath);

    thread1.join();
    thread2.join();

    return 0;
}
