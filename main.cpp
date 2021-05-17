#include "scanner.hpp"
#include "parser.hpp"
#include "icg.hpp"
#include <tuple>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

// use different file paths based on the platform i.e. Windows or Linux
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    string test_file_path = "..\\tests\\submitted.c";
#else
    string test_file_path = "tests/submitted.c";
#endif

void handler(int sig) {
    // print the backtrace using gdb
    char pid_buf[32];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)] = 0;

    int child_pid = fork();
    if (!child_pid) {
        dup2(2, 1); // redirect output to stderr
        fprintf(stdout, "\nStack trace for %s pid=%s\n", name_buf, pid_buf);
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); // if gdb failed to start
    } else {
        waitpid(child_pid, NULL, 0);
    }

    exit(1);
}

int main(int argc, char** argv) {
    if (argc == 2) // file called using a command line argument i.e. ./main tests/test08.c
        test_file_path = argv[1];

    signal(SIGSEGV, handler);

    vector<TokenNode> tokens;
    vector<string> lines;

    cout << "Scanning..." << endl;
    tie(tokens, lines) = scan(test_file_path); // scan for all tokens
    // for (auto t : tokens)
    //     t.print();  // print the tokens

    cout << "\nParsing..." << endl;
    shared_ptr<ASTNode> root = parse(tokens, lines);

    cout << "\nGenerating intermediate code..." << endl;
    genIntermediateCode(root);

    return 0;
}