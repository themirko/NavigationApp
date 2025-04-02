#include "../include/ObjectExtraction.hpp"
#include "ParserHandler.cpp"

#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>


namespace OSMParser
{
    void ObjectExtraction::nodes(const char* filePath) {
        osmium::io::Reader reader{filePath, osmium::osm_entity_bits::node};

        Handler parserHandler;
        osmium::apply(reader, parserHandler);


        reader.close();
    }

    void ObjectExtraction::ways(const char* filePath) {
        osmium::io::Reader reader{filePath, osmium::osm_entity_bits::way};

        Handler parserHandler;
        osmium::apply(reader, parserHandler);

        reader.close();
    }
}

