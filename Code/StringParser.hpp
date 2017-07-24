#pragma once

#include <string>
#include <vector>
#include <iostream>

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
    std::vector<std::string>& getTargets();

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

std::vector<std::string>& StringParser::getTargets() {
    return m_targetList;
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

void StringParser::parsePath() {
    std::string operatePath = DiskSystem::getInstance()->getCurrentPath();
    std::cout << "operatePath:" << operatePath << std::endl;
    NTree<File>* operateNode = DiskSystem::getInstance()->getCurrentNode().get(); // NOTE: get() return C-ptr
    std::cout << "operateNode:" << operateNode->getNTreeInfo()->getFileName() << std::endl;
    NTree<File>* operateNodeParent = operateNode->getParent(); // NOTE: getParent() return C-ptr
    for(auto target : m_targetList) {
        // calculate operator directory
        auto it = target.begin();
        auto start = it;
        do {
            if(*it == '\\') {
                std::string path(start, it);
                it++;
                start = it;
                // case: "command \folder"
                if(path.empty()) {
                    std::cout << "path:" << path << std::endl;
                    std::cout << "case0: \\" << std::endl;
                    // get node
                    operateNode = DiskSystem::getInstance()->getCurrentPartition().get();
                    std::cout << "operateNode:" << operateNode->getNTreeInfo()->getFileName() << std::endl;
                    // get path
                    operatePath = DiskSystem::getInstance()->getCurrentPartition()->getNTreeInfo()->getFileName();
                    std::cout << "operatePath:" << operatePath << std::endl;
                    auto hashMapIt = m_targets.find(operatePath);
                    if(hashMapIt == m_targets.end()) {
                        m_targets.emplace(operatePath, std::vector<std::string>({ target }));
                    } else {
                        hashMapIt->second.push_back(target);
                    }
                    break;
                }
                // case: "command .\folder"
                if(path.compare(".") == 0) {
                    std::cout << "path:" << path << std::endl;
                    std::cout << "case1: .\\" << std::endl;
                    // get node
                    operateNode = DiskSystem::getInstance()->getCurrentNode().get();
                    std::cout << "operateNode:" << operateNode->getNTreeInfo()->getFileName() << std::endl;
                    // get path
                    operatePath = DiskSystem::getInstance()->getCurrentPath();
                    std::cout << "operatePath:" << operatePath << std::endl;
                    auto hashMapIt = m_targets.find(operatePath);
                    if(hashMapIt == m_targets.end()) {
                        m_targets.emplace(operatePath, std::vector<std::string>({ target }));
                    } else {
                        hashMapIt->second.push_back(target);
                    }
                    continue;
                }
                // case: "command ..\folder"
                if(path.compare("..") == 0) {
                    std::cout << "path:" << path << std::endl;
                    std::cout << "case2: ..\\" << std::endl;
                    // get node
                    // if node's parent node is nullptr
                    if(operateNodeParent == nullptr) { // for "C:mkdir ..\folder"
                        // that means end of the partition
                        operateNodeParent = operateNode; // TODO: output command error
                        std::cout << "Error Command!" << std::endl;
                    } else { // if node's parent node is not nullptr
                        // step back to parent
                        operateNode = operateNode->getParent();
                        operateNodeParent = operateNode->getParent();
                    }
                    std::cout << "operateNode:" << operateNode->getNTreeInfo()->getFileName() << std::endl;

                    // get path
                    operatePath = operateNode->getNTreeInfo()->getFileName();
                    auto hashMapIt = m_targets.find(operatePath);
                    if(hashMapIt == m_targets.end()) {
                        m_targets.emplace(operatePath, std::vector<std::string>({ target }));
                    } else {
                        hashMapIt->second.push_back(target);
                    }
                    continue;
                }
                // case: "command folder\folder"
                std::cout << "path:" << path << std::endl;
                std::cout << "case3: folder\\folder" << std::endl;
                // get node: search or create a node here
                auto children = DiskSystem::getInstance()->getCurrentNode()->getChildren();
                // TODO: DiskSystem find if there is same node by creating a rvalue File, here provide string, DiskSystem use string to find
                // std::find(children.begin(), children.end(), );
                operateNode = DiskSystem::getInstance()->getCurrentNode().get();
                std::cout << "operateNode:" << operateNode->getNTreeInfo()->getFileName() << std::endl;
                // get path
                auto hashMapIt = m_targets.find(operatePath);
                if(hashMapIt == m_targets.end()) {
                    m_targets.emplace(path, std::vector<std::string>({ target }));
                } else {
                    hashMapIt->second.push_back(target);
                }
            }
            it++;
        } while(it != target.end());
    }
    for(auto target : m_targets) {
        std::cout << "path:" << target.first << std::endl;
        std::cout << "targets:";
        for(auto t : target.second) {
            std::cout << t << " ";
        }
        std::cout << std::endl;
    }
}
