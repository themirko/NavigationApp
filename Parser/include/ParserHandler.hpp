#ifndef PARSERHANDLER_HPP
#define PARSERHANDLER_HPP

#include <fstream>
#include <string_view>
#include <osmium/handler.hpp>

namespace OSMParser
{
    class Handler : public osmium::handler::Handler
    {
    private:
        std::ofstream nodeFile;
        std::ofstream wayFile;

    public:
        Handler();
        ~Handler();

        void node(const osmium::Node&);
        void way(const osmium::Way&);

        void extractHighwayElements(const osmium::Way&, const std::basic_string_view<char>);
    }; // Handler class

} // OSMParser


#endif //PARSERHANDLER_HPP
