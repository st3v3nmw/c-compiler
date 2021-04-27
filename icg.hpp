#pragma once
#ifndef ICG_H
#define ICG_H

class ASTNode {
    public:
        vector<shared_ptr<ASTNode>> children;
        int astType;

        string rule;
        TokenNode token;
        bool isNulled, isTerminal;

        ASTNode(string rule) {
            this->rule = rule;
            isTerminal = false;
            isNulled = false;
        }

        void print(int level = 0) {
            if (isTerminal) {
                if (!(token.token >= T_LCURLY && token.token <= T_SEMICOLON))
                    cout << string(level, ' ') << rule << " -> " << token.value << endl;
            } else {
                cout << string(level, ' ') << rule << endl;
                for (shared_ptr<ASTNode> child: children) {
                    if (!child->isNulled)
                        child->print(level + 2);
                }
            }
        }
};

#endif