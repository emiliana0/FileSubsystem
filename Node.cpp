#include "Node.h"

Node::Node(string name) : name(name) {
    time(&creationDate);
    time(&modificationDate);
}

void Node::printDetails() const {
    cout << name << "\t" << ctime(&creationDate) << "\t" << ctime(&modificationDate);
}