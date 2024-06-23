#include "FileSystem.h"

FileSystem::FileSystem() {
    root = std::make_shared<Directory>("root");
    currentDirectory = root;
}

std::shared_ptr<Directory> FileSystem::getRoot() const {
    return root;
}

std::shared_ptr<Directory> FileSystem::getCurrentDirectory() const {
    return currentDirectory;
}

void FileSystem::setCurrentDirectory(std::shared_ptr<Directory> dir) {
    currentDirectory = dir;
}

void FileSystem::ls() const {
    for (const std::pair<MyString, std::shared_ptr<Node>>& entry : currentDirectory->children) {
        const MyString& name = entry.first;//
        const std::shared_ptr<Node>& node = entry.second;
        std::cout << name << "\t" << "Creation date: " << ctime(&node->creationDate) << "\t"
            << "Modification date: " << ctime(&node->modificationDate);
    }
    std::cout << std::endl;
}

void FileSystem::ls(const MyString& path) {
    std::shared_ptr<Directory> dir = navigateToDirectory(path);
    if (dir == nullptr) {
        std::cout << "Invalid path: " << path << std::endl;
        return;
    }

    for (const std::pair<MyString, std::shared_ptr<Node>>& entry : dir->children) {
        const MyString& name = entry.first;//
        const std::shared_ptr<Node>& node = entry.second;
        std::cout << name << "\t" << ctime(&node->creationDate) << "\t" << ctime(&node->modificationDate);
    }
    std::cout << std::endl;
}

std::shared_ptr<Directory> currentDirectory = std::make_shared<Directory>("root");
std::vector<MyString> FileSystem::splitPath(const MyString& path) {
    std::vector<MyString> components;
    std::stringstream ss(path.c_str());
    char item[32];
    while (!ss.eof()) {
        ss.getline(item, 32, '/');
        if (strcmp(item,"") != 0) {
            components.push_back(item);//
        }
    }

    return components;
}

std::shared_ptr<Directory> FileSystem::navigateToDirectory(const MyString& path) {
    std::vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir = currentDirectory;
    for (const MyString& component : components) {
        if (component == "..") {//navigating to parent directory
           /* if (currentDir->parent != nullptr) {
                currentDir = dynamic_pointer_cast<Directory>(currentDir->parent);
            }*/
            if (!currentDir->parent.expired()) {
                currentDir = std::dynamic_pointer_cast<Directory>(currentDir->parent.lock());
            }
        }
        else if (currentDir->children.find(component) != currentDir->children.end() &&
            currentDir->children[component]->isDirectory()) {
            currentDir = std::dynamic_pointer_cast<Directory>(currentDir->children[component]);
        }
        else {
            throw "Error";
        }
    }
    return currentDir;
}

void FileSystem::mkdir(const MyString& dirName) {
    if (currentDirectory->children.find(dirName) != currentDirectory->children.end()) {
        std::cout << "Directory or file already exists: " << dirName << std::endl;
        return;
    }
    std::shared_ptr<Directory> newDir = std::make_shared<Directory>(dirName);
    newDir->parent = currentDirectory;
    currentDirectory->add(newDir);
}

void FileSystem::pwd() {
    std::vector<MyString> pathComponents;
    std::shared_ptr<Node> current = currentDirectory;

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
    for (std::vector<MyString>::reverse_iterator i = pathComponents.rbegin(); i != pathComponents.rend(); ++i) {
        if (i != pathComponents.rbegin())
            std::cout << "/";
        std::cout << *i;
    }
    //cout << endl;
}

void FileSystem::cd(const MyString& path) {
    // ��������� �� ���� �� ����������
    std::vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir = currentDirectory;

    for (const MyString& component : components) {
        if (component == "..") {
            if (!currentDir->parent.expired()) {
                currentDir = std::dynamic_pointer_cast<Directory>(currentDir->parent.lock());
            }
        }
        else if (currentDir->children.find(component) != currentDir->children.end() &&
            currentDir->children[component]->isDirectory()) {
            currentDir = std::dynamic_pointer_cast<Directory>(currentDir->children[component]);
        }
        else {
            std::cout << "Invalid path: " << path << std::endl;
            return;
        }
    }
    currentDirectory = currentDir;
}

/*void FileSystem::touch(const MyString& name) {
    if (currentDirectory->children.find(name) != currentDirectory->children.end()) {
        currentDirectory->children[name]->modificationDate = time(0);
    }
    else {
        shared_ptr<Node> newNode;
        if (name.find(".sh") != MyString::npos) {
            newNode = make_shared<File>(name);
        }
        else if (name.find(".lnk") != MyString::npos) {
            newNode = make_shared<File>(name);
        }
        else {
            newNode = make_shared<File>(name);
        }
        currentDirectory->add(newNode);
    }
}*/

void FileSystem::rm(const MyString& path) {
    /*shared_ptr<Node> node = navigateToNode(path);
    if (node == nullptr) {
        cout << "Invalid path: " << path << endl;
        return;
    }

    if (node->isDirectory()) {
        cout << "Cannot remove directory with rm. Use rmdir instead." << endl;
        return;
    }

    shared_ptr<Directory> parentDir = dynamic_pointer_cast<Directory>(node->parent.lock());
    if (parentDir != nullptr) {
        parentDir->remove(node->name);
    }*/
    std::shared_ptr<Node> file = navigateToNode(path);
    if (file == nullptr || file->isDirectory()) {
        std::cout << "File not found or path is a directory: " << path << std::endl;
        return;
    }
    /*std::shared_ptr<Directory> parentDir = file->parent.lock();
    if (parentDir != nullptr) {
        parentDir->remove(file->name);
    }*/
}

void FileSystem::rmdir(const MyString& path) {
    std::shared_ptr<Node> node = navigateToNode(path);
    if (node == nullptr || !node->isDirectory()) {
        std::cout << "Invalid directory path: " << path << std::endl;
        return;
    }

    std::shared_ptr<Directory> dir = std::dynamic_pointer_cast<Directory>(node);
    if (!dir->children.empty()) {
        std::cout << "Directory is not empty: " << path << std::endl;
        return;
    }

    std::shared_ptr<Directory> parentDir = std::dynamic_pointer_cast<Directory>(dir->parent.lock());
    if (parentDir != nullptr) {
        parentDir->remove(dir->name);
    }
}

std::shared_ptr<Node> FileSystem::navigateToNode(const MyString& path) {
    std::vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir = currentDirectory;
    std::shared_ptr<Node> currentNode = currentDir;
    for (std::vector<MyString>::iterator it = components.begin(); it != components.end(); ++it) {
        const MyString& component = *it;
        if (component == "..") {
            if (!currentDir->parent.expired()) {
                currentNode = currentDir->parent.lock();
                currentDir = std::dynamic_pointer_cast<Directory>(currentNode);
            }
        }
        else if (currentDir->children.find(component) != currentDir->children.end()) {
            currentNode = currentDir->children[component];
            if (currentNode->isDirectory()) {
                currentDir = std::dynamic_pointer_cast<Directory>(currentNode);
            }
        }
        else {
            return nullptr;
        }
    }
    return currentNode;
}

/*void FileSystem::exec(const std::string& filePath) {
    std::shared_ptr<Node> node = navigateToNode(filePath);
    if (node == nullptr || node->isDirectory()) {
        std::cout << "File not found or path is a directory: " << filePath << std::endl;
        return;
    }
    std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(node);
    if (file->name.ends_with(".txt")) {
        std::cout << file->content << std::endl;
    }
    else if (file->name.ends_with(".sh")) {
        std::stringstream ss(file->content);
        std::string command;
        while (std::getline(ss, command)) {
            processCommand(command);
        }
    }
    else if (file->name.ends_with(".lnk")) {
        std::string linkPath = file->content;
        exec(linkPath);
    }
    else {
        std::cout << "Unknown file type: " << filePath << std::endl;
    }
}
void FileSystem::processCommand(const std::string& command) {
    // ��� �������������� �������� �� ���������� �� ��������� �� ����������� �������
}

void FileSystem::find(const std::string& startPath, const std::string& searchString) {
    std::shared_ptr<Directory> startDir = navigateToDirectory(startPath);
    if (startDir == nullptr) {
        std::cout << "Invalid start path: " << startPath << std::endl;
        return;
    }
    findRecursive(startDir, searchString, startPath);
}

void FileSystem::findRecursive(std::shared_ptr<Directory> dir, const std::string& searchString, const std::string& currentPath) {
    for (const std::pair<const std::string, std::shared_ptr<Node>>& entry : dir->children) {
        const std::string& name = entry.first;
        std::shared_ptr<Node> node = entry.second;
        std::string fullPath = currentPath + "/" + name;
        if (name.find(searchString) != std::string::npos) {
            std::cout << fullPath << std::endl;
        }
        if (node->isDirectory()) {
            findRecursive(std::dynamic_pointer_cast<Directory>(node), searchString, fullPath);
        }
    }
}
void exitApplication() {
    saveFileSystem();
    saveCurrentDirectory();
    std::exit(0);
}

void saveFileSystem() {
    // ������������� �� �������������� �� ��������� �������
}

void saveCurrentDirectory() {
    // ������� �������� ������� ����������
}*/