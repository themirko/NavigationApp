#pragma once

#include <iostream>

#include "include/NodeClass.hpp"
#include "include/EdgeClass.hpp"
#include <memory>

#include "include/types.hpp"

int main() {

    nodePtr str =  std::make_shared<Node>("123321");
    Edge edge("Street", 0.123, nullptr);
    str->addEdge(edge);


    str->printNode();

    return 0;
}