#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include "scanner.hpp"
#include "defs.hpp"

using namespace std;

void parse(vector<TokenNode> tokens, vector<string> lines) {
    stack<TokenNode> input;
    stack<string> rules;
    string rule;

    input.push(TokenNode(-1, "$", -1, -1));
    reverse(tokens.begin(), tokens.end());
    for (TokenNode node: tokens)
        input.push(node);

    rules.push("$");
    rules.push("OUTER_STMTS");

    while (input.size() > 0 && rules.size() > 0) {
        rule = rules.top(); rules.pop();
        if (table.find(rule) == table.end()) { // nonterminal

        } else {

        }
    }
}

#endif