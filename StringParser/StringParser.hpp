#pragma once

#include <string>
#include <vector>
#include <iostream>

class StringParser {
public:
    static StringParser* getInstance();
    static void destoryInstance();
    void parseString(const std::string& str);
    void parseResult() const;
    void clear();

private:
    StringParser();
    static StringParser* m_instance;
    void parse();
    std::string command;
    std::vector<std::string> arguments;
    std::vector<std::string> targets;

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
    command = std::string(itStart, it);

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
        arguments.push_back(std::string(itStart, it));
        // fault tolerence: space between arguments
        while(*it == ' ') {
            it++;
        }
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
            targets.push_back(std::string(itStart, it));
            // fault tolerence: space between targets
            while(*it == ' ') {
                it++;
            }
            itStart = it;
        }
    } while(it != str.end());
    targets.push_back(std::string(itStart, it));
}

void StringParser::parseResult() const {
    std::cout << "parse result:" << std::endl;
    std::cout << "command:" << command << std::endl;
    std::cout << "arguments:" << std::endl;
    for(auto argument : arguments) {
        std::cout << argument << " ";
    }
    std::cout << std::endl;
    std::cout << "target:" << std::endl;
    for(auto target : targets) {
        std::cout << target << " ";
    }
    std::cout << std::endl;
}

void StringParser::clear() {
    command.clear();
    arguments.clear();
    targets.clear();
}
