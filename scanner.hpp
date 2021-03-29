#pragma once
#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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
        int token, line_number, token_end;
        string value;

        TokenNode(int token, string value, int line_number, int token_end) {
            this->token = token;
            this->value = value;
            this->line_number = line_number;
            this->token_end = token_end;
        }

        // print a string representation of the token
        void print() {
            cout << "(line " << line_number + 1 << " " << token_end - value.size() << ":" << token_end << ") ";
            if (token == T_STR_LIT)
                cout << "string -> \"" << value << "\"";
            else if (token == T_CHAR_LIT)
                cout << "character literal -> '" << value << "'";
            else if (token >= T_INT_LIT)
                cout << tokenString[token] << " -> " << value;
            else {
                if (token >= T_INT && token <= T_PRINT)
                    cout << "keyword -> " << tokenString[token];
                else if (token >= T_PLUS && token <= T_SLASH_ASSIGN)
                    cout << "operator -> " << tokenString[token];
                else if (token >= T_LCURLY && token <= T_SEMICOLON)
                    cout << "separator -> " << tokenString[token];
            }
            cout << endl;
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

pair<vector<TokenNode>, vector<string>> scan(const string &file_name) {
    infile.open(file_name, ios::in);   // attempt to open the file
    if (infile.fail()) { // check for file status
        perror("file open failed");
        exit(EXIT_FAILURE);
    }

    char ch;
    string buffer = "", line = "";
    vector<TokenNode> tokens = {};
    int line_number = 0, token_end = 0;

    while (infile >> noskipws >> ch) {
        token_end += 1;
        switch (ch) {
            case '{':
                tokens.push_back(TokenNode(T_LCURLY, "{", line_number, token_end));
                break;
            case '}':
                tokens.push_back(TokenNode(T_RCURLY, "}", line_number, token_end));
                break;
            case '(':
                tokens.push_back(TokenNode(T_LPAREN, "(", line_number, token_end));
                break;
            case ')':
                tokens.push_back(TokenNode(T_RPAREN, ")", line_number, token_end));
                break;
            case ',':
                tokens.push_back(TokenNode(T_COMMA, ",", line_number, token_end));
                break;
            case ';':
                tokens.push_back(TokenNode(T_SEMICOLON, ";", line_number, token_end));
                break;
            case '=':
                infile >> noskipws >> ch;
                if (ch == '=') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_EQ, "==", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_ASSIGN, "=", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '\"': // match strings
                buffer = "";
                infile >> noskipws >> ch;
                token_end += 1;

                while (ch != '\"' && !infile.eof()) {
                    buffer += ch;
                    infile >> noskipws >> ch;
                    token_end += 1;
                    if (ch == '\\' && infile.peek() == '\"') { // process \" string escapes
                        buffer += ch;
                        infile >> noskipws >> ch; token_end += 1;
                        buffer += ch;
                        infile >> noskipws >> ch; token_end += 1;
                    }
                }
                tokens.push_back(TokenNode(T_STR_LIT, buffer, line_number, token_end));
                break;
            case '/':
                infile >> noskipws >> ch;
                if (ch == '/') { // match comment
                    buffer = "/"; token_end += 1;
                    while (ch != '\n' && !infile.eof()) {
                        buffer += ch;
                        infile >> noskipws >> ch; token_end += 1;
                    }
                    tokens.push_back(TokenNode(T_COMMENT, buffer, line_number, token_end));
                    line_number++; // move count to next line
                    token_end = 0;
                } else if (ch == '=') { // /=
                    token_end += 1;
                    tokens.push_back(TokenNode(T_SLASH_ASSIGN, "/=", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_SLASH, "/", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '!':
                infile >> noskipws >> ch;
                if (ch == '=') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_NEQ, "!=", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_NEGATION, "!", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '&':
                infile >> noskipws >> ch;
                if (ch == '&') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_AND, "&&", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_BITW_AND, "&", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '|':
                infile >> noskipws >> ch;
                if (ch == '|') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_OR, "||", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_BITW_OR, "|", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '*':
                infile >> noskipws >> ch;
                if (ch == '=') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_STAR_ASSIGN, "*=", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_STAR, "*", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '>':
                infile >> noskipws >> ch;
                if (ch == '=') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_GEQ, ">=", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_GT, ">", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '<':
                infile >> noskipws >> ch;
                if (ch == '=') {
                    token_end += 1;
                    tokens.push_back(TokenNode(T_LEQ, "<=", line_number, token_end));
                } else {
                    tokens.push_back(TokenNode(T_LT, "<", line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                }
                break;
            case '\'': // match a character literal i.e. 'q'
                infile >> noskipws >> ch; token_end += 1;
                tokens.push_back(TokenNode(T_CHAR_LIT, ch == '\'' ? "" : string(1, ch), line_number, token_end));
                if (infile.peek() == '\'') {// else char literal isn't properly closed
                    infile >> noskipws >> ch; // consume closing ' and discard it
                    token_end += 1;
                }
                break;
            default:
                if (isalpha(ch) || ch == '_') { // match identifier or keyword or boolean literal
                    buffer = ch;
                    infile >> noskipws >> ch; token_end += 1;
                    while ((isalnum(ch) || ch == '_') && !infile.eof()) {
                        buffer += ch;
                        infile >> noskipws >> ch; token_end += 1;
                    }

                    int tokenType = (buffer == "true" || buffer == "false") ? T_BOOL_LIT : T_IDENTIFIER;
                    for (int i = T_INT; i <= T_PRINT; i++) {
                        if (buffer == tokenString[i]) {
                            tokenType = i;
                            break;
                        }
                    }
                    tokens.push_back(TokenNode(tokenType, buffer, line_number, token_end));
                    infile.putback(ch); // putback unused character to file stream
                } else if (isdigit(ch)) { // match non-signed number (float or int)
                    buffer = ch + scanNumber();
                    token_end += buffer.size() - 1;
                    size_t point = buffer.find_first_of(".");
                    if (point == string::npos) // integer
                        tokens.push_back(TokenNode(T_INT_LIT, buffer, line_number, token_end));
                    else // float
                        tokens.push_back(TokenNode(T_FLOAT_LIT, buffer, line_number, token_end));
                } else if (ch == '+' || ch == '-') {
                    if (isdigit(infile.peek())) { // match signed number (float or int)
                        buffer = scanNumber();
                        token_end += buffer.size() - 1;
                        size_t point = buffer.find_first_of(".");
                        TokenNode last = tokens.back();
                        if ((last.token >= T_PLUS && last.token <= T_SLASH) || (last.token >= T_ASSIGN && last.token <= T_SLASH_ASSIGN)) {
                            if (point == string::npos) // integer
                                tokens.push_back(TokenNode(T_INT_LIT, ch + buffer, line_number, token_end));
                            else // float
                                tokens.push_back(TokenNode(T_FLOAT_LIT, ch + buffer, line_number, token_end));
                        } else {
                            tokens.push_back(TokenNode(ch == '+' ? T_PLUS : T_MINUS, ch == '+' ? "+" : "-", line_number, token_end));
                            if (point == string::npos) {
                                tokens.push_back(TokenNode(T_INT_LIT, buffer, line_number, token_end));
                            } else {
                                tokens.push_back(TokenNode(T_INT_LIT, buffer, line_number, token_end));
                            }
                        }
                    } else if (infile.peek() == ch && (infile.peek() == '+' | infile.peek() == '-')) { // match unary ++ or --
                        tokens.push_back(TokenNode(infile.peek() == '+' ? T_UNARY_PLUS : T_UNARY_MINUS, infile.peek() == '+' ? "++" : "--", line_number, token_end));
                        infile >> noskipws >> ch; // consume last + or - and discard it
                        token_end += 1;
                    } else {
                        if (infile.peek() == '=') {
                            tokens.push_back(TokenNode(ch == '+' ? T_PLUS_ASSIGN : T_MINUS_ASSIGN, ch == '+' ? "+=" : "-=", line_number, token_end));
                            infile >> noskipws >> ch; // consume = and discard it
                            token_end += 1;
                        } else
                            tokens.push_back(TokenNode(ch == '+' ? T_PLUS : T_MINUS, ch == '+' ? "+" : "-", line_number, token_end));
                    }
                } else {
                    if (ch == '\n') {
                        line_number++; // move count to next line
                        token_end = 0;
                    } else if (!(ch == ' ' || ch == '\t' || ch == '\r')) // ignore whitespace
                        cerr << "Error [line " << line_number + 1 << "]: Unexpected character " << ch << endl;
                }
        }
    }

    // close the file
    infile.close();

    vector<string> lines = {};
    ifstream file(file_name);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line))
            lines.push_back(line);
        file.close();
    }

    return {tokens, lines};
}

#endif