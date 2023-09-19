#ifndef REG_H
#define REG_H
#include <unordered_set>
#include "node.h"

class reg {
public:
    reg();
    reg(char c);
    reg& concatenate(reg&);
    reg& unionWith(reg&);
    reg& applyKleeneStar();
    Node* getStartState();
    Node* getAcceptState();
    void print();
private:
    Node* startState;
    Node* acceptState;
    void printHelper(Node* node, std::unordered_set<int>& visited);
};

#endif // REG_H