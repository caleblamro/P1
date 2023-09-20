#include "node.h"

int Node::nodeCount = 0; // Initialize static member

Node::Node() {
    id = ++nodeCount; // Increment and assign unique number
}

//get the id of this node, used for readability while debugging
int Node::getId() const {
    return id;
}
//add a transition from this node to another node on c
void Node::addTransition(char c, Node* node) {
    transitions.push_back(Transition(c, node));
}
//get all transitions
std::vector<Transition> Node::getTransitions() {
    return transitions;
}