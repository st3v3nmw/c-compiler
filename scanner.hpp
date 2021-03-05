#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

enum {
    T_EOF,
    T_INT, T_FLOAT, T_CHAR, T_BOOL, T_VOID, T_RETURN, T_BREAK, T_CONTINUE, T_FOR, T_WHILE, T_IF, T_ELSE, T_PRINT, // keywords
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_UNARY_PLUS, T_UNARY_MINUS, T_BITW_AND, T_BITW_OR, // arithmetic operators
    T_NEGATION, T_AND, T_OR,
    T_EQ, T_NEQ, T_LT, T_GT, T_LEQ, T_GEQ, // relational operators
    T_ASSIGN, T_PLUS_ASSIGN, T_MINUS_ASSIGN, T_STAR_ASSIGN, T_SLASH_ASSIGN, // assignment operators
    T_LCURLY, T_RCURLY, T_COMMA, T_LPAREN, T_RPAREN, T_SEMICOLON, // separators
    T_INTLIT, T_FLOAT_LIT, T_STRLIT, T_BOOLIT, T_IDENTIFIER, // literals & identifier
    T_COMMENT // not really a token, but put here for "pedagogical" reasons, will be discarded in next phase of compilation
};

string tokenString[] = {
    "EOF",
    "int", "float", "char", "bool", "void", "return", "break", "continue", "for", "while", "if", "else", "printf",
    "+", "-", "*", "/", "++", "--", "&", "|",
    "!", "&&", "||",
    "==", "!=", "<", ">", "<=", ">=",
    "=", "+=", "-=", "*=", "/=",
    "{", "}", ",", "(", ")", ";",
    "integer literal", "floating-point literal", "string literal", "boolean literal", "identifier",
    "comment"
};

map<string, int> keywords;
map<string, int> separators;
map<string, int> operators;

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
                cout << tokenString[this->token] << " -> " << this->value << "\n";
            else {
                if (this->token >= T_INT && this->token <= T_PRINT)
                    cout << "keyword -> " << tokenString[this->token] << "\n";
                else if (this->token >= T_PLUS && this->token <= T_SLASH_ASSIGN)
                    cout << "operator -> " << tokenString[this->token] << "\n";
                else if (this->token >= T_LCURLY && this->token <= T_SEMICOLON)
                    cout << "separator -> " << tokenString[this->token] << "\n";
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

int getTokenType(const string &token, int line_number) {
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
        cerr << "\nSyntax error\nUnexpected token " << token << " on line " << line_number << "\n";
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
    int line_number = 0;
    vector<TokenNode> tokens = {};
    // comment
    bool singlelinecomment = false;
    string buffer;
    string comm;
    // file handling
    ifstream infile;
    infile.open(file_name, ios::in);   // Attempt to open the file
    // Check for file status
    if(infile.fail()) {
        cerr << "File not found" << endl;
        exit(EXIT_FAILURE);
    }

    while (infile >> noskipws >> ch) {

        if (singlelinecomment){
            if(ch == '\n'){
                line_number++;
                tokens.push_back(TokenNode(getTokenType(comm, line_number), comm));
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

        if(isWhitespace(string(1, ch))){
            if (string(1, ch) == "\n")
                line_number++;
            if(!buffer.empty()){
                if(buffer[0] == '"'){
                    buffer += ch;
                }
                else{
                    tokens.push_back(TokenNode(getTokenType(buffer, line_number), buffer));
                    buffer = "";
                }

            }
            continue;
        }

        if (isOperator(string(1, ch)) && !isOperator(buffer)){
            if (!buffer.empty()){
                tokens.push_back(TokenNode(getTokenType(buffer, line_number), buffer));
                buffer = "";
            }
        }

        if (!isOperator(string(1, ch)) && isOperator(buffer)){
            tokens.push_back(TokenNode(getTokenType(buffer, line_number), buffer));
            buffer = "";
        }

        if (isSeparator(string(1, ch))) {
            if (!buffer.empty()) {
                tokens.push_back(TokenNode(getTokenType(buffer, line_number), buffer));
                buffer = "";
            }
            if (isSeparator(string(1, ch))) {
                tokens.push_back(TokenNode(getTokenType(string(1, ch), line_number), buffer));
                continue;
            }
        }
        buffer += ch;
    }

    // Close the file
    infile.close();

    return tokens;
}

#endif