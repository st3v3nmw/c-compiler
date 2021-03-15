#ifndef DEFS_H
#define DEFS_H

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
    T_COMMENT // not really a token, but put here for "pedagogical" reasons, will be discarded in next phase of compilation
};

// string representation of token types above
std::string tokenString[] = {
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

#endif