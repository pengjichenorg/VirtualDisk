#pragma once

#include "DiskSystem.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

class StringParser {
public:
    /**
     * get string parser instance
     * @return instance
     */
    static StringParser* getInstance();

    /**
     * destory string parser instance
     */
    static void destoryInstance();

    /**
     * parse string
     * @param str read
     */
    void parseString(const std::string& str);

    /**
     * get parse string command
     */
    const std::string& getCommand() const;

    /**
     * get parse string arguments
     */
    std::vector<std::string>& getArguments();

    /**
     * get parse string targets
     */
    std::unordered_map<std::string, std::vector<std::string>>& getTargets();

    /**
     * output parse result for test
     */
    void parseResult() const;

    /**
     * clear parse result for next parsing
     */
    void clear();

private:
    /**
     * private constructor for singleton pattern
     */
    StringParser();

    /**
     * parse each target's path from target list
     */
    void parsePath();

    static StringParser* m_instance;
    std::string m_command;
    std::vector<std::string> m_arguments;
    std::vector<std::string> m_targetList;
    std::unordered_map<std::string, std::vector<std::string>> m_targets;

};

StringParser* StringParser::m_instance = nullptr;

StringParser::StringParser() {

}

StringParser* StringParser::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new StringParser();
    }
    return m_instance;
}

void StringParser::destoryInstance() {
    if(m_instance != nullptr) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void StringParser::parseString(const std::string& str) {
    auto it = str.begin();

    // fault tolerence
    while(*it == ' ') {
        it++;
    }
    // command
    auto itStart = it;
    do {
        it++;
    } while(*it != ' ' && it != str.end());
    m_command = std::string(itStart, it);

    // fault tolerence: no arguments no targets
    if(it == str.end()) {
        return;
    }
    // fault tolerence: space between command and arguments
    while(*it == ' ') {
        it++;
    }
    // arguments
    while(*it == '/') { // argument is same like: /d /s /r /...
        itStart = it;
        do {
            it++;
        } while(*it != ' ' && it != str.end());
        m_arguments.push_back(std::string(itStart, it));
        // fault tolerence: no arguments no targets
        if(it == str.end()) {
            return;
        }
        // fault tolerence: space between arguments
        while(*it == ' ') {
            it++;
        }
    }

    // fault tolerence: no arguments no targets
    if(it == str.end()) {
        return;
    }
    // fault tolerence: space between arguments and targets
    while(*it == ' ') {
        it++;
    }
    // targets
    itStart = it;
    do {
        it++;
        if(*it == ' ') {
            m_targetList.push_back(std::string(itStart, it));
            // fault tolerence: no arguments no targets
            if(it == str.end()) {
                return;
            }
            // fault tolerence: space between targets
            while(*it == ' ') {
                it++;
            }
            itStart = it;
        }
    } while(it != str.end());
    m_targetList.push_back(std::string(itStart, it));
    parsePath();
}

const std::string& StringParser::getCommand() const {
    return m_command;
}

std::vector<std::string>& StringParser::getArguments() {
    return m_arguments;
}

std::unordered_map<std::string, std::vector<std::string>>& StringParser::getTargets() {
    return m_targets;
}

void StringParser::parseResult() const {
    std::cout << "parse result:" << std::endl;
    std::cout << "command:" << m_command << std::endl;
    std::cout << "arguments:" << std::endl;
    for(auto argument : m_arguments) {
        std::cout << argument << " ";
    }
    std::cout << std::endl;
    std::cout << "target list:" << std::endl;
    for(auto target : m_targetList) {
        std::cout << target << " ";
    }
    std::cout << std::endl;
}

void StringParser::clear() {
    m_command.clear();
    m_arguments.clear();
    m_targetList.clear();
    m_targets.clear();
}

// 正向分析
// void StringParser::parsePath() {
//     std::string operatePath = DiskSystem::getInstance()->getCurrentPath();
//     std::cout << "operatePath:" << operatePath << std::endl;
//     NTree<File>* operateNode = DiskSystem::getInstance()->getCurrentNode();
//     std::cout << "operateNode:" << operateNode->getNTreeValue() << std::endl;
//     NTree<File>* operateNodeParent = operateNode->getNTreeParent();
//     for(auto target : m_targetList) {
//         std::cout << "\npath+target:" << target << std::endl;
//         auto findalIt = target.end();
//         while(findalIt != target.begin() && *findalIt != '\\') {
//             findalIt--;
//         };
//         std::string finalTarget(findalIt+1, target.end());
//         std::cout << "findal target:" << finalTarget << std::endl;
//         if(findalIt == target.begin()) {
//             // case: "command folder"
//             std::cout << "command folder" << std::endl;
//             operatePath = DiskSystem::getInstance()->getCurrentPath();
//             std::cout << "operatePath:" << operatePath << std::endl;
//             auto findIt = m_targets.find(operatePath);
//             if(findIt == m_targets.end()) {
//                 m_targets.emplace(operatePath, std::vector<std::string>({ target }));
//             } else {
//                 findIt->second.push_back(target);
//             }
//             continue;
//         }
//         // calculate operator directory
//         auto it = target.begin();
//         auto start = it;
//         do {
//             // calculate operate path
//             if(*it == '\\') {
//                 std::string path(start, it);
//                 // std::string _tar();
//                 if(path.empty()) {
//                     std::cout << "empty" << std::endl;
//                 } else {
//                     std::cout << path << std::endl;
//                 }
//                 it++;
//                 start = it;
//                 // case: "command \folder"
//                 if(path.empty()) {
//                     std::cout << "command \\folder" << std::endl;
//                     operatePath = DiskSystem::getInstance()->getCurrentPartition()->getNTree()->getNTreeValue().getFileName();
//                     std::cout << "operatePath:" << operatePath << std::endl;
//                     auto findIt = m_targets.find(operatePath);
//                     if(findIt == m_targets.end()) {
//                         m_targets.emplace(operatePath, std::vector<std::string>({ target }));
//                     } else {
//                         findIt->second.push_back(target);
//                     }
//                 } else if(path.compare(".") == 0) { // case: "command .\folder"
//                     std::cout << "command .\\folder" << std::endl;
//                     operatePath = DiskSystem::getInstance()->getCurrentPath();
//                     std::cout << "operatePath:" << operatePath << std::endl;
//                     auto findIt = m_targets.find(operatePath);
//                     if(findIt == m_targets.end()) {
//                         m_targets.emplace(operatePath, std::vector<std::string>({ target }));
//                     } else {
//                         findIt->second.push_back(target);
//                     }
//                 } else if(path.compare("..") == 0) { // case: "command ..\folder"
//                     std::cout << "command ..\\folder" << std::endl;
//                     operateNode = DiskSystem::getInstance()->getCurrentNode();
//                     if(operateNode == DiskSystem::getInstance()->getCurrentPartition()->getNTree()) {
//                         operatePath = operateNode->getNTreeValue().getFileName();
//                     } else {
//                         operatePath = DiskSystem::getInstance()->getCurrentPartition()->getFullPath(operateNode);
//                     }
//                     std::cout << "operatePath:" << operatePath << std::endl;
//                     auto findIt = m_targets.find(operatePath);
//                     if(findIt == m_targets.end()) {
//                         m_targets.emplace(operatePath, std::vector<std::string>({ target }));
//                     } else {
//                         findIt->second.push_back(target);
//                     }
//                 } else { // case: "command folder\folder"
//                     std::cout << "command folder\\folder" << std::endl;
//                     operateNode = DiskSystem::getInstance()->getCurrentNode();
//                     operatePath = path;
//                     std::cout << "operatePath:" << operatePath << std::endl;
//                     auto findIt = m_targets.find(operatePath);
//                     if(findIt == m_targets.end()) {
//                         m_targets.emplace(operatePath, std::vector<std::string>({ target }));
//                     } else {
//                         findIt->second.push_back(target);
//                     }
//                 }
//             }
//             it++;
//         } while(it != target.end());
//     }
//     std::cout << "\nresult:" << std::endl;
//     for(auto target : m_targets) {
//         std::cout << "path:" << target.first << std::endl;
//         std::cout << "targets:";
//         for(auto t : target.second) {
//             std::cout << t << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// // 反向分析
// void StringParser::parsePath() {
//     std::string operatePath = DiskSystem::getInstance()->getCurrentPath();
//     NTree<File>* operateNode = DiskSystem::getInstance()->getCurrentNode();
//     NTree<File>* operateNodeParent = operateNode->getNTreeParent();
//     for(auto target : m_targetList) {
//         std::cout << "\npath+target:" << target << std::endl;
//         size_t findIt = 0;
//         findIt = target.find_last_of("\\");
//         if(findIt == std::string::npos) { // case: command target
//             std::cout << "operate target:" << target << std::endl;
//             auto operatePath = DiskSystem::getInstance()->getCurrentPath();
//             std::cout << "operate path:" << operatePath << std::endl;
//             auto findIt = m_targets.find(operatePath);
//             if(findIt == m_targets.end()) {
//                 m_targets.emplace(operatePath, std::vector<std::string>({ target }));
//             } else {
//                 findIt->second.push_back(target);
//             }
//         } else {
//             do {
//                 auto operateTarget = target.substr(findIt+1); // f1\f2 -> f2
//                 std::cout << "operate target:" << operateTarget << std::endl;
//                 target = target.substr(0, findIt); // f1\f2 -> f1
//                 if(target.empty()) { // case: command \target
//                     auto operatePath = DiskSystem::getInstance()->getCurrentPartition()->getNTree()->getNTreeValue().getFileName();
//                     std::cout << "operate path:" << operatePath << std::endl;
//                     auto findIt = m_targets.find(operatePath);
//                     if(findIt == m_targets.end()) {
//                         m_targets.emplace(operatePath, std::vector<std::string>({ operateTarget }));
//                     } else {
//                         findIt->second.push_back(operateTarget);
//                     }
//                     break;
//                 }
//                 findIt = target.find_last_of("\\");
//                 auto operatePath = target.substr(findIt+1);
//                 std::cout << "operate path:" << operatePath << std::endl; // case: command path\target
//                 auto findIt = m_targets.find(operatePath);
//                 if(findIt == m_targets.end()) {
//                     m_targets.emplace(operatePath, std::vector<std::string>({ operateTarget }));
//                 } else {
//                     findIt->second.push_back(operateTarget);
//                 }
//             } while(findIt != std::string::npos);
//         }
//     }
//     for(auto target : m_targets) {
//         std::cout << "path:" << target.first << std::endl;
//         std::cout << "targets:";
//         for(auto t : target.second) {
//             std::cout << t << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// 路径加法分析
void StringParser::parsePath() {
    std::string operatePath = DiskSystem::getInstance()->getCurrentPath();
    NTree<File>* operateNode = DiskSystem::getInstance()->getCurrentNode();
    NTree<File>* operateNodeParent = operateNode->getNTreeParent();
    for(auto target : m_targetList) {
        std::cout << "path target:" << target << std::endl;
        auto findPos = target.find_last_of("\\");
        auto operateTarget = target.substr(findPos+1);
        if(findPos == std::string::npos) { // no path
            std::cout << "case: cmd folder" << std::endl;
            operatePath = ".";
            std::cout << "operate path:" << operatePath << std::endl;
        } else {
            // in condition: cmd \f1, operatePath is empty
            operatePath = target.substr(0, findPos);
            if(operatePath.empty()) {
                std::cout << "case: cmd \\folder" << std::endl; // NOTE: use empty std::string as key word
            } else {
                std::cout << "case: cmd [path]\\folder" << std::endl;
            }
            std::cout << "operate path:" << operatePath << std::endl;
        }
        auto findIt = m_targets.find(operatePath);
        if(findIt == m_targets.end()) {
            m_targets.emplace(operatePath, std::vector<std::string>({ operateTarget }));
        } else {
            findIt->second.push_back(operateTarget);
        }
    }
    // test
    for(auto target : m_targets) {
        std::cout << "path:" << target.first << std::endl;
        std::cout << "targets:";
        for(auto t : target.second) {
            std::cout << t << " ";
        }
        std::cout << std::endl;
    }
}
