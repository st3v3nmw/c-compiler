#include "defs.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

string data_segment = ".data\n\tnewline: .asciiz \"\\n\"\n";
string main_text_segment = ".text\n";
// bad attempt at name mangling...
string compute_globals =
    ".globl U3quItLgAXaPHLuf\n.ent U3quItLgAXaPHLuf\nU3quItLgAXaPHLuf:\n";
string other = "";

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

int label = 0, litCount = 0;

unordered_map<string, string> operations = {
    {"<", "slt"},  {"<=", "sle"}, {">", "sgt"},  {">=", "sge"},
    {"==", "seq"}, {"!=", "sne"}, {"&&", "and"}, {"||", "or"}};
unordered_map<string, string> symbolTable = {};

enum FnMode { M_MAIN, M_COMPUTE_GLOBALS, M_OTHER };
FnMode mode = M_COMPUTE_GLOBALS;

inline void addToDataSegment(string type, string identifier) {
  if (symbolTable.find(identifier) != symbolTable.end())
    return;

  symbolTable.insert({identifier, (type == "T_BOOL" ? "T_INT" : type)});
  data_segment += "\t" + identifier + ": ";
  if (type == "T_INT" || type == "T_BOOL")
    data_segment += ".word 0";
  else if (type == "T_FLOAT")
    data_segment += ".float 0.0";
  else if (type == "T_CHAR")
    data_segment += ".byte 1";
  data_segment += "\n";
}

inline string &select(FnMode mode) {
  if (mode == M_MAIN)
    return main_text_segment;
  else if (mode == M_COMPUTE_GLOBALS)
    return compute_globals;
  return other;
}

inline void coerce(string &type1, int &reg1, string &type2, int &reg2) {
  if (type1 == "T_FLOAT" && type2 == "T_INT" ||
      type2 == "T_FLOAT" && type1 == "T_INT") {
    string str_reg = type1 == "T_INT" ? to_string(reg1) : to_string(reg2);
    select(mode) += "\tmtc1 $t" + str_reg + ", $f" + str_reg + "\n";
    select(mode) += "\tcvt.s.w $f" + str_reg + ", $f" + str_reg + "\n";

    type1 = "T_FLOAT";
    type2 = "T_FLOAT";
  }
}

ASTNode::ASTNode(string rule) {
  this->rule = rule;
  isTerminal = false;
  isNulled = false;
}

void ASTNode::print(int level = 0) {
  if (isTerminal) {
    if (!(token.token >= T_LCURLY && token.token <= T_SEMICOLON))
      cout << string(level, ' ') << rule << " -> " << token.value << endl;
  } else {
    cout << string(level, ' ') << rule << endl;
    for (shared_ptr<ASTNode> child : children) {
      if (!child->isNulled)
        child->print(level + 2);
    }
  }
}

pair<int, string> ASTNode::genIntermediateCode() {
  if (isTerminal) {
    int reg = allocateRegister();
    string type;

    if (token.token == T_STR_LIT || token.token == T_CHAR_LIT) {
      litCount++;
      // again, bad attempt at name mangling
      string identifier = "UFJ8Pcjn1844XajR" + to_string(litCount);
      data_segment += "\t" + identifier + ": .asciiz \"" + token.value + "\"\n";
      select(mode) += "\tla $t" + to_string(reg) + ", " + identifier + "\n";
      type = "T_STR";
    } else if (token.token == T_INT_LIT) {
      select(mode) += "\tli $t" + to_string(reg) + ", " + token.value + "\n";
      type = "T_INT";
    } else if (token.token == T_FLOAT_LIT) {
      select(mode) += "\tli.s $f" + to_string(reg) + ", " + token.value + "\n";
      type = "T_FLOAT";
    } else if (token.token == T_BOOL_LIT) {
      select(mode) += "\tli $t" + to_string(reg) + ", " +
                      (token.value == "true" ? "1" : "0") + "\n";
      type = "T_INT";
    } else if (token.token == T_IDENTIFIER) {
      type = symbolTable[token.value];
      if (type == "T_FLOAT")
        select(mode) += "\tl.s $f" + to_string(reg) + ", " + token.value + "\n";
      else
        select(mode) += "\tlw $t" + to_string(reg) + ", " + token.value + "\n";
    }

    return {reg, type};
  }

  if (rule == "OUTER_STMTS") {
    string type = children[0]->children[0]->rule;
    string identifier = children[1]->token.value;
    if (children[2]->children[0]->rule == "FN") { // function
      mode = identifier == "main" ? M_MAIN : M_OTHER;
      select(mode) += "\n.globl " + identifier + "\n";
      if (identifier != "main")
        select(mode) += ".ent " + identifier + "\n";
      select(mode) += identifier + ":\n";
      if (identifier == "main")
        select(mode) += "\tjal U3quItLgAXaPHLuf\n";
      if (!children[2]->children[0]->children[4]->isNulled)
        children[2]->children[0]->children[4]->genIntermediateCode();
      if (identifier != "main") {
        select(mode) += "\n.end " + identifier + "\n";
      }
      // handle opt_return
    } else { // global variable declaration or assignment
      mode = M_COMPUTE_GLOBALS;
      addToDataSegment(type, identifier);
      if (!children[2]->children[0]->isNulled) {
        int reg;
        string type;
        tie(reg, type) =
            children[2]->children[0]->children[1]->genIntermediateCode();
        if (type == "T_FLOAT")
          select(mode) +=
              "\ts.s $f" + to_string(reg) + ", " + identifier + "\n";
        else
          select(mode) += "\tsw $t" + to_string(reg) + ", " + identifier + "\n";
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
        int reg;
        string type;
        tie(reg, type) =
            children[0]->children[2]->children[1]->genIntermediateCode();
        if (type == "T_FLOAT")
          select(mode) +=
              "\ts.s $f" + to_string(reg) + ", " + identifier + "\n";
        else
          select(mode) += "\tsw $t" + to_string(reg) + ", " + identifier + "\n";
        freeReg[reg] = true;
      }
    } else if (children[0]->rule == "T_IDENTIFIER") {
      string identifier = children[0]->token.value;
      int reg;
      string type;
      tie(reg, type) =
          children[1]->children[0]->children[1]->genIntermediateCode();
      if (type == "T_FLOAT")
        select(mode) += "\ts.s $f" + to_string(reg) + ", " + identifier + "\n";
      else
        select(mode) += "\tsw $t" + to_string(reg) + ", " + identifier + "\n";
      freeReg[reg] = true;
      // handle unary, different types of assigns, & function calls
    } else if (children[0]->rule == "T_PRINT") {
      int reg;
      string type;
      tie(reg, type) = children[2]->genIntermediateCode();
      if (type == "T_INT")
        select(mode) +=
            "\tli $v0, 1\n\tmove $a0, $t" + to_string(reg) + "\n\tsyscall\n";
      else if (type == "T_FLOAT")
        select(mode) +=
            "\tli $v0, 2\n\tmov.s $f12, $f" + to_string(reg) + "\n\tsyscall\n";
      else
        select(mode) +=
            "\tli $v0, 4\n\tmove $a0, $t" + to_string(reg) + "\n\tsyscall\n";
      select(mode) +=
          "\tli $v0, 4\n\tla $a0, newline\n\tsyscall\n"; // print newline
      freeReg[reg] = true;
    } else if (children[0]->rule == "IF_STMT") {
      int reg;
      string type;
      tie(reg, type) =
          children[0]->children[2]->genIntermediateCode(); // expression result
      label += 1;
      int outer_label = label;
      select(mode) +=
          "\tbeq $t" + to_string(reg) + ", $0, L" + to_string(label) + "\n";
      freeReg[reg] = true;
      children[0]->children[5]->genIntermediateCode();
      label += 1;
      select(mode) += "\tj L" + to_string(label) + "\n";
      select(mode) += "L" + to_string(outer_label) + ":\n";
      outer_label = label;
      if (!children[0]->children[7]->isNulled) { // has an else block
        shared_ptr<ASTNode> elseBlock = children[0]->children[7]->children[1];
        if (elseBlock->children[0]->rule == "T_IF") {
          int reg;
          string type;
          tie(reg, type) = elseBlock->children[2]->genIntermediateCode();
          label += 1;
          int outer_label = label;
          select(mode) +=
              "\tbeq $t" + to_string(reg) + ", $0, L" + to_string(label) + "\n";
          freeReg[reg] = true;
          elseBlock->children[5]->genIntermediateCode();
          select(mode) += "L" + to_string(outer_label) + ":\n";
        } else
          elseBlock->children[1]->genIntermediateCode();
      }
      select(mode) += "L" + to_string(outer_label) + ":\n";
    } else if (children[0]->rule == "LOOP")
      children[0]->genIntermediateCode();
  } else if (rule == "EXPR") {
    if (children[0]->rule == "ADD") {
      int reg1;
      string type1;
      tie(reg1, type1) = children[0]->genIntermediateCode();
      if (!children[1]->isNulled) {
        int reg2;
        string type2;
        tie(reg2, type2) = children[1]->children[1]->genIntermediateCode();

        coerce(type1, reg1, type2, reg2);
        int reg3 = allocateRegister();
        string op = children[1]->children[0]->children[0]->token.value;
        select(mode) += "\t" + operations[op] + " $t" + to_string(reg3) +
                        ", $t" + to_string(reg1) + ", $t" + to_string(reg2) +
                        "\n";
        freeReg[reg1] = true;
        freeReg[reg2] = true;
        reg1 = reg3;
      }
      return {reg1, type1};
    } else { // unary negation
      int reg1;
      string type1;
      tie(reg1, type1) = children[1]->genIntermediateCode();
      int reg2 = allocateRegister();
      if (type1 == "T_FLOAT")
        select(mode) +=
            "neg.s $f" + to_string(reg2) + ", $f" + to_string(reg1) + "\n";
      else
        select(mode) +=
            "neg $t" + to_string(reg2) + ", $t" + to_string(reg1) + "\n";
      freeReg[reg1] = true;
      return {reg2, type1};
    }
  } else if (rule == "ADD") {
    int reg1;
    string type1;
    tie(reg1, type1) = children[0]->genIntermediateCode();
    shared_ptr<ASTNode> addp = children[1];
    while (!addp->isNulled) {
      int reg2;
      string type2;
      tie(reg2, type2) = addp->children[1]->genIntermediateCode();
      coerce(type1, reg1, type2, reg2);
      int reg3 = allocateRegister();
      if (type1 == "T_FLOAT") {
        if (addp->children[0]->children[0]->rule == "T_PLUS") // addition
          select(mode) += "\tadd.s $f" + to_string(reg3) + ", $f" +
                          to_string(reg1) + ", $f" + to_string(reg2) + "\n";
        else // subtraction
          select(mode) += "\tsub.s $f" + to_string(reg3) + ", $f" +
                          to_string(reg1) + ", $f" + to_string(reg2) + "\n";
      } else {
        if (addp->children[0]->children[0]->rule == "T_PLUS") // addition
          select(mode) += "\tadd $t" + to_string(reg3) + ", $t" +
                          to_string(reg1) + ", $t" + to_string(reg2) + "\n";
        else // subtraction
          select(mode) += "\tsub $t" + to_string(reg3) + ", $t" +
                          to_string(reg1) + ", $t" + to_string(reg2) + "\n";
      }

      addp = addp->children[2];
      freeReg[reg1] = true;
      freeReg[reg2] = true;
      reg1 = reg3;
    }
    return {reg1, type1};
  } else if (rule == "MULT") {
    int reg1;
    string type1;
    tie(reg1, type1) = children[0]->genIntermediateCode();
    shared_ptr<ASTNode> multp = children[1];
    while (!multp->isNulled) {
      int reg2;
      string type2;
      tie(reg2, type2) = multp->children[1]->genIntermediateCode();
      coerce(type1, reg1, type2, reg2);
      int reg3 = allocateRegister();
      if (type1 == "T_FLOAT") {
        if (multp->children[0]->children[0]->rule == "T_STAR") // multiplication
          select(mode) += "\tmul.s $f" + to_string(reg3) + ", $f" +
                          to_string(reg1) + ", $f" + to_string(reg2) + "\n";
        else // division
          select(mode) += "\tdiv.s $f" + to_string(reg3) + ", $f" +
                          to_string(reg1) + ", $f" + to_string(reg2) + "\n";
      } else {
        if (multp->children[0]->children[0]->rule == "T_STAR") // multiplication
          select(mode) += "\tmul $t" + to_string(reg3) + ", $t" +
                          to_string(reg1) + ", $t" + to_string(reg2) + "\n";
        else // division
          select(mode) += "\tdiv $t" + to_string(reg3) + ", $t" +
                          to_string(reg1) + ", $t" + to_string(reg2) + "\n";
      }
      multp = multp->children[2];
      freeReg[reg1] = true;
      freeReg[reg2] = true;
      reg1 = reg3;
    }
    return {reg1, type1};
  } else if (rule == "LOOP") {
    if (children[0]->rule == "T_WHILE") { // while loop
      label += 1;
      select(mode) += "L" + to_string(label) + ":\n";
      int reg;
      string type;
      tie(reg, type) = children[2]->genIntermediateCode();
      label += 1;
      int outer_label = label;
      select(mode) += "\tbeq $t" + to_string(reg) + ", $0, L" +
                      to_string(outer_label) + "\n";

      // loop body
      if (!children[5]->isNulled)
        children[5]->genIntermediateCode();

      select(mode) += "\tj L" + to_string(outer_label - 1) + "\n";
      select(mode) += "L" + to_string(outer_label) + ":\n";
      freeReg[reg] = true;
    } else { // for loop
      // initialization bit
      if (!children[3]->isNulled) {
        if (!children[2]->isNulled) // type of iterator variable defined
          addToDataSegment(children[2]->children[0]->token.value,
                           children[3]->children[0]->token.value);
        int reg;
        string type;
        tie(reg, type) =
            children[3]->children[1]->children[1]->genIntermediateCode();
        if (type == "T_FLOAT")
          select(mode) += "\ts.s $f" + to_string(reg) + ", " +
                          children[3]->children[0]->token.value + "\n";
        else
          select(mode) += "\tsw $t" + to_string(reg) + ", " +
                          children[3]->children[0]->token.value + "\n";
        freeReg[reg] = true;
      }

      // comparison bit
      label += 1;
      select(mode) += "L" + to_string(label) + ":\n";
      int reg;
      string type;
      tie(reg, type) = children[5]->genIntermediateCode();
      label += 1;
      int outer_label = label;
      select(mode) += "\tbeq $t" + to_string(reg) + ", $0, L" +
                      to_string(outer_label) + "\n";

      // loop body
      if (!children[10]->isNulled)
        children[10]->genIntermediateCode();

      // increment/decrement bit
      if (!children[7]->isNulled) {
        int reg;
        string type;
        tie(reg, type) =
            children[7]->children[1]->children[1]->genIntermediateCode();
        if (type == "T_FLOAT")
          select(mode) += "\ts.s $f" + to_string(reg) + ", " +
                          children[3]->children[0]->token.value + "\n";
        else
          select(mode) += "\tsw $t" + to_string(reg) + ", " +
                          children[3]->children[0]->token.value + "\n";
        freeReg[reg] = true;
      }

      // jump
      select(mode) += "\tj L" + to_string(outer_label - 1) + "\n";
      select(mode) += "L" + to_string(outer_label) + ":\n";
      freeReg[reg] = true;
    }
  } else if (rule == "TERM") {
    if (children[0]->rule == "CONST")
      return children[0]->genIntermediateCode();
    else if (children[0]->rule == "T_LPAREN") // ( EXPR )
      return children[1]->genIntermediateCode();
    else { // function call or variable
      return children[0]->genIntermediateCode();
    }
  } else {
    return children[0]->genIntermediateCode();
  }

  // the stuff of nightmares!
  return {-1, "NONE"};
}

void genIntermediateCode(shared_ptr<ASTNode> root) {
  root->genIntermediateCode();
  main_text_segment += "\tli $v0, 10\n\tsyscall\n";

  ofstream file("out.asm");
  if (file.is_open()) {
    file << data_segment << "\n"
         << main_text_segment << "\n"
         << compute_globals << "\tjr $ra\n.end U3quItLgAXaPHLuf\n" + other;
    file.close();
  }
}