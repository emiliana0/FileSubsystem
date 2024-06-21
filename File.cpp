#include "File.h"

File::File(string name) : Node(name) {}

bool File::isDirectory() const {
    return false;
}