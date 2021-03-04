#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


bool isString(const string &str){
    return str[0] == '"' && str[str.size()-1] == '"';
}

bool isDigit(const string &str){
    return all_of(str.begin(), str.end(), ::isdigit);
}

bool isBoolean(const string &str){
    return str == "true" || str == "false";
}

bool isComment(const string &str){
    return str[0] == '/' && str[1] == '/';
}

bool isLiteral(const string &str){
    return isDigit(str) || isBoolean(str);
}

bool isIllegal(const string &str){
    return str == " " || str == "\n";
}

bool isIdentifier(const string &str){
    if(isdigit(str[0])){
        return false;
    }
    int itr = 0;
    if(str[0] == '_')
        itr++;
    while (itr<str.size()){
        if(!(isalnum(str[itr]) || str[itr] == '_'))
            return false;
        itr++;
    }
    return true;
}

bool isKeyword(const string &str){
    const vector<string> preserved{"int", "float", "printf", "void", "return"};
    for (const auto &keyword : preserved){
        if(keyword == str){
            return true;
        }
    }
    return false;
}

bool isStatement(const string &str){
    const vector<string> statement{"for", "while", "if", "else"};
    for (const auto &state : statement){
        if (state == str){
            return true;
        }
    }
     return false;
}

bool isOperator(const string &str){
    const vector<string> operators{"<", ">", "<=", ">=", "*", "+", "-", "/", "=", "-=", "*=", "+=", "/=", "++", "--", "=="};
    for (const auto &opt : operators){
        if (opt == str){
            return true;
        }
    }
    return false;
}

bool isSeparator(const string &str){
    const vector<string> separators{"{", "}", ",", "(", ")", ";"};
    for(const auto &sep : separators){
        if (sep == str){
            return true;
        }
    }
    return false;
}

void printToken(const string &token){
    if(isComment(token)){
        cout<<"(comment -> " <<token<<" )\n";
    } else if (isLiteral(token)){
        cout<<"(literal -> " <<token<< " )\n";
    } else if (isSeparator(token)){
        cout<<"(separator -> " <<token<< " )\n";
    } else if (isKeyword(token)){
        cout<<"(keyword -> " <<token<< " )\n";
    }  else if (isStatement(token)){
        cout<<"(keyword-statement -> " <<token<< " )\n";
    } else if (isIdentifier(token)){
        cout<<"(identifier -> " <<token<< " )\n";
    } else if (isOperator(token)){
        cout<<"(operator -> " <<token<< " )\n";
    } else if (isString(token)){
        cout<<"(string -> " <<token<< " )\n";
    }
    else{
        throw runtime_error(token);
    }
}

void scanner(const string &file_name){
    char ch;
    //comment
    bool singlelinecomment = false;
    string buffer;
    string comm;
    //file handling
    ifstream infile;
    infile.open (file_name, ios::in);   //Attempt to open the file
    // Check for file status
    if(infile.fail()) {
        cout << "File not found" << endl;
        exit(EXIT_FAILURE);
    }


    while (infile >> noskipws >> ch){

        if (singlelinecomment){
            if(ch == '\n'){
                printToken(comm);
                singlelinecomment = false;
            }
            if (singlelinecomment)
                comm += ch;
            else
                comm = "";
            continue;
        }

        if (ch == '/'){
            infile >> ch;
            if (ch == '/'){
                singlelinecomment = true;
                comm += ch;
            }
            if (singlelinecomment){
                comm += ch;
                continue;
            }
        }

        if(isIllegal(string(1, ch))){
            if(!buffer.empty()){
                if(buffer[0] == '"'){
                    buffer += ch;
                }
                else{
                    printToken(buffer);
                    buffer = "";
                }

            }
            continue;
        }

        if (isOperator(string(1, ch)) && !isOperator(buffer)){
            if (!buffer.empty()){
                printToken(buffer);
                buffer = "";
            }
        }

        if (!isOperator(string(1, ch)) && isOperator(buffer)){
            printToken(buffer);
            buffer = "";
        }

        if (isSeparator(string(1, ch))) {
            if (!buffer.empty()) {
                printToken(buffer);
                buffer = "";
            }
            if (isSeparator(string(1, ch))) {
                printToken(string(1, ch));
                continue;
            }
        }
        buffer += ch;
    }

    //Close the file
    infile.close();
}


int main() {
    string file = "..\\c_program.dat";
    scanner(file);
    return 0;
}
