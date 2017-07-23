#include "Any.hpp"
#include <string>

class A {
public:
    int m_int = -1;
};

int main(int argc, char* argv[])
{
    Any n0 = std::string("strng object");
    std::cout << n0.cast<std::string>() << std::endl;

    Any n1 = 1;
    std::cout << n1.cast<int>() << std::endl;

    Any n2 = A();
    std::cout << n2.cast<A>().m_int << std::endl;

    return 0;
}
