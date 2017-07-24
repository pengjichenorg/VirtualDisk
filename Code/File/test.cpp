#include "File.hpp"

int main(int argc, char const *argv[]) {
    auto f = new File("DOC.md");
    delete f;
    return 0;
}
