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
    string rule, curr_inp;
    vector<string> next;

    input.push(TokenNode(-1, "$", -1, -1));
    reverse(tokens.begin(), tokens.end());
    for (TokenNode node: tokens) {
        if (node.token != T_COMMENT)
            input.push(node);
    }

    rules.push("$");
    rules.push("OUTER_STMTS");

    while (input.size() > 0 && rules.size() > 0) {
        rule = rules.top(); rules.pop();
        TokenNode curr_node = input.top();
        curr_inp = formalTokenString[curr_node.token];

        if (rule == "$" && curr_node.value == "$") {
            cout << "Matched successfully!" << endl;
            break;
        }

        next = table[rule][curr_inp];
        if (next.size() == 0) {
            if (rule == curr_inp) {
                input.pop();
                cout << "Matched " << curr_inp << " " << curr_node.value << endl;
            } else if (nullable.find(rule) != nullable.end()) {
                cout << "Setting " << rule << " to Ɛ" << endl; 
            } else {
                cout << "Stacks top: " << rule << ", " << curr_inp << " " << curr_node.value << endl;
                cerr << "\n" << lines[curr_node.line_number] << endl;
                cerr << "Couldn't match " << rule << " line " << curr_node.line_number << endl;
                exit(0);
            }
        } else {
            cout << "Stacks top: " << rule << ", " << curr_inp << " " << curr_node.value << endl;
            cout << "Applying production: " << rule << " -> ";
            for (string s: next)
                cout << s << " ";
            cout << endl;

            reverse(next.begin(), next.end());
            for (string s: next)
                rules.push(s);
        }
    }
}

#endif