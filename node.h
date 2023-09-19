#ifndef NODE_H
#define NODE_H

#include <vector>

class Node;

struct Transition {
    char c;
    Node* node;
    Transition(char c, Node* node) : c(c), node(node) {}
};

class Node {
public:
    Node();
    void addTransition(char c, Node *node);
    std::vector<Transition> getTransitions();
    int getId() const; // Function to get the unique number of a node
private:
    int id; // Member to store the unique number for each node
    std::vector<Transition> transitions;
    static int nodeCount; // Static member to keep track of the number of nodes
};

#endif // NODE_H