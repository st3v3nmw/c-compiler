#include "scanner.hpp"

int main(int argc, char** argv) {
    string file = "tests/submitted.c";
    if (argc == 2) // i.e. file called using a command line argument
        file = argv[1];

    vector<TokenNode> tokens = scanner(file);
    for (auto t : tokens)
        t.print();

    return 0;
}