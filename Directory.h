#pragma once
#include "Node.h"
class Directory : public Node {
public:
    std::map<MyString, std::shared_ptr<Node>> children;
   
    Directory(const MyString& name);
    bool isDirectory() const override;

    void add(std::shared_ptr<Node> node, const std::shared_ptr<Directory>& parent);

    void remove(const MyString& name);

    std::shared_ptr<Node> get(const MyString& name);

    void printDetails() const override;
};
