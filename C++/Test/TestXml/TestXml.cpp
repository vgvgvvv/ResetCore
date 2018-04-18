//
// Created by 董宸 on 2018/4/18.
//

#include <iostream>
#include "pugixml/src/pugixml.hpp"

void printChild(const pugi::xml_node& parent, const int layer = 0);

int main(){
    pugi::xml_document doc;
    auto result = doc.load_file("test.xml");
    std::cout << "Load result: " << result.description() << std::endl;
    printChild(doc.root());

    return 0;
}

void printChild(const pugi::xml_node& parent, const int layer){
    std::cout << std::string(layer, '-') << "type:" << parent.type() << " name:" << parent.name() << " value:" << parent.child_value() << std::endl;
    auto children = parent.children();
    if(parent.empty())
        return;
    for (auto child : children) {
        printChild(child, layer + 1);
    }
}