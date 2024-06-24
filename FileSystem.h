#pragma once
#include "Directory.h"
#include "File.h"

class FileSystem {
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentDirectory;

public:
    FileSystem();

    std::shared_ptr<Directory> getRoot() const;

    std::shared_ptr<Directory> getCurrentDirectory() const;

    void setCurrentDirectory(std::shared_ptr<Directory> dir);

    void ls() const;
    void ls(const MyString& path) const;
    void mkdir(const MyString& dirName) const;
    void pwd() const;
    void cd(const MyString& path);
    void touch(const MyString& fileName) const;

    void rm(const MyString& path) const;
    void rmdir(const MyString& path);
    void exec(const MyString& path);
    void find(const MyString& path, const MyString& searchString) const;
    void echo(const MyString& text) const;
    void echoSaveToFile(const MyString& text, const MyString& filename) const;
    void echoAppendToFile(const MyString& text, const MyString& filename) const;
    //void exit();
    void processCommand(const char* command);
private:
    Vector<MyString> splitPath(const MyString& path) const;
    std::shared_ptr<Directory> navigateToDirectory(const MyString& path) const;
    std::shared_ptr<Node> navigateToNode(const MyString& path) const;
    //void saveToFile(const MyString& filename);
    void findRecursive(std::shared_ptr<Directory>, const MyString&, const MyString&) const;
    void executeScript(const std::shared_ptr<File>& file);
};