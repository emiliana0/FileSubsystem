#pragma once
#include <iostream>
#include <map>
#include <ctime>
#include <sstream>
#include <fstream>
#pragma warning(disable : 4996)
#include "MyString.h"
#include "Vector.hpp"

class Node {
public:
    MyString name;
    time_t creationDate;
    time_t modificationDate;
    std::weak_ptr<Node> parent; // We are using weak_ptr to prevent loops

    Node(MyString name);

    virtual ~Node() = default;

    virtual bool isDirectory() const = 0;

    virtual void printDetails() const;
};