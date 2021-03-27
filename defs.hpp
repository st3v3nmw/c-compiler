#pragma once
#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <unordered_map>
#include <array>
#include <unordered_set>

using namespace std;

// keep track of token types
enum {
    T_EOF,
    T_INT, T_FLOAT, T_CHAR, T_BOOL, T_VOID, T_RETURN, T_BREAK, T_CONTINUE, T_FOR, T_WHILE, T_IF, T_ELSE, T_PRINT, // keywords
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_UNARY_PLUS, T_UNARY_MINUS, T_BITW_AND, T_BITW_OR, // arithmetic operators
    T_NEGATION, T_AND, T_OR, // logical operators
    T_EQ, T_NEQ, T_LT, T_GT, T_LEQ, T_GEQ, // relational operators
    T_ASSIGN, T_PLUS_ASSIGN, T_MINUS_ASSIGN, T_STAR_ASSIGN, T_SLASH_ASSIGN, // assignment operators
    T_LCURLY, T_RCURLY, T_COMMA, T_LPAREN, T_RPAREN, T_SEMICOLON, // separators
    T_INT_LIT, T_FLOAT_LIT, T_STR_LIT, T_CHAR_LIT, T_BOOL_LIT, T_IDENTIFIER, // literals & identifier
    T_COMMENT // not really a token, but put here T_FOR "pedagogical" reasons, will be discarded in next phase of compilation
};

// string representation of token types above
string tokenString[] = {
        "EOF",
        "int", "float", "char", "bool", "void", "return", "break", "continue", "for", "while", "if", "else", "printf",
        "+", "-", "*", "/", "++", "--", "&", "|",
        "!", "&&", "||",
        "==", "!=", "<", ">", "<=", ">=",
        "=", "+=", "-=", "*=", "/=",
        "{", "}", ",", "(", ")", ";",
        "integer literal", "floating-point literal", "string literal", "character literal", "boolean literal", "identifier",
        "comment"
};

// cause C++ doesn't support enum -> string conversion, ¯\_(ツ)_/¯
string formalTokenString[] = {
        "T_EOF",
        "T_INT", "T_FLOAT", "T_CHAR", "T_BOOL", "T_VOID", "T_RETURN", "T_BREAK", "T_CONTINUE", "T_FOR", "T_WHILE", "T_IF", "T_ELSE", "T_PRINT",
        "T_PLUS", "T_MINUS", "T_STAR", "T_SLASH", "T_UNARY_PLUS", "T_UNARY_MINUS", "T_BITW_AND", "T_BITW_OR",
        "T_NEGATION", "T_AND", "T_OR",
        "T_EQ", "T_NEQ", "T_LT", "T_GT", "T_LEQ", "T_GEQ",
        "T_ASSIGN", "T_PLUS_ASSIGN", "T_MINUS_ASSIGN", "T_STAR_ASSIGN", "T_SLASH_ASSIGN",
        "T_LCURLY", "T_RCURLY", "T_COMMA", "T_LPAREN", "T_RPAREN", "T_SEMICOLON",
        "T_INT_LIT", "T_FLOAT_LIT", "T_STR_LIT", "T_CHAR_LIT", "T_BOOL_LIT", "T_IDENTIFIER",
        "T_COMMENT"
};

// LL(1) grammar
unordered_map<string, vector<vector<string>>> grammar = {{"OUTER_STMTS",{{"TSPEC", "T_IDENTIFIER", "OUTER_STMT", "OUTER_STMTS"}}},{"OUTER_STMT",{{"FN"}, {"OPT_ASSIGN", "T_SEMICOLON"}}},{"FN",{{"T_LPAREN", "PL", "T_RPAREN", "T_LCURLY", "INNER_STMTS", "OPT_RETURN", "T_RCURLY"}}},{"PL",{{"TSPEC", "T_IDENTIFIER", "O_PL"}}},{"O_PL",{{"T_COMMA", "PL"}}},{"OPT_RETURN",{{"T_RETURN", "EXPR", "T_SEMICOLON"}}},{"TSPEC",{{"T_INT"}, {"T_FLOAT"}, {"T_CHAR"}, {"T_VOID"}, {"T_BOOL"}}},{"OPT_TSPEC",{{"TSPEC"}}},{"INNER_STMTS",{{"INNER_STMT", "INNER_STMTS"}}},{"INNER_STMT",{{"DECL_INIT", "T_SEMICOLON"}, {"T_IDENTIFIER", "ASSIGN_FNCALL", "T_SEMICOLON"}, {"IF_STMT"}, {"LOOP"}, {"T_PRINT", "T_LPAREN", "ARGS", "T_RPAREN", "T_SEMICOLON"}}},{"DECL_INIT",{{"TSPEC", "T_IDENTIFIER", "OPT_ASSIGN"}}},{"OPT_ASSIGN",{{"ASSIGN_OP", "EXPR"}}},{"ASSIGN_FNCALL",{{"T_LPAREN", "ARGS", "T_RPAREN"}, {"ASSIGN_P"}}},{"ASSIGN_P",{{"ASSIGN_OP", "EXPR"}, {"T_UNARY_PLUS"}, {"T_UNARY_MINUS"}}},{"ASSIGN_OP",{{"T_ASSIGN"}, {"T_PLUS_ASSIGN"}, {"T_MINUS_ASSIGN"}, {"T_STAR_ASSIGN"}, {"T_SLASH_ASSIGN"}}},{"IF_STMT",{{"T_IF", "T_LPAREN", "EXPR", "T_RPAREN", "T_LCURLY", "INNER_STMTS", "T_RCURLY", "ELSE"}}},{"ELSE",{{"T_ELSE", "O_ELSE"}}},{"O_ELSE",{{"T_IF", "T_LPAREN", "EXPR", "T_RPAREN", "T_LCURLY", "INNER_STMTS", "T_RCURLY"}, {"T_LCURLY", "INNER_STMTS", "T_RCURLY"}}},{"LOOP",{{"T_WHILE", "T_LPAREN", "EXPR", "T_RPAREN", "T_LCURLY", "LOOP_STMTS", "T_RCURLY"}, {"T_FOR", "T_LPAREN", "OPT_TSPEC", "FOR_P", "T_SEMICOLON", "OPT_EXPR", "T_SEMICOLON", "FOR_P", "T_RPAREN", "T_LCURLY", "LOOP_STMTS", "T_RCURLY"}}},{"FOR_P",{{"T_IDENTIFIER", "ASSIGN_P"}}},{"LOOP_STMTS",{{"INNER_STMT", "LOOP_STMTS"}, {"T_BREAK", "T_SEMICOLON", "LOOP_STMTS"}, {"T_CONTINUE", "T_SEMICOLON", "LOOP_STMTS"}}},{"ARGS",{{"EXPR", "OPT_ARGS"}}},{"OPT_ARGS",{{"T_COMMA", "ARGS"}}},{"EXPR",{{"ADD", "EXPR_P"}, {"T_NEGATION", "EXPR"}}},{"EXPR_P",{{"O_OP", "EXPR"}}},{"OPT_EXPR",{{"EXPR"}}},{"ADD",{{"MULT", "ADD_P"}}},{"ADD_P",{{"ADD_OP", "MULT", "ADD_P"}}},{"MULT",{{"TERM", "MULT_P"}}},{"MULT_P",{{"MULT_OP", "TERM", "MULT_P"}}},{"ADD_OP",{{"T_PLUS"}, {"T_MINUS"}}},{"MULT_OP",{{"T_STAR"}, {"T_SLASH"}}},{"O_OP",{{"T_LT"}, {"T_LEQ"}, {"T_GT"}, {"T_GEQ"}, {"T_EQ"}, {"T_NEQ"}, {"T_AND"}, {"T_OR"}, {"T_BITW_AND"}, {"T_BITW_OR"}}},{"TERM",{{"T_LPAREN", "EXPR", "T_RPAREN"}, {"CONST"}, {"T_IDENTIFIER"}}},{"CONST",{{"T_INT_LIT"}, {"T_FLOAT_LIT"}, {"T_STR_LIT"}, {"T_CHAR_LIT"}, {"T_BOOL_LIT"}}},{"S",{{"OUTER_STMTS", "$"}}}};

// nullable productions in grammar
unordered_set<string> nullable = { "OUTER_STMTS", "PL", "O_PL", "OPT_RETURN", "INNER_STMTS", "OPT_ASSIGN", "ELSE", "LOOP_STMTS", "ARGS", "OPT_ARGS", "EXPR_P", "OPT_EXPR", "ADD_P", "MULT_P", "OPT_TSPEC", "FOR_P"};

// parser predictive table
unordered_map<string, unordered_map<string, vector<string>>> table = {{"S",{{"$",{"OUTER_STMTS", "$"}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"OUTER_STMTS", "$"}},{"T_FLOAT",{"OUTER_STMTS", "$"}},{"T_CHAR",{"OUTER_STMTS", "$"}},{"T_VOID",{"OUTER_STMTS", "$"}},{"T_BOOL",{"OUTER_STMTS", "$"}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"OUTER_STMTS",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"TSPEC", "T_IDENTIFIER", "OUTER_STMT", "OUTER_STMTS"}},{"T_FLOAT",{"TSPEC", "T_IDENTIFIER", "OUTER_STMT", "OUTER_STMTS"}},{"T_CHAR",{"TSPEC", "T_IDENTIFIER", "OUTER_STMT", "OUTER_STMTS"}},{"T_VOID",{"TSPEC", "T_IDENTIFIER", "OUTER_STMT", "OUTER_STMTS"}},{"T_BOOL",{"TSPEC", "T_IDENTIFIER", "OUTER_STMT", "OUTER_STMTS"}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"OUTER_STMT",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{"OPT_ASSIGN", "T_SEMICOLON"}},{"T_LPAREN",{"FN"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{"OPT_ASSIGN", "T_SEMICOLON"}},{"T_PLUS_ASSIGN",{"OPT_ASSIGN", "T_SEMICOLON"}},{"T_MINUS_ASSIGN",{"OPT_ASSIGN", "T_SEMICOLON"}},{"T_STAR_ASSIGN",{"OPT_ASSIGN", "T_SEMICOLON"}},{"T_SLASH_ASSIGN",{"OPT_ASSIGN", "T_SEMICOLON"}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"FN",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{"T_LPAREN", "PL", "T_RPAREN", "T_LCURLY", "INNER_STMTS", "OPT_RETURN", "T_RCURLY"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"PL",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"TSPEC", "T_IDENTIFIER", "O_PL"}},{"T_FLOAT",{"TSPEC", "T_IDENTIFIER", "O_PL"}},{"T_CHAR",{"TSPEC", "T_IDENTIFIER", "O_PL"}},{"T_VOID",{"TSPEC", "T_IDENTIFIER", "O_PL"}},{"T_BOOL",{"TSPEC", "T_IDENTIFIER", "O_PL"}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"O_PL",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{"T_COMMA", "PL"}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"OPT_RETURN",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{"T_RETURN", "EXPR", "T_SEMICOLON"}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"TSPEC",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"T_INT"}},{"T_FLOAT",{"T_FLOAT"}},{"T_CHAR",{"T_CHAR"}},{"T_VOID",{"T_VOID"}},{"T_BOOL",{"T_BOOL"}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"OPT_TSPEC",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"TSPEC"}},{"T_FLOAT",{"TSPEC"}},{"T_CHAR",{"TSPEC"}},{"T_VOID",{"TSPEC"}},{"T_BOOL",{"TSPEC"}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"INNER_STMTS",{{"$",{}},{"T_IDENTIFIER",{"INNER_STMT", "INNER_STMTS"}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"INNER_STMT", "INNER_STMTS"}},{"T_FLOAT",{"INNER_STMT", "INNER_STMTS"}},{"T_CHAR",{"INNER_STMT", "INNER_STMTS"}},{"T_VOID",{"INNER_STMT", "INNER_STMTS"}},{"T_BOOL",{"INNER_STMT", "INNER_STMTS"}},{"T_PRINT",{"INNER_STMT", "INNER_STMTS"}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{"INNER_STMT", "INNER_STMTS"}},{"T_ELSE",{}},{"T_WHILE",{"INNER_STMT", "INNER_STMTS"}},{"T_FOR",{"INNER_STMT", "INNER_STMTS"}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"INNER_STMT",{{"$",{}},{"T_IDENTIFIER",{"T_IDENTIFIER", "ASSIGN_FNCALL", "T_SEMICOLON"}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"DECL_INIT", "T_SEMICOLON"}},{"T_FLOAT",{"DECL_INIT", "T_SEMICOLON"}},{"T_CHAR",{"DECL_INIT", "T_SEMICOLON"}},{"T_VOID",{"DECL_INIT", "T_SEMICOLON"}},{"T_BOOL",{"DECL_INIT", "T_SEMICOLON"}},{"T_PRINT",{"T_PRINT", "T_LPAREN", "ARGS", "T_RPAREN", "T_SEMICOLON"}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{"IF_STMT"}},{"T_ELSE",{}},{"T_WHILE",{"LOOP"}},{"T_FOR",{"LOOP"}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"DECL_INIT",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"TSPEC", "T_IDENTIFIER", "OPT_ASSIGN"}},{"T_FLOAT",{"TSPEC", "T_IDENTIFIER", "OPT_ASSIGN"}},{"T_CHAR",{"TSPEC", "T_IDENTIFIER", "OPT_ASSIGN"}},{"T_VOID",{"TSPEC", "T_IDENTIFIER", "OPT_ASSIGN"}},{"T_BOOL",{"TSPEC", "T_IDENTIFIER", "OPT_ASSIGN"}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"OPT_ASSIGN",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_PLUS_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_MINUS_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_STAR_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_SLASH_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"ASSIGN_FNCALL",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{"T_LPAREN", "ARGS", "T_RPAREN"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{"ASSIGN_P"}},{"T_UNARY_MINUS",{"ASSIGN_P"}},{"T_ASSIGN",{"ASSIGN_P"}},{"T_PLUS_ASSIGN",{"ASSIGN_P"}},{"T_MINUS_ASSIGN",{"ASSIGN_P"}},{"T_STAR_ASSIGN",{"ASSIGN_P"}},{"T_SLASH_ASSIGN",{"ASSIGN_P"}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"ASSIGN_P",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{"T_UNARY_PLUS"}},{"T_UNARY_MINUS",{"T_UNARY_MINUS"}},{"T_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_PLUS_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_MINUS_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_STAR_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_SLASH_ASSIGN",{"ASSIGN_OP", "EXPR"}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"ASSIGN_OP",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{"T_ASSIGN"}},{"T_PLUS_ASSIGN",{"T_PLUS_ASSIGN"}},{"T_MINUS_ASSIGN",{"T_MINUS_ASSIGN"}},{"T_STAR_ASSIGN",{"T_STAR_ASSIGN"}},{"T_SLASH_ASSIGN",{"T_SLASH_ASSIGN"}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"IF_STMT",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{"T_IF", "T_LPAREN", "EXPR", "T_RPAREN", "T_LCURLY", "INNER_STMTS", "T_RCURLY", "ELSE"}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"ELSE",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{"T_ELSE", "O_ELSE"}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"O_ELSE",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{"T_LCURLY", "INNER_STMTS", "T_RCURLY"}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{"T_IF", "T_LPAREN", "EXPR", "T_RPAREN", "T_LCURLY", "INNER_STMTS", "T_RCURLY"}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"LOOP",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{"T_WHILE", "T_LPAREN", "EXPR", "T_RPAREN", "T_LCURLY", "LOOP_STMTS", "T_RCURLY"}},{"T_FOR",{"T_FOR", "T_LPAREN", "OPT_TSPEC", "FOR_P", "T_SEMICOLON", "OPT_EXPR", "T_SEMICOLON", "FOR_P", "T_RPAREN", "T_LCURLY", "LOOP_STMTS", "T_RCURLY"}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"FOR_P",{{"$",{}},{"T_IDENTIFIER",{"T_IDENTIFIER", "ASSIGN_P"}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"LOOP_STMTS",{{"$",{}},{"T_IDENTIFIER",{"INNER_STMT", "LOOP_STMTS"}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{"INNER_STMT", "LOOP_STMTS"}},{"T_FLOAT",{"INNER_STMT", "LOOP_STMTS"}},{"T_CHAR",{"INNER_STMT", "LOOP_STMTS"}},{"T_VOID",{"INNER_STMT", "LOOP_STMTS"}},{"T_BOOL",{"INNER_STMT", "LOOP_STMTS"}},{"T_PRINT",{"INNER_STMT", "LOOP_STMTS"}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{"INNER_STMT", "LOOP_STMTS"}},{"T_ELSE",{}},{"T_WHILE",{"INNER_STMT", "LOOP_STMTS"}},{"T_FOR",{"INNER_STMT", "LOOP_STMTS"}},{"T_BREAK",{"T_BREAK", "T_SEMICOLON", "LOOP_STMTS"}},{"T_CONTINUE",{"T_CONTINUE", "T_SEMICOLON", "LOOP_STMTS"}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"ARGS",{{"$",{}},{"T_IDENTIFIER",{"EXPR", "OPT_ARGS"}},{"T_SEMICOLON",{}},{"T_LPAREN",{"EXPR", "OPT_ARGS"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{"EXPR", "OPT_ARGS"}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"EXPR", "OPT_ARGS"}},{"T_FLOAT_LIT",{"EXPR", "OPT_ARGS"}},{"T_STR_LIT",{"EXPR", "OPT_ARGS"}},{"T_CHAR_LIT",{"EXPR", "OPT_ARGS"}},{"T_BOOL_LIT",{"EXPR", "OPT_ARGS"}}}},{"OPT_ARGS",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{"T_COMMA", "ARGS"}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"EXPR",{{"$",{}},{"T_IDENTIFIER",{"ADD", "EXPR_P"}},{"T_SEMICOLON",{}},{"T_LPAREN",{"ADD", "EXPR_P"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{"T_NEGATION", "EXPR"}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"ADD", "EXPR_P"}},{"T_FLOAT_LIT",{"ADD", "EXPR_P"}},{"T_STR_LIT",{"ADD", "EXPR_P"}},{"T_CHAR_LIT",{"ADD", "EXPR_P"}},{"T_BOOL_LIT",{"ADD", "EXPR_P"}}}},{"EXPR_P",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{"O_OP", "EXPR"}},{"T_LEQ",{"O_OP", "EXPR"}},{"T_GT",{"O_OP", "EXPR"}},{"T_GEQ",{"O_OP", "EXPR"}},{"T_EQ",{"O_OP", "EXPR"}},{"T_NEQ",{"O_OP", "EXPR"}},{"T_AND",{"O_OP", "EXPR"}},{"T_OR",{"O_OP", "EXPR"}},{"T_BITW_AND",{"O_OP", "EXPR"}},{"T_BITW_OR",{"O_OP", "EXPR"}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"OPT_EXPR",{{"$",{}},{"T_IDENTIFIER",{"EXPR"}},{"T_SEMICOLON",{}},{"T_LPAREN",{"EXPR"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{"EXPR"}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"EXPR"}},{"T_FLOAT_LIT",{"EXPR"}},{"T_STR_LIT",{"EXPR"}},{"T_CHAR_LIT",{"EXPR"}},{"T_BOOL_LIT",{"EXPR"}}}},{"ADD",{{"$",{}},{"T_IDENTIFIER",{"MULT", "ADD_P"}},{"T_SEMICOLON",{}},{"T_LPAREN",{"MULT", "ADD_P"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"MULT", "ADD_P"}},{"T_FLOAT_LIT",{"MULT", "ADD_P"}},{"T_STR_LIT",{"MULT", "ADD_P"}},{"T_CHAR_LIT",{"MULT", "ADD_P"}},{"T_BOOL_LIT",{"MULT", "ADD_P"}}}},{"ADD_P",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{"ADD_OP", "MULT", "ADD_P"}},{"T_MINUS",{"ADD_OP", "MULT", "ADD_P"}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"MULT",{{"$",{}},{"T_IDENTIFIER",{"TERM", "MULT_P"}},{"T_SEMICOLON",{}},{"T_LPAREN",{"TERM", "MULT_P"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"TERM", "MULT_P"}},{"T_FLOAT_LIT",{"TERM", "MULT_P"}},{"T_STR_LIT",{"TERM", "MULT_P"}},{"T_CHAR_LIT",{"TERM", "MULT_P"}},{"T_BOOL_LIT",{"TERM", "MULT_P"}}}},{"MULT_P",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{"MULT_OP", "TERM", "MULT_P"}},{"T_SLASH",{"MULT_OP", "TERM", "MULT_P"}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"ADD_OP",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{"T_PLUS"}},{"T_MINUS",{"T_MINUS"}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"MULT_OP",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{"T_STAR"}},{"T_SLASH",{"T_SLASH"}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"O_OP",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{"T_LT"}},{"T_LEQ",{"T_LEQ"}},{"T_GT",{"T_GT"}},{"T_GEQ",{"T_GEQ"}},{"T_EQ",{"T_EQ"}},{"T_NEQ",{"T_NEQ"}},{"T_AND",{"T_AND"}},{"T_OR",{"T_OR"}},{"T_BITW_AND",{"T_BITW_AND"}},{"T_BITW_OR",{"T_BITW_OR"}},{"T_INT_LIT",{}},{"T_FLOAT_LIT",{}},{"T_STR_LIT",{}},{"T_CHAR_LIT",{}},{"T_BOOL_LIT",{}}}},{"TERM",{{"$",{}},{"T_IDENTIFIER",{"T_IDENTIFIER"}},{"T_SEMICOLON",{}},{"T_LPAREN",{"T_LPAREN", "EXPR", "T_RPAREN"}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"CONST"}},{"T_FLOAT_LIT",{"CONST"}},{"T_STR_LIT",{"CONST"}},{"T_CHAR_LIT",{"CONST"}},{"T_BOOL_LIT",{"CONST"}}}},{"CONST",{{"$",{}},{"T_IDENTIFIER",{}},{"T_SEMICOLON",{}},{"T_LPAREN",{}},{"T_RPAREN",{}},{"T_LCURLY",{}},{"T_RCURLY",{}},{"T_COMMA",{}},{"T_RETURN",{}},{"T_INT",{}},{"T_FLOAT",{}},{"T_CHAR",{}},{"T_VOID",{}},{"T_BOOL",{}},{"T_PRINT",{}},{"T_UNARY_PLUS",{}},{"T_UNARY_MINUS",{}},{"T_ASSIGN",{}},{"T_PLUS_ASSIGN",{}},{"T_MINUS_ASSIGN",{}},{"T_STAR_ASSIGN",{}},{"T_SLASH_ASSIGN",{}},{"T_IF",{}},{"T_ELSE",{}},{"T_WHILE",{}},{"T_FOR",{}},{"T_BREAK",{}},{"T_CONTINUE",{}},{"T_NEGATION",{}},{"T_PLUS",{}},{"T_MINUS",{}},{"T_STAR",{}},{"T_SLASH",{}},{"T_LT",{}},{"T_LEQ",{}},{"T_GT",{}},{"T_GEQ",{}},{"T_EQ",{}},{"T_NEQ",{}},{"T_AND",{}},{"T_OR",{}},{"T_BITW_AND",{}},{"T_BITW_OR",{}},{"T_INT_LIT",{"T_INT_LIT"}},{"T_FLOAT_LIT",{"T_FLOAT_LIT"}},{"T_STR_LIT",{"T_STR_LIT"}},{"T_CHAR_LIT",{"T_CHAR_LIT"}},{"T_BOOL_LIT",{"T_BOOL_LIT"}}}}};

#endif