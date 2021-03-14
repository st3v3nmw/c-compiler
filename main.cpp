#include "scanner.hpp"

int main(int argc, char** argv) {
    unsigned int micro = 1000;
    string flex_file = "flex ..\\flex\\check.l";
    const char* command = flex_file.c_str();


    system(command);
    system("gcc lex.yy.c");
    cout<<"scanning..."<<endl;
    DELAY(2*micro);
    system("start a.exe");
    DELAY(2*micro);


    string file = "Output.txt";
    if (argc == 2) // i.e. file called using a command line argument
        file = argv[1];

    ifstream infile;
    infile.open(file, ios::in);   // attempt to open the file
    // check for file status
    if(infile.fail()) {
        perror("file open failed");
        exit(EXIT_FAILURE);
    }
    string output, label;
    while (getline(infile, output)){
        cout<<output<<" "<<label<<endl;
    }

    // close the file
    infile.close();


    return 0;
}