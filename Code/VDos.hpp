#pragma once

#include "StringParser.hpp"
#include "Command.hpp"

#include <iostream>
#include <memory>
#include <string>

class VDos {
public:
    VDos();
    ~VDos();
    void run();

private:
    std::shared_ptr<Command> m_command = nullptr;
};

VDos::VDos() {
    m_command = std::make_shared<Command>();
}

VDos::~VDos() {
    StringParser::getInstance()->destoryInstance();
}

void VDos::run() {
    while(true) {
        std::cout << DiskSystem::getInstance()->getCurrentPath();
        // input command
        std::string inputString;
        std::getline(std::cin, inputString);

        // parse input string as command
        StringParser::getInstance()->parseString(inputString);

        // NOTE: for test
        // StringParser::getInstance()->parseResult();

        // get command exeutor
        auto commandExecutor = m_command->getExecutor(StringParser::getInstance()->getCommand());

        // execute command
        commandExecutor(StringParser::getInstance()->getArguments(), StringParser::getInstance()->getTargets());

        // clear parse string
        StringParser::getInstance()->clear();
    }
}
