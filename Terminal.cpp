#include "Terminal.h"

void Terminal::run() {
    string command;
    while (true) {
        fs.pwd();
        cout << "# ";
        getline(cin, command);
        istringstream iss(command);
        string cmd;
        iss >> cmd;
        if (cmd == "exit") {
            break;
        }
        else if (cmd == "ls") {
            string path;
            iss >> path;
            if (path.empty())
                fs.ls();
            else
                fs.ls(path);
        }
        else if (cmd == "cd") {
            string path;
            iss >> path;
            fs.cd(path);
        }
        else if (cmd == "pwd") {
            fs.pwd();
            cout << endl;
        }
        else if (cmd == "mkdir") {
            string name;
            iss >> name;
            fs.mkdir(name);
        }
        else if (cmd == "touch") {
            string name;
            iss >> name;
            // Implement touch command logic here
        }
        else if (cmd == "rm") {
            string path;
            iss >> path;
            // Implement rm command logic here
        }
        else if (cmd == "rmdir") {
            string path;
            iss >> path;
            // Implement rmdir command logic here
        }
        else if (cmd == "exec") {
            string path;
            iss >> path;
            // Implement exec command logic here
        }
        else if (cmd == "find") {
            string path, searchString;
            iss >> path >> searchString;
            // Implement find command logic here
        }
        else if (cmd == "echo") {
            // Implement echo command logic here
        }
        else {
            cout << "Unknown command.\n";
        }
    }
}