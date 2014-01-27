#include <string>
#include <cstdio>
#include <cstdlib>

int main()
{
    std::string myhex = "abcdef";
    unsigned int x = strtoul(myhex.substr(0, 2).c_str(), NULL, 16);
    printf("x = %d\n", x);
}
