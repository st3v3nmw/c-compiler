#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "scanner.hpp"
#include "defs.hpp"

using namespace std;

void print_input_stack(stack<TokenNode> tempinput) {
    if (!tempinput.empty()) {
        cout << "Input: ";
        while (!tempinput.empty()) {
            cout << tempinput.top().value << " ";
            tempinput.pop();
        }
        cout << endl;
    }
}

void print_rules_stack(stack<string> tempinput) {
    if (!tempinput.empty()) {
        cout << "Rules stack: ";
        while (!tempinput.empty()) {
            cout << tempinput.top() << " ";
            tempinput.pop();
        }
        cout << endl;
    }
}

void parse(vector<TokenNode> tokens, vector<string> lines) {
    stack<TokenNode> input;
    stack<TokenNode> tempinput;
    stack<string> rules;
    string rule, former_rule, curr_inp;
    vector<string> next;

    // add $ as end of input in input-stack
    input.push(TokenNode(T_EOF, "$", 0, 0));
    reverse(tokens.begin(), tokens.end());
    for (TokenNode node: tokens) {
        // discard T_COMMENT tokens since they're not useful here
        if (node.token != T_COMMENT)
            input.push(node);
    }

    // add $ as end of input in rules-stack
    rules.push("$");
    rules.push("OUTER_STMTS");

    cout << "---" << endl;
    while (rules.size() > 0 && input.size() > 0) {
        former_rule = rule;
        rule = rules.top(); rules.pop();
        TokenNode curr_node = input.top();
        // get "formal" string representation of token
        curr_inp = formalTokenString[curr_node.token];

        // we reached end of input having matched everything successfully
        if (rule == "$" && curr_node.value == "$") {
            cout << "Token stream parsed successfully!" << endl;
            break;
        }

        // pick next productions to expand to from current production/rule
        next = table[rule][curr_inp];
        // there's nothing to expand to (might be a non-terminal, error or nullable production/rule)
        if (next.size() == 0) {
            if (rule == curr_inp) { // non-terminal matches on both stacks
                input.pop();
                cout << "Matched " << curr_inp << " " << curr_node.value << endl;
                print_input_stack(input);
            } else if (nullable.find(rule) != nullable.end()) { // nullable production/rule
                cout << "Setting " << rule << " to the null string" << endl;
            } else { // error
                print_rules_stack(rules);

                if (rule == "$")
                    rule = "OUTER_STMTS";

                cerr << "\n" << lines[curr_node.line_number] << endl;
                cerr << string(curr_node.token_end - curr_node.value.size(), ' ') << string(curr_node.value.size(), '^') << endl;
                cerr << "Error [line " << curr_node.line_number + 1 << "]: Expected "; 
                
                int n = first[rule].size();
                if (n > 0) { // non-terminal on rules-stack top
                    for (int i = 0; i < n; i++) {
                        if (i != n - 1)
                            cerr << "`" << tokenString[first[rule][i]] << "`, ";
                        else
                            cerr << "or `" << tokenString[first[rule][i]] << "`.";
                    }
                } else // terminal on rules-stack top
                    cerr << "`" << tokenString[tokenEnumStr[rule]] << "`.";
                cerr << " Found `" << curr_node.value << "`." << endl;

                exit(0);
            }
        } else {
            print_rules_stack(rules);
            cout << "Applying production: " << rule << " -> ";

            for (string s: next)
                cout << s << " ";
            cout << endl;

            print_input_stack(input);

            reverse(next.begin(), next.end());
            // add all the parts of the production we're expanding to
            for (string s: next)
                rules.push(s);
        }
        cout << "---" << endl;
    }
}

#endif