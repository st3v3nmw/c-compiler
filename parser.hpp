#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <memory>
#include "scanner.hpp"
#include "icg.hpp"
#include "defs.hpp"

using namespace std;

ASTNode parse(vector<TokenNode> tokens, vector<string> lines) {
    stack<TokenNode> input;
    stack<TokenNode> tempinput;
    stack<string> rules;
    string rule, former_rule, curr_inp;
    vector<string> next;

    stack<shared_ptr<ASTNode>> nodes;
    shared_ptr<ASTNode> root(new ASTNode("OUTER_STMTS"));
    nodes.push(root);
    shared_ptr<ASTNode> local_root;

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

    while (rules.size() > 0 && input.size() > 0) {
        former_rule = rule;
        rule = rules.top(); rules.pop();
        if (nodes.size() > 0) {
            local_root = nodes.top(); nodes.pop();
        }
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
                local_root->isTerminal = true;
                local_root->token = curr_node;
            } else if (nullable.find(rule) != nullable.end()) { // nullable production/rule
                local_root->isNulled = true;
            } else { // error
                if (rule == "$")
                    rule = "OUTER_STMTS";

                cout << "\n" << lines[curr_node.line_number] << endl;
                cout << string(curr_node.token_end - curr_node.value.size(), ' ') << string(curr_node.value.size(), '^') << endl;
                cout << "Error [line " << curr_node.line_number + 1 << "]: Expected "; 
                
                int n = first[rule].size();
                if (n > 0) { // non-terminal on rules-stack top
                    for (int i = 0; i < n; i++) {
                        if (i != n - 1)
                            cout << "`" << tokenString[first[rule][i]] << "`, ";
                        else
                            cout << "or `" << tokenString[first[rule][i]] << "`.";
                    }
                } else // terminal on rules-stack top
                    cout << "`" << tokenString[tokenEnumStr[rule]] << "`.";
                cout << " Found `" << curr_node.value << "`." << endl;

                exit(0);
            }
        } else {
            reverse(next.begin(), next.end());
            // add all the parts of the production we're expanding to
            for (string s: next) {
                rules.push(s);
                shared_ptr<ASTNode> child(new ASTNode(s));
                local_root->children.insert(local_root->children.begin(), child);
                nodes.push(child);
            }
        }
    }

    cout << "\nParse Tree\n-----------" << endl;
    root->print();

    return *root;
}

#endif