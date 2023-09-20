#ifndef REG_H
#define REG_H
#include <unordered_set>
#include "node.h"
#include "lexer.h"

class reg {
public:
    reg();
    reg(Token, char);
    reg& concatenate(reg&);
    reg& unionWith(reg&);
    reg& applyKleeneStar();
    Node* getStartState();
    Node* getAcceptState();
    bool acceptsEmptyString();
    void print();
    int match(std::string, int);
    Token token_def;
private:
    Node* startState;
    Node* acceptState;
    void printHelper(Node*, std::unordered_set<int>&);
    bool acceptsEmptyStringHelper(Node *, std::unordered_set<Node *> &);
    int matchHelper(Node *currentState, const std::string &s, int p, std::unordered_set<Node *> &visited);
};

#endif // REG_H