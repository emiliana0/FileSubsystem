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
    void ls(const MyString& path);
    void mkdir(const MyString& dirName);
    void pwd();
    void cd(const MyString& path);
    void touch(const MyString& fileName);

    void rm(const MyString& path);
    void rmdir(const MyString& path);
    void exec(const MyString& path);
    void find(const MyString& path, const MyString& searchString);
    void echo(const MyString& text);
    void echoSaveToFile(const MyString& text, const MyString& filename);
    void echoAppendToFile(const MyString& text, const MyString& filename);
    //void exit();
    void processCommand(const char* command);
private:
    Vector<MyString> splitPath(const MyString& path);
    std::shared_ptr<Directory> navigateToDirectory(const MyString& path);
    std::shared_ptr<Node> navigateToNode(const MyString& path);
    //void saveToFile(const MyString& filename);
    void findRecursive(std::shared_ptr<Directory>, const MyString&, const MyString&);
    void executeScript(const std::shared_ptr<File>& file);
};