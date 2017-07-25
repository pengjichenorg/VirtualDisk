#pragma once

#include "DiskSystem.hpp"
#include "File.hpp"

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <unordered_map>

void commandDir(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandMkdir(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandRd(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandCd(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandDel(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandCopy(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandRen(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandMove(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandMklink(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandSave(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandLoad(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandCls(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandExit(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);
void commandError(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets);

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
     std::function<void(std::vector<std::string>, std::unordered_map<std::string, std::vector<std::string>>)> getExecutor(const std::string& command);

private:
    // typedef std::unordered_map<std::string, std::vector<std::string>> argumentsType;
    std::unordered_map<std::string, std::function<void(std::vector<std::string>, std::unordered_map<std::string, std::vector<std::string>>)>> m_commandTable;

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

std::function<void(std::vector<std::string>, std::unordered_map<std::string, std::vector<std::string>>)> Command::getExecutor(const std::string& command) {
    // fault tolerence: error command
    auto it = m_commandTable.find(command);
    if(it == m_commandTable.end()) {
        return &commandError;
    }
    return it->second;
}

void commandDir(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command dir" << std::endl;
    // std::cout << "dir " << arguments << " " << targets << std::endl;
}

void commandMkdir(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    if(targets.empty()) {
        std::cout << "error command!" << std::endl;
    }  else {
        std::cout << "mkdir [path]\\[dir]" << std::endl;
        auto currentNode = DiskSystem::getInstance()->getCurrentNode();
        for(auto targetIt = targets.begin(); targetIt != targets.end(); targetIt++) { // for each target
            std::cout << "deal target" << std::endl;
            auto path = targetIt->first;
            std::cout << "targetIt->first:" << path << std::endl;
            // TODO: deal 5 condition:
            // - mkdir folder1, path is ., use currentNode, like .\folder1
            // - mkdir \folder1, path is empty, use currentPartition's node
            // - mkdir .\folder1, path is ., use currentNode
            // - mkdir ..\folder1, path is .., use currentNode's parent or currentPartition's node
            // - mkdir folder1\folder2, path is f1, use currentNode's child or create create node
            int test = 0;
            if(path.empty()) {
                std::cout << "condition 2" << std::endl;
            } else {
                auto findPos = path.find("\\");
                while(findPos != std::string::npos) { // eat path
                    std::cout << "path:" << path << std::endl;
                    // deal this path
                    auto findPath = path.substr(0, findPos);
                    std::cout << "findPath:" << findPath << std::endl;
                    if(findPath.compare(".") == 0) { // .\
                        // use currentNode
                        std::cout << "use currentNode" << std::endl;
                    } else if(findPath.compare("..") == 0) { // ..\
                        // use currentNode's parent or currentPartition's node
                        std::cout << "use currentNode's parent or currentPartition's node" << std::endl;
                    } else { // path\
                        // use currentNode's child or create create node
                        std::cout << "use currentNode's child or create create node" << std::endl;
                    }
                    // go to next path
                    path = path.substr(findPos+1);
                    std::cout << "path:" << path << std::endl;
                    findPos = path.find("\\");
                }
                std::cout << "path:" << path << std::endl;
                std::cout << "deal condition 1-5" << std::endl;
                if(path.compare(".") == 0) { // .\
                    // use currentNode
                    std::cout << "use currentNode" << std::endl;
                } else if(path.compare("..") == 0) { // ..\
                    // use currentNode's parent or currentPartition's node
                    std::cout << "use currentNode's parent or currentPartition's node" << std::endl;
                } else { // path\
                    // use currentNode's child or create create node
                    std::cout << "use currentNode's child or create create node" << std::endl;
                }
                std::cout << "deal end" << std::endl;
            }
        }
    }
    std::cout << "command execution end" << std::endl;
    // test
    // std::cout << "test" << std::endl;
    // DiskSystem::getInstance()->getCurrentPartition()->traversalTest();
}

void commandRd(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command rd" << std::endl;
    // std::cout << "rd " << arguments << " " << targets << std::endl;
}

void commandCd(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    const std::string errorMessage("can't find directory");
    if(targets.empty()) {
        std::cout << "cd" << std::endl;
        std::cout << DiskSystem::getInstance()->getCurrentPath() << std::endl;
    } else if (targets.size() > 1) {
        std::cout << errorMessage << std::endl;
    } else {
        std::cout << "cd [path]" << std::endl;
        auto currentNode = DiskSystem::getInstance()->getCurrentNode();
        auto currentPartition = DiskSystem::getInstance()->getCurrentPartition();
        for(auto it = targets.begin(); it != targets.end(); it++) {
            std::cout << "path:" << it->first << std::endl;
            if(currentPartition->getPartitionName().compare(it->first) == 0) {
                currentNode = currentPartition->getNTree();
            } else {
                auto dirIt = currentNode->getNTreeChildren().find(it->first);
                if(dirIt == currentNode->getNTreeChildren().end()) {
                    std::cout << errorMessage << std::endl;
                    return;
                }
            }
            for(auto dir : it->second) {
                auto dirIt = currentNode->getNTreeChildren().find(dir);
                if(dirIt == currentNode->getNTreeChildren().end()) {
                    std::cout << errorMessage << std::endl;
                    return;
                }
                // TODO: test
                currentNode = dirIt->second;
            }
        }
    }
}

void commandDel(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command del" << std::endl;
    // std::cout << "del " << arguments << " " << targets << std::endl;
}

void commandCopy(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command copy" << std::endl;
    // std::cout << "copy " << arguments << " " << targets << std::endl;
}

void commandRen(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command ren" << std::endl;
    // std::cout << "ren " << arguments << " " << targets << std::endl;
}

void commandMove(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command move" << std::endl;
    // std::cout << "move " << arguments << " " << targets << std::endl;
}

void commandMklink(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command mklink" << std::endl;
    // std::cout << "mklink " << arguments << " " << targets << std::endl;
}

void commandSave(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command save" << std::endl;
    // std::cout << "save " << arguments << " " << targets << std::endl;
}

void commandLoad(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command load" << std::endl;
    // std::cout << "load " << arguments << " " << std::endl;
}

void commandCls(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command cls" << std::endl;
    system("cls");
}

void commandExit(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command exit" << std::endl;
}

void commandError(std::vector<std::string> arguments, std::unordered_map<std::string, std::vector<std::string>> targets) {
    std::cout << "execute command error" << std::endl;
}
