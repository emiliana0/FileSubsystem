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
Vector<MyString> FileSystem::splitPath(const MyString& path) {
    Vector<MyString> components;
    std::stringstream ss(path.c_str());
    char item[32];
    while (!ss.eof()) {
        ss.getline(item, 32, '/');
        if (strcmp(item, "") != 0) {
            components.push_back(item);//
        }
    }

    return components;
}

std::shared_ptr<Directory> FileSystem::navigateToDirectory(const MyString& path) {
    Vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir = currentDirectory;
    for (size_t i = 0; i < components.getSize(); ++i) {
        if (components[i] == "..") {//navigating to parent directory
            /* if (currentDir->parent != nullptr) {
                 currentDir = dynamic_pointer_cast<Directory>(currentDir->parent);
             }*/
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

void FileSystem::mkdir(const MyString& dirName) {
    if (currentDirectory->children.find(dirName) != currentDirectory->children.end()) {
        std::cout << "Directory or file already exists: " << dirName << std::endl;
        return;
    }
    std::shared_ptr<Directory> newDir = std::make_shared<Directory>(dirName);
    //newDir->parent = currentDirectory;
    currentDirectory->add(newDir, currentDirectory);
}

void FileSystem::pwd() {
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
    // Printing should be moved
    for (size_t i = pathComponents.getSize(); i > 0; --i) {
        if (i != pathComponents.getSize())
            std::cout << "/";
        std::cout << pathComponents[i - 1];
    }
    //cout << endl;
}

void FileSystem::cd(const MyString& path) {
    Vector<MyString> components = splitPath(path);
    std::shared_ptr<Directory> currentDir;

    if (path.c_str()[0] == '/') {
        currentDir = root;  // Абсолютен път - започваме от кореновата директория
    }
    else {
        currentDir = currentDirectory;  // Относителен път - започваме от текущата директория
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
        else {
            std::cout << "Invalid path: " << path << std::endl;
            return;
        }
    }
    currentDirectory = currentDir;
}

void FileSystem::touch(const MyString& name) {
    std::shared_ptr<Directory> currentDir = currentDirectory;
    std::map<MyString, std::shared_ptr<Node>>::iterator it = currentDir->children.find(name);
    if (it != currentDir->children.end()) { // If the file already exist, we will change the modification date
        std::shared_ptr<File> existingFile = std::dynamic_pointer_cast<File>(it->second);
        std::time(&existingFile->modificationDate);
    }
    else {
        std::shared_ptr<File> newFile = std::make_shared<File>(name);
        currentDir->add(newFile, currentDir);
    }
}

void FileSystem::echo(const MyString& text) {
    std::cout << text << std::endl;
}
void FileSystem::echoSaveToFile(const MyString& text, const MyString& filename) {
    std::shared_ptr<Directory> targetDir = currentDirectory;

    if (targetDir->children.find(filename) == targetDir->children.end()) {
        touch(filename);
    }

    std::shared_ptr<File> targetFile = std::dynamic_pointer_cast<File>(targetDir->children[filename]);
    if (targetFile) {
        targetFile->content = text;
    }
    else {
        std::cout << "Error: " << filename << " is not a regular file." << std::endl;
    }
}

void FileSystem::echoAppendToFile(const MyString& text, const MyString& filename) {
    std::shared_ptr<Directory> targetDir = currentDirectory;

    if (targetDir->children.find(filename) == targetDir->children.end()) {
        touch(filename);
    }

    std::shared_ptr<File> targetFile = std::dynamic_pointer_cast<File>(targetDir->children[filename]);
    if (targetFile) {
        targetFile->content += text;  // Добавяме текста към съдържанието на файла
    }
    else {
        std::cout << "Error: " << filename << " file not found." << std::endl;
    }
}

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
    //std::shared_ptr<Directory> parentDir = file->parent.lock();
    std::shared_ptr<Directory> parentDir = std::dynamic_pointer_cast<Directory>(file->parent.lock());
    if (parentDir != nullptr) {
        parentDir->remove(file->name);
    }
    else {
        std::cout << "Failed to find parent directory for: " << path << std::endl;
    }
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
    saveToFile("filesystem_state.txt"); // Запазваме текущото състояние на файловата система
    std::cout << "Exiting the file system.\n";
    // Тук може да изведете допълнително съобщение, ако е необходимо
    return; // Затваряме приложението с код за успешно завършване
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

Метод за възстановяване от файл(FileSystem::restoreFromFile) :

    cpp

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
/*void FileSystem::exec(const MyString& filePath) {
    std::shared_ptr<Node> node = navigateToNode(filePath);
    if (node == nullptr || node->isDirectory()) {
        std::cout << "File not found or path is a directory: " << filePath << std::endl;
        return;
    }
    std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(node);
    if (ends_with(file->name, ".txt")) {
        std::cout << file->content << std::endl;
    }
    else if (ends_with(file->name, ".sh")) {
        std::stringstream ss(file->content.c_str());
        char command[32];
        while (std::getline(ss, command)) {
            processCommand(command);
        }
    }
    else if (ends_with(file->name, ".lnk")) {
        MyString linkPath = file->content.c_str();
        exec(linkPath);
    }
    else {
        std::cout << "Unknown file type: " << filePath << std::endl;
    }
}*/
/*void FileSystem::processCommand(const MyString& command) {
    // ??? ?????????????? ???????? ?? ?????????? ?? ????????? ?? ??????????? ???????
}*/

/*void FileSystem::find(const std::string& startPath, const std::string& searchString) {
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
    // Имплементация на сериализацията на файловата система
}

void saveCurrentDirectory() {
    // Записва текущата работна директория
}*/