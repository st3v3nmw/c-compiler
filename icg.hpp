#pragma once
#ifndef ICG_H
#define ICG_H

#include <fstream>
#include <string>
#include <map>
#include "defs.hpp"

string data_segment = ".data\n\tnewline: .asciiz \"\\n\"\n", text_segment = ".text\n";

// keep track of free registers
bool freeReg[8] = {true, true, true, true, true, true, true, true};

int allocateRegister() {
    for (int i = 0; i < 8; i++) {
        if (freeReg[i]) {
            freeReg[i] = false;
            return i;
        }
    }
    cout << "No more available registers\n";
    exit(2);
}

int label = 0;

map<string, string> operations = { {"<", "slt"}, {"<=", "sle"}, {">", "sgt"}, {">=", "sge"}, {"==", "seq"}, {"!=", "sne"}, {"&&", "and"}, {"||", "or"}};

inline void addToDataSegment(string type, string identifier) {
    data_segment += "\t" + identifier + ": ";
    if (type == "T_INT" || type == "T_BOOL")
        data_segment += ".word 0";
    else if (type == "T_FLOAT")
        data_segment += ".float";
    else if (type == "T_CHAR")
        data_segment += ".byte 1";
    data_segment += "\n";
}

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

        int genIntermediateCode() {
            if (isTerminal) {
                int reg = allocateRegister();
                if (token.token == T_IDENTIFIER)
                    text_segment += "\tlw $t" + to_string(reg) + ", " + token.value + "\n";
                else
                    text_segment += "\tli $t" + to_string(reg) + ", " + token.value + "\n";
                return reg;
            }

            if (rule == "OUTER_STMTS") {
                string type = children[0]->children[0]->rule;
                string identifier = children[1]->token.value;
                if (children[2]->children[0]->rule == "FN") { // function
                    if (identifier == "main")
                        text_segment += "\n.globl main\n";
                    text_segment += children[1]->token.value + ":\n";
                    children[2]->children[0]->children[4]->genIntermediateCode();
                    // handle opt_return
                } else { // global variable declaration or assignment
                    addToDataSegment(type, identifier);
                    if (!children[2]->children[0]->isNulled) {
                        int reg = children[2]->children[0]->children[1]->genIntermediateCode();
                        text_segment += "\tsw $t" + to_string(reg) + ", " + identifier + "\n";
                        freeReg[reg] = true;
                    }
                }

                if (!children[3]->isNulled)
                    children[3]->genIntermediateCode();
            } else if (rule == "INNER_STMTS") {
                children[0]->genIntermediateCode();
                if (!children[1]->isNulled)
                    children[1]->genIntermediateCode();
            } else if (rule == "INNER_STMT") {
                if (children[0]->rule == "DECL_INIT") {
                    string type = children[0]->children[0]->children[0]->rule;
                    string identifier = children[0]->children[1]->token.value;
                    addToDataSegment(type, identifier);
                    if (!children[0]->children[2]->isNulled) {
                        int reg = children[0]->children[2]->children[1]->genIntermediateCode();
                        text_segment += "\tsw $t" + to_string(reg) + ", " + identifier + "\n";
                        freeReg[reg] = true;
                    }
                } else if (children[0]->rule == "T_PRINT") {
                    int reg = children[2]->genIntermediateCode();
                    text_segment += "\tli $v0, 1\n\tmove $a0, $t" + to_string(reg) + "\n\tsyscall\n";
                    text_segment += "\tli $v0, 4\n\tla $a0, newline\n\tsyscall\n"; // print newline
                    freeReg[reg] = true;
                } else if (children[0]->rule == "IF_STMT") {
                    int reg = children[0]->children[2]->genIntermediateCode(); // expression result
                    label += 1;
                    text_segment += "\tbeq $t" + to_string(reg) + ", $0, L" + to_string(label) + "\n";
                    freeReg[reg] = true;
                    children[0]->children[5]->genIntermediateCode();
                    text_segment += "L" + to_string(label) + ":\n";
                    if (!children[0]->children[7]->isNulled) { // has an else block
                        shared_ptr<ASTNode> elseBlock = children[0]->children[7]->children[1];
                        if (elseBlock->children[0]->rule == "T_IF") {
                            int reg = elseBlock->children[2]->genIntermediateCode();
                            label += 1;
                            text_segment += "\tbeq $t" + to_string(reg) + ", $0, L" + to_string(label) + "\n";
                            freeReg[reg] = true;
                            elseBlock->children[5]->genIntermediateCode();
                            text_segment += "L" + to_string(label) + ":\n";
                        } else
                            elseBlock->children[1]->genIntermediateCode();
                    }
                } else if (children[0]->rule == "LOOP")
                    children[0]->genIntermediateCode();
            } else if (rule == "EXPR") {
                if (children[0]->rule == "ADD") {
                    int reg1 = children[0]->genIntermediateCode();
                    if (!children[1]->isNulled) {
                        int reg2 = children[1]->children[1]->genIntermediateCode();
                        int reg3 = allocateRegister();
                        string op = children[1]->children[0]->children[0]->token.value;
                        text_segment += "\t" + operations[op] + " $t" + to_string(reg3) + ", $t" + to_string(reg1) + ", $t" + to_string(reg2) + "\n";
                        freeReg[reg1] = true;
                        freeReg[reg2] = true;
                        reg1 = reg3;
                    }
                    return reg1;
                } else { // unary negation
                    int reg1 = children[1]->genIntermediateCode();
                    int reg2 = allocateRegister();
                    text_segment += "neg $t" + to_string(reg2) + ", $t" + to_string(reg1) + "\n";
                    freeReg[reg1] = true;
                    return reg2;
                }
            } else if (rule == "ADD") {
                int reg1 = children[0]->genIntermediateCode();
                shared_ptr<ASTNode> addp = children[1];
                while (!addp->isNulled) {
                    int reg2 = addp->children[1]->genIntermediateCode();
                    int reg3 = allocateRegister();
                    if (addp->children[0]->children[0]->rule == "T_PLUS") // addition
                        text_segment += "\tadd $t" + to_string(reg3) + ", $t" + to_string(reg1) + ", $t" + to_string(reg2) + "\n";
                    else // subtraction
                        text_segment += "\tsub $t" + to_string(reg3) + ", $t" + to_string(reg1) + ", $t" + to_string(reg2) + "\n";
                    addp = addp->children[2];
                    freeReg[reg1] = true;
                    freeReg[reg2] = true;
                    reg1 = reg3;
                }
                return reg1;
            } else if (rule == "MULT") {
                int reg1 = children[0]->genIntermediateCode();
                shared_ptr<ASTNode> multp = children[1];
                while (!multp->isNulled) {
                    int reg2 = multp->children[1]->genIntermediateCode();
                    int reg3 = allocateRegister();
                    if (multp->children[0]->children[0]->rule == "T_STAR") // multiplication
                        text_segment += "\tmul $t" + to_string(reg3) + ", $t" + to_string(reg1) + ", $t" + to_string(reg2) + "\n";
                    else // division
                        text_segment += "\tdiv $t" + to_string(reg3) + ", $t" + to_string(reg1) + ", $t" + to_string(reg2) + "\n";
                    multp = multp->children[2];
                    freeReg[reg1] = true;
                    freeReg[reg2] = true;
                    reg1 = reg3;
                }
                return reg1;
            } else if (rule == "LOOP") {

            } else if (rule == "TERM") {
                if (children[0]->rule == "CONST")
                    return children[0]->genIntermediateCode();
                else if (children[0]->rule == "T_LPAREN") // ( EXPR )
                    return children[1]->genIntermediateCode();
                else { // function call
                    return children[0]->genIntermediateCode();
                }
            } else {
                return children[0]->genIntermediateCode();
            }
        }
};

void genIntermediateCode(shared_ptr<ASTNode> root) {
    root->genIntermediateCode();
    text_segment += "\tli $v0, 10\n\tsyscall\n";

    ofstream file("out.asm");
    if (file.is_open()) {
        file << data_segment << "\n" << text_segment;
        file.close();
    }
}

#endif