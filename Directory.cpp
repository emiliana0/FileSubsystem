#include "Directory.h"

Directory::Directory(const MyString& name) : Node(name) {}

bool Directory::isDirectory() const {
    return true;
}

void Directory::add(std::shared_ptr<Node> node, const std::shared_ptr<Directory>& parent) {
    children[node->name] = node;
    time(&modificationDate);
    node->parent = parent;

}

void Directory::remove(const MyString& name) {
    children.erase(name);
    //time(&modificationDate);

}

std::shared_ptr<Node> Directory::get(const MyString& name) {
    if (children.find(name) != children.end())
        return children[name];
    return nullptr;
}

void Directory::printDetails() const{
    std::cout << name << "/\t" << ctime(&creationDate) << "\t" << ctime(&modificationDate);
}