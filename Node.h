#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <memory>
#include <sstream>
#pragma warning(disable : 4996)
using namespace std;

class Node {
public:
    string name;
    time_t creationDate;
    time_t modificationDate;
    weak_ptr<Node> parent;// Използваме weak_ptr, за да избегнем циклични зависимости

    Node(string name);

    virtual ~Node() = default;

    virtual bool isDirectory() const = 0;

    virtual void printDetails() const;
};