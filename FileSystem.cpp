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
        const MyString& name = entry.first;
        const std::shared_ptr<Node>& node = entry.second;
        std::cout << name << "\t" << "Creation date: " << ctime(&node->creationDate) << "\t"
            << "Modification date: " << ctime(&node->modificationDate);
    }
    std::cout << std::endl;
}

void FileSystem::ls(const MyString& path) const {
    std::shared_ptr<Directory> dir = navigateToDirectory(path);
    if (dir == nullptr) {
        std::cout << "Invalid path: " << path << std::endl;
        return;
    }

    for (const std::pair<MyString, std::shared_ptr<Node>>& entry : dir->children) {
        const MyString& name = entry.first;
        const std::shared_ptr<Node>& node = entry.second;
        std::cout << name << "\t" << ctime(&node->creationDate) << "\t" << ctime(&node->modificationDate);
    }
    std::cout << std::endl;
}

std::shared_ptr<Directory> currentDirectory = std::make_shared<Directory>("root");
Vector<MyString> FileSystem::splitPath(const MyString& path) const {
    Vector<MyString> components;
    std::stringstream ss(path.c_str());
    char item[32];
    while (!ss.eof()) {
        ss.getline(item, 32, '/');
        if (strcmp(item, "") != 0) {
            components.push_back(item);
        }
    }

    return components;
}

std::shared_ptr<Directory> FileSystem::navigateToDirectory(const MyString& path)  const {
    Vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir = currentDirectory;
    for (size_t i = 0; i < components.getSize(); ++i) {
        if (operator==(components[i],"..")) {
            if (!currentDir->parent.expired()) {
                currentDir = std::dynamic_pointer_cast<Directory>(currentDir->parent.lock());
            }
        }
        else if (currentDir->children.find(components[i]) != currentDir->children.end() &&
            currentDir->children[components[i]]->isDirectory()) {
            currentDir = std::dynamic_pointer_cast<Directory>(currentDir->children[components[i]]);
        }
        else {
            throw "Error";
        }
    }
    return currentDir;
}

void FileSystem::mkdir(const MyString& dirName) const {
    if (currentDirectory->children.find(dirName) != currentDirectory->children.end()) 
        throw std::exception("Directory or file already exists");

    std::shared_ptr<Directory> newDir = std::make_shared<Directory>(dirName);
    currentDirectory->add(newDir, currentDirectory);
}

void FileSystem::pwd() const {
    Vector<MyString> pathComponents;
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
    
    for (size_t i = pathComponents.getSize(); i > 0; --i) {
        if (i != pathComponents.getSize())
            std::cout << "/";
        std::cout << pathComponents[i - 1];
    }
}

void FileSystem::cd(const MyString& path) {
    Vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir;

    if (path.c_str()[0] == '/') {
        currentDir = root;  
    }
    else {
        currentDir = currentDirectory;  
    }     

    for (size_t i = 0; i < components.getSize(); ++i) {
        if (components[i] == "..") {
            if (!currentDir->parent.expired()) {
                currentDir = std::dynamic_pointer_cast<Directory>(currentDir->parent.lock());
            }
        }
        else if (currentDir->children.find(components[i]) != currentDir->children.end() &&
            currentDir->children[components[i]]->isDirectory()) {
            currentDir = std::dynamic_pointer_cast<Directory>(currentDir->children[components[i]]);
        }
        else 
            throw std::invalid_argument("Invalid path");
    }
    currentDirectory = currentDir;
}

void FileSystem::touch(const MyString& name) const {
    std::shared_ptr<Directory> currentDir = currentDirectory;
    std::map<MyString, std::shared_ptr<Node>>::iterator it = currentDir->children.find(name);
    if (it != currentDir->children.end()) { 
        std::shared_ptr<File> existingFile = std::dynamic_pointer_cast<File>(it->second);
        std::time(&existingFile->modificationDate);
    }
    else {
        std::shared_ptr<File> newFile = std::make_shared<File>(name);
        currentDir->add(newFile, currentDir);
    }
}

void FileSystem::echo(const MyString& text) const {
    std::cout << text << std::endl;
}
void FileSystem::echoSaveToFile(const MyString& text, const MyString& filename) const {
    std::shared_ptr<Directory> targetDir = currentDirectory;

    if (targetDir->children.find(filename) == targetDir->children.end()) {
        touch(filename);
    }

    std::shared_ptr<File> targetFile = std::dynamic_pointer_cast<File>(targetDir->children[filename]);
    if (targetFile) {
        targetFile->content = text;
    }
    else
        throw std::exception("File not valid");
}

void FileSystem::echoAppendToFile(const MyString& text, const MyString& filename) const {
    std::shared_ptr<Directory> targetDir = currentDirectory;

    if (targetDir->children.find(filename) == targetDir->children.end()) {
        touch(filename);
    }

    std::shared_ptr<File> targetFile = std::dynamic_pointer_cast<File>(targetDir->children[filename]);
    if (targetFile) {
        targetFile->content += text;  
    }
    else {
        std::cout << "Error: " << filename << " file not found." << std::endl;
    }
}

void FileSystem::rm(const MyString& path) const {
    std::shared_ptr<Node> file = navigateToNode(path);
    if (file == nullptr || file->isDirectory()) {
        std::cout << "File not found or path is a directory: " << path << std::endl;
        return;
    }
    std::shared_ptr<Directory> parentDir = std::dynamic_pointer_cast<Directory>(file->parent.lock());
    if (parentDir != nullptr) {
        parentDir->remove(file->name);
    }
    else {
        throw std::exception("Failed to find parent directory");
    }
}

void FileSystem::rmdir(const MyString& path) {
    std::shared_ptr<Node> node = navigateToNode(path);
    if (node == nullptr || !node->isDirectory()) {
        throw std::invalid_argument("Invalid directory path");
    }

    std::shared_ptr<Directory> dir = std::dynamic_pointer_cast<Directory>(node);
    if (!dir->children.empty()) {
        throw std::exception("Directory is not empty and will not be deleted");
    }

    std::shared_ptr<Directory> parentDir = std::dynamic_pointer_cast<Directory>(dir->parent.lock());
    if (parentDir != nullptr) {
        parentDir->remove(dir->name);
    }
}

std::shared_ptr<Node> FileSystem::navigateToNode(const MyString& path) const {
    Vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir = currentDirectory;
    std::shared_ptr<Node> currentNode = currentDir;
    for (size_t i = 0; i < components.getSize(); ++i) {
        if (components[i] == "..") {
            if (!currentDir->parent.expired()) {
                currentNode = currentDir->parent.lock();
                currentDir = std::dynamic_pointer_cast<Directory>(currentNode);
            }
        }
        else if (currentDir->children.find(components[i]) != currentDir->children.end()) {
            currentNode = currentDir->children[components[i]];
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
bool ends_with(const MyString& fullString, const MyString& ending) {
    if (fullString.getSize() >= ending.getSize()) {
        return fullString.substr(fullString.getSize() - ending.getSize(), ending.getSize()) == ending;
    }
    else {
        return false;
    }
}

/*void FileSystem::exit() {
    saveToFile("filesystem_state.txt"); // ????????? ???????? ????????? ?? ????????? ???????
    std::cout << "Exiting the file system.\n";
    // ??? ???? ?? ???????? ???????????? ?????????, ??? ? ??????????
    return; // ????????? ???????????? ? ??? ?? ??????? ??????????
}

void FileSystem::saveToFile(const MyString& filename) {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) 
        throw std::exception("Error: Unable to open file for writing.\n");

    // Write the current directory path
    file << currentDirectory->getP() << std::endl;

    // Recursively save the file system structure
    saveNodeToFile(currentDirectory, file);

    file.close();
}

void FileSystem::saveNodeToFile(std::shared_ptr<Node> node, std::ofstream& file) {
    if (node == nullptr)
        return;

    // Save the node's information
    file << node->getType() << " " << node->getName() << std::endl;

    // If it's a directory, recursively save its children
    if (node->getType() == NodeType::Directory) {
        std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(node);
        for (const auto& pair : directory->getChildren()) {
            saveNodeToFile(pair.second, file);
        }
    }
}

    void FileSystem::restoreFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for reading.\n";
        return;
    }

    // Read the current directory path
    std::string currentDirPath;
    std::getline(file, currentDirPath);

    // Restore the file system structure
    currentDirectory = restoreNodeFromFile(file, nullptr);

    file.close();

    // Set the current directory to the restored path
    cd(currentDirPath);
}

std::shared_ptr<Node> FileSystem::restoreNodeFromFile(std::ifstream& file, std::shared_ptr<Directory> parent) {
    std::string line;
    std::getline(file, line);
    if (line.empty()) {
        return nullptr;
    }

    std::stringstream ss(line);
    std::string typeStr, name;
    ss >> typeStr >> name;

    NodeType type = (typeStr == "Directory") ? NodeType::Directory : NodeType::File;
    std::shared_ptr<Node> node = (type == NodeType::Directory) ? std::make_shared<Directory>(name, parent) : std::make_shared<File>(name, parent);

    // If it's a directory, recursively restore its children
    if (type == NodeType::Directory) {
        std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(node);
        while (std::getline(file, line)) {
            std::shared_ptr<Node> child = restoreNodeFromFile(file, directory);
            if (child != nullptr) {
                directory->addChild(child);
            }
        }
    }

    return node;
}*/
void FileSystem::processCommand(const char* command) {
    std::stringstream iss(command);
    char cmd[32];
    iss >> cmd;
    if (strcmp(cmd, "exit") == 0) {
       // exit();
        return;
    }
    else if (strcmp(cmd, "ls") == 0) {
        char path[32];
        iss >> path;
        if (strcmp(path, "") == 0)
            ls();
        else
            ls(path);
    }
    else if (strcmp(cmd, "cd") == 0) {
        char path[32];
        iss >> path;
        cd(path);
    }
    else if (strcmp(cmd, "pwd") == 0) {
        pwd();
        std::cout << std::endl;
    }
    else if (strcmp(cmd, "mkdir") == 0) {
        char name[32];
        iss >> name;
        mkdir(name);
    }
    else if (strcmp(cmd, "touch") == 0) {
        char name[32];
        iss >> name;
        touch(name);
    }
    else if (strcmp(cmd, "rm") == 0) {
        char path[32];
        iss >> path;
        rm(path);
    }
    else if (strcmp(cmd, "rmdir") == 0) {
        char path[32];
        iss >> path;
        rmdir(path);
    }
    else if (strcmp(cmd, "exec") == 0) {
        char name[32];
        iss >> name;
        exec(name);
    }
    else if (strcmp(cmd, "find") == 0) {
        char path[32], searchString[32];
        iss >> path;
        iss >> searchString;
        find(path, searchString);
    }
    else if (strcmp(cmd, "echo") == 0) {
        char arg1[32];
        MyString text;

        while (strcmp(arg1, "<") != 0 && strcmp(arg1, "<<") != 0 && !iss.eof()) {
            iss >> arg1;
            text += arg1;
            text += " ";

        }

        iss >> arg1;

        if (strlen(arg1) == 0) {
            echo(text);
        }

        else if (strcmp(arg1, ">") == 0) {
            char filename[32];
            iss >> filename;
            echoSaveToFile(arg1, filename);
        }
        
        else if (strcmp(arg1, ">>") == 0) {
            char filename[32];
            iss >> filename;
            echoAppendToFile(arg1, filename);
        }
        else
            throw std::exception("Invalid input");
    }
    else {
        std::cout << "Unknown command.\n";
    }
}
void FileSystem::executeScript(const std::shared_ptr<File>& file) {
    std::stringstream ss(file->content.c_str());
    char command[72];
    while (ss.getline(command,72)) {
        if (!strcmp(command, "")) {
            processCommand(command);
        }
    }
}
void FileSystem::exec(const MyString& filePath) {
    std::shared_ptr<Node> node = navigateToNode(filePath);
    if (node == nullptr || node->isDirectory()) {
        throw std::invalid_argument("File not found or path is a directory: ");
    }
    std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(node);
    if (ends_with(file->name, ".txt")) {
        operator<<(std::cout, file->content);
        std::cout << std::endl;
    }
    else if (ends_with(file->name, ".sh")) {
        executeScript(file);
    }
    else if (ends_with(file->name, ".lnk")) {
        MyString linkPath = file->content.c_str();
        exec(linkPath);
    }
    else {
        std::cout << "Unknown file type: " << filePath << std::endl;
    }
}

void FileSystem::find(const MyString& startPath, const MyString& searchString) const {
    std::shared_ptr<Directory> startDir = navigateToDirectory(startPath);
    if (startDir == nullptr) {
        std::cout << "Invalid start path: " << startPath << std::endl;
        return;
    }
    findRecursive(startDir, searchString, startPath);
}

bool containsSubstring(const MyString& str, const MyString& substr) {
    for (size_t i = 0; i <= str.getSize() - substr.getSize(); ++i) {
        if (operator==(str.substr(i, substr.getSize()),substr)) 
            return true;
    }
    return false;
}

void FileSystem::findRecursive(std::shared_ptr<Directory> dir, const MyString& searchString, const MyString& currentPath) const {
    for (const std::pair<const MyString, std::shared_ptr<Node>>& entry : dir->children) {
        const MyString& name = entry.first;
        std::shared_ptr<Node> node = entry.second;
        MyString fullPath = currentPath + "/" + name;
        if (containsSubstring(name, searchString)) {
            std::cout << fullPath << std::endl;
        }
        if (node->isDirectory()) {
            findRecursive(std::dynamic_pointer_cast<Directory>(node), searchString, fullPath);
        }
    }
}
/*void exitApplication() {
    saveFileSystem();
    saveCurrentDirectory();
    std::exit(0);
}


void saveFileSystem() {
    // ????????????? ?? ?????????????? ?? ????????? ???????
}

void saveCurrentDirectory() {
    // ??????? ???????? ??????? ??????????
}*/