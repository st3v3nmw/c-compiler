#pragma once
#ifndef DECL_H
#define DECL_H

#include "defs.hpp"

pair<vector<TokenNode>, vector<string>> scan(const string &file_name);

void genIntermediateCode(shared_ptr<ASTNode> root);

#endif