#include "Directory.h"

Directory::Directory(string name) : Node(name) {}

bool Directory::isDirectory() const {
    return true;
}

void Directory::add(shared_ptr<Node> node) {
    children[node->name] = node;
    time(&modificationDate);
}

void Directory::remove(const string& name) {
    children.erase(name);
    time(&modificationDate);
}

shared_ptr<Node> Directory::get(const string& name) {
    if (children.find(name) != children.end())
        return children[name];
    return nullptr;
}

void Directory::printDetails() const{
    cout << name << "/\t" << ctime(&creationDate) << "\t" << ctime(&modificationDate);
}