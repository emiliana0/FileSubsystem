#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <memory>
#include <sstream>

using namespace std;

class Node {
public:
    string name;
    time_t creationDate;
    time_t modificationDate;

    Node(string name);

    virtual ~Node() = default;

    virtual bool isDirectory() const = 0;

    virtual void printDetails() const;
};