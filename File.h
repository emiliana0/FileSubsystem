#pragma once
#include "Node.h"

class File : public Node {
public:
    MyString content;

    File(const MyString& name);

    bool isDirectory() const override;
};
