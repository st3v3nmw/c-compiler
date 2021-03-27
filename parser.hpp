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

    // add $ as end of input in input stack
    input.push(TokenNode(T_EOF, "$", -1, -1));
    reverse(tokens.begin(), tokens.end());
    for (TokenNode node: tokens) {
        // discard T_COMMENT tokens since they're not useful here
        if (node.token != T_COMMENT)
            input.push(node);
    }

    // add $ as end of input in rules stack
    rules.push("$");
    rules.push("OUTER_STMTS");

    while (input.size() > 0 && rules.size() > 0) {
        rule = rules.top(); rules.pop();
        TokenNode curr_node = input.top();
        // get "formal" string representation of token
        curr_inp = formalTokenString[curr_node.token];

        // we reached end of input having matched everything successfully
        if (rule == "$" && curr_node.value == "$") {
            cout << "Matched successfully!" << endl;
            break;
        }

        // pick next productions to expand to from current production/rule
        next = table[rule][curr_inp];
        // there's nothing to expand to (might be a nonterminal, error or nullable production/rule)
        if (next.size() == 0) {
            if (rule == curr_inp) { // non terminal matches on both stacks
                input.pop();
                cout << "Matched " << curr_inp << " " << curr_node.value << endl;
            } else if (nullable.find(rule) != nullable.end()) { // nullable production/rule
                cout << "Setting " << rule << " to Ɛ" << endl;
            } else { // error
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
            // add all the parts of the production we're expanding to
            for (string s: next)
                rules.push(s);
        }
    }
}

#endif