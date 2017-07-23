#pragma once

#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>

void commandDir(std::string arguments, std::string targets);
void commandMkdir(std::string arguments, std::string targets);
void commandRd(std::string arguments, std::string targets);
void commandCd(std::string arguments, std::string targets);
void commandDel(std::string arguments, std::string targets);
void commandCopy(std::string arguments, std::string targets);
void commandRen(std::string arguments, std::string targets);
void commandMove(std::string arguments, std::string targets);
void commandMklink(std::string arguments, std::string targets);
void commandSave(std::string arguments, std::string targets);
void commandLoad(std::string arguments, std::string targets);
void commandCls(std::string arguments, std::string targets);

// IDEA: inefficient hash table
class Command {
public:
    Command();
    std::function<void(std::string, std::string)> getExecutor(const std::string& command);
private:
    std::unordered_map<std::string, std::function<void(std::string, std::string)>> commandTable;
};

Command::Command() {
    commandTable.emplace(std::string("dir"), &commandDir);
    commandTable.emplace(std::string("mkdir"), &commandMkdir);
    commandTable.emplace(std::string("rd"), &commandRd);
    commandTable.emplace(std::string("cd"), &commandCd);
    commandTable.emplace(std::string("del"), &commandDel);
    commandTable.emplace(std::string("copy"), &commandCopy);
    commandTable.emplace(std::string("ren"), &commandRen);
    commandTable.emplace(std::string("move"), &commandMove);
    commandTable.emplace(std::string("mklink"), &commandMklink);
    commandTable.emplace(std::string("save"), &commandSave);
    commandTable.emplace(std::string("load"), &commandLoad);
    commandTable.emplace(std::string("cls"), &commandCls);
}

std::function<void(std::string, std::string)> Command::getExecutor(const std::string& command) {
    return commandTable[command];
}

void commandDir(std::string arguments, std::string targets) {
    std::cout << "execute command dir" << std::endl;
    std::cout << "dir " << arguments << " " << targets << std::endl;
}

void commandMkdir(std::string arguments, std::string targets) {
    std::cout << "execute command mkdir" << std::endl;
    std::cout << "mkdir " << arguments << " " << targets << std::endl;
}

void commandRd(std::string arguments, std::string targets) {
    std::cout << "execute command rd" << std::endl;
    std::cout << "rd " << arguments << " " << targets << std::endl;
}

void commandCd(std::string arguments, std::string targets) {
    std::cout << "execute command cd" << std::endl;
    std::cout << "cd " << arguments << " " << targets << std::endl;
}

void commandDel(std::string arguments, std::string targets) {
    std::cout << "execute command del" << std::endl;
    std::cout << "del " << arguments << " " << targets << std::endl;
}

void commandCopy(std::string arguments, std::string targets) {
    std::cout << "execute command copy" << std::endl;
    std::cout << "copy " << arguments << " " << targets << std::endl;
}

void commandRen(std::string arguments, std::string targets) {
    std::cout << "execute command ren" << std::endl;
    std::cout << "ren " << arguments << " " << targets << std::endl;
}

void commandMove(std::string arguments, std::string targets) {
    std::cout << "execute command move" << std::endl;
    std::cout << "move " << arguments << " " << targets << std::endl;
}

void commandMklink(std::string arguments, std::string targets) {
    std::cout << "execute command mklink" << std::endl;
    std::cout << "mklink " << arguments << " " << targets << std::endl;
}

void commandSave(std::string arguments, std::string targets) {
    std::cout << "execute command save" << std::endl;
    std::cout << "save " << arguments << " " << targets << std::endl;
}

void commandLoad(std::string arguments, std::string targets) {
    std::cout << "execute command load" << std::endl;
    std::cout << "load " << arguments << " " << std::endl;
}

void commandCls(std::string arguments, std::string targets) {
    std::cout << "execute command cls" << std::endl;
    std::cout << "cls " << arguments << " " << targets << std::endl;
}
