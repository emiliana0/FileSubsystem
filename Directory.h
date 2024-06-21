#pragma once
#include "Node.h"
class Directory : public Node {
public:
    map<string, shared_ptr<Node>> children;

    Directory(string name);
    bool isDirectory() const override;

    void add(shared_ptr<Node> node);

    void remove(const string& name);

    shared_ptr<Node> get(const string& name);

    void printDetails() const override;
};
