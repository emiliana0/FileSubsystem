#include "Terminal.h"

void Terminal::run() {
    char command[72];
    while (true) {
        fs.pwd();
        std::cout << "# ";
        std::cin.getline(command, 72);
        std::stringstream iss(command);
        char cmd[32];
        iss >> cmd;
        if (strcmp(cmd, "exit") == 0) {
            //fs.exit();
            return;
        }
        else if (strcmp(cmd, "ls") == 0) {
            char path[32];
            iss >> path;
            if (strcmp(path, "") == 0)
                fs.ls();
            else
                fs.ls(path);
        }
        else if (strcmp(cmd, "cd") == 0) {
            char path[32];
            iss >> path;
            fs.cd(path);
        }
        else if (strcmp(cmd, "pwd") == 0) {
            fs.pwd();
            std::cout << std::endl;
        }
        else if (strcmp(cmd, "mkdir") == 0) {
            char name[32];
            iss >> name;
            fs.mkdir(name);
        }
        else if (strcmp(cmd, "touch") == 0) {
            char name[32];
            iss >> name;
            fs.touch(name);
        }
        else if (strcmp(cmd, "rm") == 0) {
            char path[32];
            iss >> path;
            fs.rm(path);
        }
        else if (strcmp(cmd, "rmdir") == 0) {
            char path[32];
            iss >> path;
            fs.rmdir(path);
        }
        else if (strcmp(cmd, "exec") == 0) {
            char name[32];
            iss >> name;
            fs.exec(name);
        }
        else if (strcmp(cmd, "find") == 0) {
            char path[32], searchString[32];
            iss >> path;
            iss >> searchString;
            fs.find(path, searchString);
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
                fs.echo(text);
            }

            else if (strcmp(arg1, ">") == 0) {
                char filename[32];
                iss >> filename;
                fs.echoSaveToFile(arg1, filename);
            }
            
            else if (strcmp(arg1, ">>") == 0) {
                char filename[32];
                iss >> filename;
                fs.echoAppendToFile(arg1, filename);
            }
            else
                throw std::exception("Invalid input");
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }
}