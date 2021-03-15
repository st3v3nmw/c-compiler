#include "scanner.hpp"

using namespace std;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    string test_file_path = "..\\tests\\submitted.c";
#else
    string test_file_path = "tests/submitted.c";
#endif

int main(int argc, char** argv) {
    if (argc == 2) // i.e. file called using a command line argument
        test_file_path = argv[1];

    vector<TokenNode> tokens = scanner(test_file_path);
    for (auto t : tokens)
        t.print();

    return 0;
}