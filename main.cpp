#include "scanner.hpp"

using namespace std;

// use different file paths based on the platform i.e. Windows or Linux
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    string test_file_path = "..\\tests\\test08.c";
#else
    string test_file_path = "tests/submitted.c";
#endif

int main(int argc, char** argv) {
    if (argc == 2) // file called using a command line argument i.e. ./main tests/test08.c
        test_file_path = argv[1];

    vector<TokenNode> tokens = scanner(test_file_path); // scan for all tokens
    for (auto t : tokens)
        t.print();  // print the tokens

    return 0;
}