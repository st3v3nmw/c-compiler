#pragma once
#ifndef DEFS_H
#define DEFS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// keep track of token types
enum {
  T_EOF,
  T_INT,
  T_FLOAT,
  T_CHAR,
  T_BOOL,
  T_VOID,
  T_RETURN,
  T_BREAK,
  T_CONTINUE,
  T_FOR,
  T_WHILE,
  T_IF,
  T_ELSE,
  T_PRINT, // keywords
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_UNARY_PLUS,
  T_UNARY_MINUS,
  T_BITW_AND,
  T_BITW_OR, // arithmetic operators
  T_NEGATION,
  T_AND,
  T_OR, // logical operators
  T_EQ,
  T_NEQ,
  T_LT,
  T_GT,
  T_LEQ,
  T_GEQ, // relational operators
  T_ASSIGN,
  T_PLUS_ASSIGN,
  T_MINUS_ASSIGN,
  T_STAR_ASSIGN,
  T_SLASH_ASSIGN, // assignment operators
  T_LCURLY,
  T_RCURLY,
  T_COMMA,
  T_LPAREN,
  T_RPAREN,
  T_SEMICOLON, // separators
  T_INT_LIT,
  T_FLOAT_LIT,
  T_STR_LIT,
  T_CHAR_LIT,
  T_BOOL_LIT,
  T_IDENTIFIER, // literals & identifier
  T_COMMENT // not really a token, but put here for "pedagogical" reasons, will
            // be discarded in next phase of compilation
};

// AST tokens
enum {
  A_ADD,
  A_MINUS,
  A_MULTIPLY,
  A_DIVIDE,
  A_ASSIGN,
  A_FUNCTION,
  A_FNCALL,
  A_IF,
  A_WHILE,
  A_BREAK,
  A_CONTINUE,
};

// string representation of token types above
extern string tokenString[];

// cause C++ doesn't support enum -> string conversion, ¯\_(ツ)_/¯
extern string formalTokenString[];

// cause C++ doesn't support string -> enum conversion, ¯\_(ツ)_/¯
extern unordered_map<string, int> tokenEnumStr;

// LL(1) grammar
extern unordered_map<string, vector<vector<string>>> grammar;

// nullable productions in grammar
extern unordered_set<string> nullable;

// FIRST sets
extern unordered_map<string, vector<int>> first;

// parser predictive table
extern unordered_map<string, unordered_map<string, vector<string>>> table;

/*
    The TokenNode class holds the information about scanned tokens.
    i.e. Token type and Token value
    Example: 1. for a string "abcd", the token type is T_STR_LIT and value =
   "abcd"
             2. for a keyword "float", the token type is T_KEYWORD and the value
   = "float"
*/
class TokenNode {
public:
  int token, line_number, token_end;
  string value;

  TokenNode() {}
  TokenNode(int token, string value, int line_number, int token_end);

  // print a string representation of the token
  void print();
};

class ASTNode {
public:
  vector<shared_ptr<ASTNode>> children;
  int astType;

  string rule;
  TokenNode token;
  bool isNulled, isTerminal;

  ASTNode(string rule);

  void print(int level);
  pair<int, string> genIntermediateCode();
};

extern bool freeReg[];
extern unordered_map<string, string> symbolTable;
enum FnMode { M_MAIN, M_COMPUTE_GLOBALS, M_OTHER };

#endif