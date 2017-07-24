#pragma once

#include "DiskSystem.hpp"
#include "File.hpp"

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <unordered_map>

void commandDir(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandMkdir(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandRd(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandCd(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandDel(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandCopy(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandRen(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandMove(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandMklink(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandSave(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandLoad(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandCls(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandExit(std::vector<std::string> arguments, std::vector<std::string> targets);
void commandError(std::vector<std::string> arguments, std::vector<std::string> targets);

// IDEA: inefficient hash table
class Command {
public:
    /**
     * empty constructor
     */
    Command();

    /**
     * get command function from hash table
     */
     std::function<void(std::vector<std::string>, std::vector<std::string>)> getExecutor(const std::string& command);

private:
    std::unordered_map<std::string, std::function<void(std::vector<std::string>, std::vector<std::string>)>> m_commandTable;

};

Command::Command() {
    m_commandTable.emplace(std::string("dir"), &commandDir);
    m_commandTable.emplace(std::string("mkdir"), &commandMkdir);
    m_commandTable.emplace(std::string("rd"), &commandRd);
    m_commandTable.emplace(std::string("cd"), &commandCd);
    m_commandTable.emplace(std::string("del"), &commandDel);
    m_commandTable.emplace(std::string("copy"), &commandCopy);
    m_commandTable.emplace(std::string("ren"), &commandRen);
    m_commandTable.emplace(std::string("move"), &commandMove);
    m_commandTable.emplace(std::string("mklink"), &commandMklink);
    m_commandTable.emplace(std::string("save"), &commandSave);
    m_commandTable.emplace(std::string("load"), &commandLoad);
    m_commandTable.emplace(std::string("cls"), &commandCls);
}

std::function<void(std::vector<std::string>, std::vector<std::string>)> Command::getExecutor(const std::string& command) {
    // fault tolerence: error command
    auto it = m_commandTable.find(command);
    if(it == m_commandTable.end()) {
        return &commandError;
    }
    return it->second;
}

void commandDir(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command dir" << std::endl;
    // std::cout << "dir " << arguments << " " << targets << std::endl;
}

void commandMkdir(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command mkdir" << std::endl;
    // make diectory
}

void commandRd(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command rd" << std::endl;
    // std::cout << "rd " << arguments << " " << targets << std::endl;
}

void commandCd(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command cd" << std::endl;
    // std::cout << "cd " << arguments << " " << targets << std::endl;
}

void commandDel(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command del" << std::endl;
    // std::cout << "del " << arguments << " " << targets << std::endl;
}

void commandCopy(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command copy" << std::endl;
    // std::cout << "copy " << arguments << " " << targets << std::endl;
}

void commandRen(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command ren" << std::endl;
    // std::cout << "ren " << arguments << " " << targets << std::endl;
}

void commandMove(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command move" << std::endl;
    // std::cout << "move " << arguments << " " << targets << std::endl;
}

void commandMklink(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command mklink" << std::endl;
    // std::cout << "mklink " << arguments << " " << targets << std::endl;
}

void commandSave(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command save" << std::endl;
    // std::cout << "save " << arguments << " " << targets << std::endl;
}

void commandLoad(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command load" << std::endl;
    // std::cout << "load " << arguments << " " << std::endl;
}

void commandCls(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command cls" << std::endl;
    system("cls");
}

void commandExit(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command exit" << std::endl;
}

void commandError(std::vector<std::string> arguments, std::vector<std::string> targets) {
    std::cout << "execute command error" << std::endl;
}
