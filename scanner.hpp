#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <vector>
#include "defs.hpp"

using namespace std;

ifstream infile;

/*
    The TokenNode class holds the information about scanned tokens.
    i.e. Token type and Token value
    Example: 1. for a string "abcd", the token type is T_STR_LIT and value = "abcd"
             2. for a keyword "float", the token type is T_KEYWORD and the value = "float"
*/
class TokenNode {
    public:
        int token;
        string value;

        TokenNode(int token, string value) {
            this->token = token;
            this->value = value;
        }

        // print a string representation of the token
        void print() {
            if (this->token == T_STR_LIT)
                cout << "string -> \"" << this->value << "\"" << endl;
            else if (this->token == T_CHAR_LIT)
                cout << "character literal -> '" << this->value << "'" << endl;
            else if (this->token >= T_INT_LIT)
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

// scan a number, integer or float
string scanNumber() {
    string buffer = "";
    char ch;
    infile >> noskipws >> ch;
    while (isdigit(ch) && !infile.eof()) { // scan number before decimal point (if any exists)
        buffer += ch;
        infile >> noskipws >> ch;
    }
    if (ch == '.') { // decimal point for float
        buffer += ch;
        infile >> noskipws >> ch;
        while (isdigit(ch) && !infile.eof()) {
            buffer += ch;
            infile >> noskipws >> ch;
        }
        infile.putback(ch); // putback unused character to file stream
    } else
        infile.putback(ch); // putback unused character to file stream
    return buffer;
}

vector<TokenNode> scanner(const string &file_name) {
    char ch;
    vector<TokenNode> tokens = {};
    string buffer = "";
    infile.open(file_name, ios::in);   // attempt to open the file
    if (infile.fail()) { // check for file status
        perror("file open failed");
        exit(EXIT_FAILURE);
    }

    int line_number = 1;

    while (infile >> noskipws >> ch) {
        switch (ch) {
            case '{':
                tokens.push_back(TokenNode(T_LCURLY, "{"));
                break;
            case '}':
                tokens.push_back(TokenNode(T_RCURLY, "}"));
                break;
            case '(':
                tokens.push_back(TokenNode(T_LPAREN, "("));
                break;
            case ')':
                tokens.push_back(TokenNode(T_RPAREN, ")"));
                break;
            case ',':
                tokens.push_back(TokenNode(T_COMMA, ","));
                break;
            case ';':
                tokens.push_back(TokenNode(T_SEMICOLON, ";"));
                break;
            case '=':
                infile >> noskipws >> ch;
                if (ch == '=')
                    tokens.push_back(TokenNode(T_EQ, "=="));
                else {
                    tokens.push_back(TokenNode(T_ASSIGN, "="));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '\"': // match strings
                buffer = "";
                infile >> noskipws >> ch;
                while (ch != '\"' && !infile.eof()) {
                    buffer += ch;
                    infile >> noskipws >> ch;
                    if (ch == '\\' && infile.peek() == '\"') { // process \" string escapes
                        buffer += ch;
                        infile >> noskipws >> ch;
                        buffer += ch;
                        infile >> noskipws >> ch;
                    }
                }
                tokens.push_back(TokenNode(T_STR_LIT, buffer));
                break;
            case '/':
                infile >> noskipws >> ch;
                if (ch == '/') { // match comment
                    buffer = "/";
                    while (ch != '\n' && !infile.eof()) {
                        buffer += ch;
                        infile >> noskipws >> ch;
                    }
                    line_number++; // move count to next line
                    tokens.push_back(TokenNode(T_COMMENT, buffer));
                } else if (ch == '=') { // /=
                    tokens.push_back(TokenNode(T_SLASH_ASSIGN, "/="));
                } else {
                    tokens.push_back(TokenNode(T_SLASH, "/"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '!':
                infile >> noskipws >> ch;
                if (ch == '=')
                    tokens.push_back(TokenNode(T_NEQ, "!="));
                else {
                    tokens.push_back(TokenNode(T_NEGATION, "!"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '&':
                infile >> noskipws >> ch;
                if (ch == '&')
                    tokens.push_back(TokenNode(T_AND, "&&"));
                else {
                    tokens.push_back(TokenNode(T_BITW_AND, "&"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '|':
                infile >> noskipws >> ch;
                if (ch == '|')
                    tokens.push_back(TokenNode(T_OR, "||"));
                else {
                    tokens.push_back(TokenNode(T_BITW_OR, "|"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '*':
                infile >> noskipws >> ch;
                if (ch == '=')
                    tokens.push_back(TokenNode(T_STAR_ASSIGN, "*="));
                else {
                    tokens.push_back(TokenNode(T_STAR, "*"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '>':
                infile >> noskipws >> ch;
                if (ch == '=')
                    tokens.push_back(TokenNode(T_GEQ, ">="));
                else {
                    tokens.push_back(TokenNode(T_GT, ">"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '<':
                infile >> noskipws >> ch;
                if (ch == '=')
                    tokens.push_back(TokenNode(T_LEQ, "<="));
                else {
                    tokens.push_back(TokenNode(T_LT, "<"));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '\'': // match a character literal i.e. 'q'
                infile >> noskipws >> ch;
                tokens.push_back(TokenNode(T_CHAR_LIT, ch == '\'' ? "" : string(1, ch)));
                if (infile.peek() == '\'') // else char literal isn't properly closed
                    infile >> noskipws >> ch; // consume closing ' and discard it
                break;
            default:
                if (isalpha(ch) || ch == '_') { // match identifier or keyword or boolean literal
                    buffer = ch;
                    infile >> noskipws >> ch;
                    while ((isalnum(ch) || ch == '_') && !infile.eof()) {
                        buffer += ch;
                        infile >> noskipws >> ch;
                    }

                    int tokenType = (buffer == "true" || buffer == "false") ? T_BOOL_LIT : T_IDENTIFIER;
                    for (int i = T_INT; i <= T_PRINT; i++) {
                        if (buffer == tokenString[i]) {
                            tokenType = i;
                            break;
                        }
                    }
                    tokens.push_back(TokenNode(tokenType, buffer));
                    infile.putback(ch); // putback unused character to file stream
                } else if (isdigit(ch)) { // match non-signed number (float or int)
                    buffer = ch + scanNumber();
                    size_t point = buffer.find_first_of(".");
                    if (point == string::npos) // integer
                        tokens.push_back(TokenNode(T_INT_LIT, buffer));
                    else // float
                        tokens.push_back(TokenNode(T_FLOAT_LIT, buffer));
                } else if (ch == '+' || ch == '-') {
                    if (isdigit(infile.peek())) { // match signed number (float or int)
                        buffer = ch + scanNumber();
                        size_t point = buffer.find_first_of(".");
                        if (point == string::npos) // integer
                            tokens.push_back(TokenNode(T_INT_LIT, buffer));
                        else // float
                            tokens.push_back(TokenNode(T_FLOAT_LIT, buffer));
                    } else if (infile.peek() == ch && (infile.peek() == '+' | infile.peek() == '-')) { // match unary ++ or --
                        tokens.push_back(TokenNode(infile.peek() == '+' ? T_UNARY_PLUS : T_UNARY_MINUS, infile.peek() == '+' ? "++" : "--"));
                        infile >> noskipws >> ch; // consume last + or - and discard it
                    } else {
                        if (infile.peek() == '=') {
                            tokens.push_back(TokenNode(ch == '+' ? T_PLUS_ASSIGN : T_MINUS_ASSIGN, ch == '+' ? "+=" : "-="));
                            infile >> noskipws >> ch; // consume = and discard it
                        } else
                            tokens.push_back(TokenNode(ch == '+' ? T_PLUS : T_MINUS, ch == '+' ? "+" : "-"));
                    }
                } else {
                    if (ch == '\n')
                        line_number++; // move count to next line
                    else if (!(ch == ' ' || ch == '\t' || ch == '\r')) // ignore whitespace
                        cerr << "Unexpected character " << ch << " on line " << line_number << endl;
                }
        }
    }

    // close the file
    infile.close();

    return tokens;
}

#endif