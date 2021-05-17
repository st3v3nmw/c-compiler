#pragma once
#ifndef DECL_H
#define DECL_H

#include "defs.hpp"

string scanNumber();
pair<vector<TokenNode>, vector<string>> scan(const string &file_name);

shared_ptr<ASTNode> parse(vector<TokenNode> tokens, vector<string> lines);

int allocateRegister();
inline void addToDataSegment(string type, string identifier);
inline string& select(FnMode mode);
inline void coerce(string &type1, int &reg1, string &type2, int &reg2);
void genIntermediateCode(shared_ptr<ASTNode> root);

#endif