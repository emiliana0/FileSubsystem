#include "File.h"

File::File(const MyString& name) : Node(name) {}

bool File::isDirectory() const {
    return false;
}