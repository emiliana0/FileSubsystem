#include "Node.h"

Node::Node(MyString name) : name(name) {
    time(&creationDate);
    time(&modificationDate);
}

void Node::printDetails() const {
    std::cout << name << "\t" << ctime(&creationDate) << "\t" << ctime(&modificationDate);
}