#include "StringParser.hpp"

int main(int argc, char* argv[])
{
    auto parser = StringParser::getInstance();
    std::string inputString;
    // std::getline(std::cin, inputString);
    inputString = "dir     asd";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    inputString = "dir /s asd";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    inputString = "dir /s/d asd";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    inputString = "dir /s asd asr";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    inputString = "dir /s/d asd asr";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    inputString = "dir /s /d asd asr";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    inputString = "     dir      /s     /d      asd     asr";
    parser->parseString(inputString);
    parser->parseResult();
    parser->clear();

    StringParser::destoryInstance();
    return 0;
}
