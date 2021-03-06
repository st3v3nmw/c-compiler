#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "defs.hpp"

using namespace std;

// global variables
map<string, int> keywords;
map<string, int> separators;
map<string, int> operators;
string line;
int line_number, cur_pos, token_start_pos;

class TokenNode {
    public:
        int token;
        string value;

        TokenNode(int token, string value) {
            this->token = token;
            this->value = value;
        }

        void print() {
            if (this->token >= T_INTLIT)
                cout << tokenString[this->token] << " -> " << this->value << endl;
            else {
                if (this->token >= T_INT && this->token <= T_PRINT)
                    cout << "keyword -> " << tokenString[this->token] << endl;
                else if (this->token >= T_PLUS && this->token <= T_SLASH_ASSIGN)
                    cout << "operator -> " << tokenString[this->token] << endl;
                else if (this->token >= T_LCURLY && this->token <= T_SEMICOLON)
                    cout << "separator -> " << tokenString[this->token] << endl;
            }
        }
};


bool isString(const string &str){
    return str[0] == '"' && str[str.size()-1] == '"';
}

bool isInteger(const string &str){
    return all_of(str.begin(), str.end(), ::isdigit);
}

bool isBoolean(const string &str){
    return str == "true" || str == "false";
}

bool isComment(const string &str){
    return str[0] == '/' && str[1] == '/';
}

bool isWhitespace(const string &str){
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

int isKeyword(const string &str){
    if (keywords.find(str) == keywords.end()) // str not found
        return 0;
    return keywords[str]; // return token type
}

int isOperator(const string &str){
    if (operators.find(str) == operators.end()) // str not found
        return 0;
    return operators[str]; // return token type
}

int isSeparator(const string &str){
    if (separators.find(str) == separators.end()) // str not found
        return 0;
    return separators[str]; // return token type
}

bool isFloat(const string &str) {
    size_t found = str.find_first_of(".");
    if (found == string::npos)
        return false;
    return isInteger(str.substr(0, found)) && isInteger(str.substr(found+1));
}

int getTokenType(const string &token) {
    if (isComment(token))
        return T_COMMENT;
    else if (isInteger(token))
        return T_INTLIT;
    else if (isBoolean(token))
        return T_BOOL;
    else if (isSeparator(token))
        return separators[token];
    else if (isKeyword(token))
        return keywords[token];
    else if (isIdentifier(token))
        return T_IDENTIFIER;
    else if (isOperator(token))
        return operators[token];
    else if (isString(token))
        return T_STRLIT;
    else if (isFloat(token))
        return T_FLOAT_LIT;
    else {
        cerr << endl << line;
        if (line[line.size() - 1] != '\n')
            cerr << endl;
        if (token_start_pos - 1 > 0)
            cerr << string(token_start_pos - 1, ' ');
        cerr << string(cur_pos - token_start_pos - 1, '^') << endl;
        cerr << "Unexpected token " << token << " on line " << line_number << endl;
        exit(2);
    }
}

vector<TokenNode> scanner(const string &file_name) {
    // populate keywords map, it maps keywords to T_sth values in the enum
    for (int t = T_INT; t <= T_PRINT; t++)
        keywords.insert({tokenString[t], t});

    for (int t = T_PLUS; t <= T_SLASH_ASSIGN; t++)
        operators.insert({tokenString[t], t});

    for (int t = T_LCURLY; t <= T_SEMICOLON; t++)
        separators.insert({tokenString[t], t});

    char ch;
    vector<TokenNode> tokens = {};
    // comment
    bool singlelinecomment = false;
    string buffer;
    string comm;
    // file handling
    ifstream infile;
    infile.open(file_name, ios::in);   // attempt to open the file
    // check for file status
    if(infile.fail()) {
        perror("file open failed");
        exit(EXIT_FAILURE);
    }

    while (infile >> noskipws >> ch) {
        line += ch;
        cur_pos += 1;
        if (singlelinecomment) {
            if(ch == '\n') {
                tokens.push_back(TokenNode(getTokenType(comm), comm));
                singlelinecomment = false;
                line_number++;
                token_start_pos = 1;
                cur_pos = 1;
                line = "";
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

        if (isWhitespace(string(1, ch))) {
            if (!buffer.empty()) {
                if( buffer[0] == '"' ){
                    buffer += ch;
                } else{
                    tokens.push_back(TokenNode(getTokenType(buffer), buffer));
                    token_start_pos = cur_pos;
                    buffer = "";
                }

            }

            if (string(1, ch) == "\n") {
                line_number++;
                token_start_pos = 1;
                cur_pos = 1;
                line = "";            }
            continue;
        }

        if (isOperator(string(1, ch)) && !isOperator(buffer)) {
            if (!buffer.empty()){
                tokens.push_back(TokenNode(getTokenType(buffer), buffer));
                token_start_pos = cur_pos;
                buffer = "";
            }
        }

        if (!isOperator(string(1, ch)) && isOperator(buffer)) {
            tokens.push_back(TokenNode(getTokenType(buffer), buffer));
            token_start_pos = cur_pos;
            buffer = "";
        }

        if (isSeparator(string(1, ch))) {
            if (!buffer.empty()) {
                tokens.push_back(TokenNode(getTokenType(buffer), buffer));
                token_start_pos = cur_pos;
                buffer = "";
            }
            if (isSeparator(string(1, ch))) {
                tokens.push_back(TokenNode(getTokenType(string(1, ch)), buffer));
                token_start_pos = cur_pos;
                continue;
            }
        }
        buffer += ch;
    }

    // close the file
    infile.close();

    return tokens;
}

#endif