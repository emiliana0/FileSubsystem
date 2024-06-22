#pragma once
#include "Directory.h"

class FileSystem {
private:
    shared_ptr<Directory> root;
    shared_ptr<Directory> currentDirectory;

public:
    FileSystem();

    shared_ptr<Directory> getRoot() const;

    shared_ptr<Directory> getCurrentDirectory() const;

    void setCurrentDirectory(shared_ptr<Directory> dir);

    vector<string> splitPath(const string& path);
    shared_ptr<Directory> navigateToDirectory(const string& path);

    void ls() const;
    void ls(const string& path);
    void mkdir(const string& dirName);
    void pwd();
    void cd(const string& path);
    // Implement other commands (touch, rm, rmdir, exec, find, echo) here
};