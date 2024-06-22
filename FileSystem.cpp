#include "FileSystem.h"

FileSystem::FileSystem() {
    root = make_shared<Directory>("root");
    currentDirectory = root;
}

shared_ptr<Directory> FileSystem::getRoot() const {
    return root;
}

shared_ptr<Directory> FileSystem::getCurrentDirectory() const {
    return currentDirectory;
}

void FileSystem::setCurrentDirectory(shared_ptr<Directory> dir) {
    currentDirectory = dir;
}

void FileSystem::ls() const {
    for (const pair<string, shared_ptr<Node>>& entry : currentDirectory->children) {
        const string& name = entry.first;
        const shared_ptr<Node>& node = entry.second;
        cout << name << "\t" << "Creation date: " << ctime(&node->creationDate) << "\t"
            << "Modification date: " << ctime(&node->modificationDate);
    }
    cout << endl;
}

void FileSystem::ls(const string& path) {
    shared_ptr<Directory> dir = navigateToDirectory(path);
    if (dir == nullptr) {
        cout << "Invalid path: " << path << endl;
        return;
    }

    for (const pair<string, shared_ptr<Node>>& entry : dir->children) {
        const string& name = entry.first;
        const shared_ptr<Node>& node = entry.second;
        cout << name << "\t" << ctime(&node->creationDate) << "\t" << ctime(&node->modificationDate);
    }
    cout << endl;
}

shared_ptr<Directory> currentDirectory = make_shared<Directory>("root");

vector<string> FileSystem::splitPath(const string& path) {
    vector<string> components;
    stringstream ss(path);
    string item;
    while (getline(ss, item, '/')) {
        if (!item.empty()) {
            components.push_back(item);
        }
    }
    return components;
}

shared_ptr<Directory> FileSystem::navigateToDirectory(const string& path) {
    vector<string> components = splitPath(path);
    shared_ptr<Directory> currentDir = currentDirectory;
    for (const string& component : components) {
        if (component == "..") {//navigating to parent directory
           /* if (currentDir->parent != nullptr) {
                currentDir = dynamic_pointer_cast<Directory>(currentDir->parent);
            }*/
            if (!currentDir->parent.expired()) {
                currentDir = dynamic_pointer_cast<Directory>(currentDir->parent.lock());
            }
        }
        else if (currentDir->children.find(component) != currentDir->children.end() &&
            currentDir->children[component]->isDirectory()) {
            currentDir = dynamic_pointer_cast<Directory>(currentDir->children[component]);
        }
        else {
            throw "Error";
        }
    }
    return currentDir;
}

void FileSystem::mkdir(const string& dirName) {
    if (currentDirectory->children.find(dirName) != currentDirectory->children.end()) {
        cout << "Directory or file already exists: " << dirName << endl;
        return;
    }
    shared_ptr<Directory> newDir = make_shared<Directory>(dirName);
    newDir->parent = currentDirectory;
    currentDirectory->add(newDir);
}

void FileSystem::pwd() {
    vector<string> pathComponents;
    shared_ptr<Node> current = currentDirectory;

    while (current != nullptr) {
        pathComponents.push_back(current->name);
        if (!current->parent.expired()) {
            current = current->parent.lock();
        }
        else {
            current = nullptr;
        }
    }
    // Printing should be moved
    for (vector<string>::reverse_iterator i = pathComponents.rbegin(); i != pathComponents.rend(); ++i) {
        if (i != pathComponents.rbegin())
            cout << "/";
        cout << *i;
    }
    //cout << endl;
}

void FileSystem::cd(const string& path) {
    // Разделяне на пътя на компоненти
    vector<string> components = splitPath(path);
    shared_ptr<Directory> currentDir = currentDirectory;

    for (const string& component : components) {
        if (component == "..") {
            if (!currentDir->parent.expired()) {
                currentDir = dynamic_pointer_cast<Directory>(currentDir->parent.lock());
            }
        }
        else if (currentDir->children.find(component) != currentDir->children.end() &&
            currentDir->children[component]->isDirectory()) {
            currentDir = dynamic_pointer_cast<Directory>(currentDir->children[component]);
        }
        else {
            cout << "Invalid path: " << path << endl;
            return;
        }
    }
    currentDirectory = currentDir;
}
