#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <memory>
#include <sstream>
#include <fstream>
#pragma warning(disable : 4996)
#include "MyString.h"

class Node {
public:
    MyString name;
    time_t creationDate;
    time_t modificationDate;
    std::weak_ptr<Node> parent;// Използваме weak_ptr, за да избегнем циклични зависимости

    Node(MyString name);

    virtual ~Node() = default;

    virtual bool isDirectory() const = 0;

    virtual void printDetails() const;
};