#pragma once
#include "Node.h"
class File : public Node {
public:
    string content;

    File(string name);

    bool isDirectory() const override;
};
