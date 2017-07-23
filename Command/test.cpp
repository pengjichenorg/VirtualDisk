#include <iostream>
#include "Command.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    Command command;
    command.getExecutor("dir")("/s/ad", "asd");
    return 0;
}
