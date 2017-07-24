#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

class Printer {
public:
    virtual void printResult() = 0;
};

class CommandDIRPrinter {
public:
    CommandDIRPrinter();
    virtual void printResult();

    std::string path;
    typedef std::tuple<std::string, std::string, std::string, std::string> Element;
    std::vector<Element> m_elements;
};

CommandDIRPrinter::CommandDIRPrinter() {

}

void CommandDIRPrinter::printResult() {

}

class CommandCDPrinter {
public:
    virtual void printResult();
};

void CommandCDPrinter::printResult() {

}
