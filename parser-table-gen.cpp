#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

template <typename T>
void print_map(map<string, T> mp, string name) {
    cout << "\n" << name << endl;
    for (auto p : mp) {
        cout << p.first << " => ";
        for (auto v: p.second) {
            for (auto s: v)
                cout << s << ", ";
            cout << "\n      ";
        }
        cout <<  endl;
    }
}

bool is_all_upper(const string& str) {
    return all_of(str.begin(), str.end(), [](unsigned char c){ return isupper(c); });
}


enum {TERMINAL, NONTERMINAL, EPSILON};

class Node {
    public:
        int nodeType;
        string name;

        Node(int nodeType, string name) {
            this->nodeType = nodeType;
            this->name = name;
        }

        Node(int nodeType) {
            this->nodeType = nodeType;
        }
};

ostream& operator<<(ostream& os, const Node& node) {
    os << (node.nodeType == TERMINAL ? "TERMINAL" : (node.nodeType == NONTERMINAL ? "NONTERMINAL" : "EPSILON"));
    os << " ";
    if (node.nodeType != EPSILON)
        os << node.name;
    return os;
}


int main() {
    /* parse input grammar */

    string file_path = "parser.in";
    ifstream infile;
    infile.open(file_path, ios::in);   // attempt to open the file
    if (infile.fail()) { // check for file status
        perror("file open failed");
        exit(EXIT_FAILURE);
    }

    char ch;
    map<string, vector<vector<Node>>> productions;
    string buffer = "";
    while (infile >> ch) {
        while (ch != ':') {
            buffer += ch;
            infile >> ch;
        }

        string prod_name = buffer;
        if (productions.find(prod_name) == productions.end())
            productions.insert({prod_name, {}});
        vector<Node> production = {};
        
        buffer = "";
        infile >> ch;
        while (true) {
            if (ch == '#') {
                production.push_back(Node(EPSILON));
            } else {
                while (ch != ';' && ch != '|' && ch != ' ' && ch != '\n' && !infile.eof()) {
                    buffer += ch;
                    infile >> noskipws >> ch;
                }

                if (ch == '|' && production.size() > 0) {
                    productions[prod_name].push_back(production);
                    production = {};
                }

                if (buffer.size() > 0) {
                    if (is_all_upper(buffer))
                        production.push_back(Node(NONTERMINAL, buffer));
                    else
                        production.push_back(Node(TERMINAL, buffer));
                    buffer = "";
                }

                if (ch == ';')
                    break;
            }
            infile >> noskipws >> ch;
        }

        if (production.size() > 0)
            productions[prod_name].push_back(production);
    }

    print_map(productions, "Productions");


    // /* compute first sets */

    // map<string, unordered_set<char>> first;
    // for (auto p: productions) {
    //     first.insert({p.first, {}});
    //     // Initially, for all nonterminals A, set
    //     // FIRST(A) = { t | A → tω for some ω }
    //     for (int i = 0; i < p.second.size(); i++) {
    //         if (!isupper(p.second[i][0]))
    //             first[p.first].insert(p.second[i][0]);
    //     }
    // }

    // // Then, repeat the following until no
    // // changes occur: For each nonterminal A,
    // // for each production A → Bω, set
    // // FIRST(A) = FIRST(A) ∪ FIRST(B)
    // // For all nonterminals A where A → ε is a production, add ε
    // // to FIRST(A).
    // // currently, ε is represented by #
    // bool changes_occurred = true;
    // while (changes_occurred) {
    //     changes_occurred = false;
    //     for (auto p: productions) {
    //         // probe for the B in A → Bω
    //         for (int i = 0; i < p.second.size(); i++) {
    //             if (isupper(p.second[i][0])) {
    //                 // FIRST(A) = FIRST(A) ∪ FIRST(B)
    //                 string a = string(1, p.first[0]);
    //                 string b = string(1, p.second[i][0]);
    //                 for (char c: first[b]) {
    //                     if (first[a].find(c) == first[a].end()) {
    //                         first[a].insert(c);
    //                         changes_occurred = true;
    //                     }
    //                 }
    //                 break;
    //             }
    //         }
    //     }
    // }

    // print_map(first, "FIRST Sets");

    // // TODO: What if the first nonterminal in a production
    // // can produce ε?

    // /* compute follow sets */

    // map<string, unordered_set<char>> follow;
}