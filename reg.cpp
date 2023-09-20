#include <iostream>
#include <unordered_set>
#include "reg.h"
#include "lexer.h"
reg::reg() {

}
//base case for either an epsilon transition or a normal transition
reg::reg(Token t, char c) {
    token_def = t;
    //create the start state
    startState = new Node();
    //create the accept state
    acceptState = new Node();
    //set a transition from the start to accept with the character as the transition functions
    startState->addTransition(c, acceptState);
}
//concatenation of two regs ex: ((a).(b))
reg& reg::concatenate(reg& other) {
    //add an epsilon transition between the accept state of the first reg and the start state of the second reg
    acceptState->addTransition('_', other.startState);
    //remove the original accept state and set it to the second regs accept state
    acceptState = other.acceptState;
    return *this;
}
//union of two regs ex: ((a)|(b))
reg& reg::unionWith(reg& other) {
    Node* newStart = new Node();
    Node* newAccept = new Node();
    //add two epsilon transitions from the new start state to the two old start states
    newStart->addTransition('_', startState);
    newStart->addTransition('_', other.startState);
    //add two epsilon transitions from the old accept states to the new accept states
    acceptState->addTransition('_', newAccept);
    other.acceptState->addTransition('_', newAccept);
    //set the start state to the new start state
    startState = newStart;
    //set the accept state to the new accept state
    acceptState = newAccept;
    return *this;
}
//Kleene star operation ex: ((a)*)
reg& reg::applyKleeneStar() {
    Node* newStart = new Node();
    Node* newAccept = new Node();
    //add an epsilon transition from the new start state to the old start state, as well as to the new accept state
    newStart->addTransition('_', startState);
    newStart->addTransition('_', newAccept);
    //add an epsilon transition from the old accept state to the new start and new accept state
    acceptState->addTransition('_', startState);
    acceptState->addTransition('_', newAccept);
    //set start and accept states correctly
    startState = newStart;
    acceptState = newAccept;
    return *this;
}

void reg::print() {
    std::unordered_set<int> visited;
    std::cout << "START_STATE: " << startState->getId() << "\n";
    std::cout << "ACCEPT_STATE: " << acceptState->getId() << "\n";
    printHelper(startState, visited);
}

void reg::printHelper(Node* node, std::unordered_set<int>& visited) {
    if (visited.find(node->getId()) != visited.end()) {
        return;
    }
    visited.insert(node->getId());
    
    auto transitions = node->getTransitions();
    for(auto transition : transitions) {
        std::cout << node->getId() << "--" << transition.c << "->" << transition.node->getId() << "\n";
        printHelper(transition.node, visited);
    }
    std::cout << std::endl;  // Newline after each node's details
}

bool reg::acceptsEmptyString() {
    std::unordered_set<Node*> visited;
    return acceptsEmptyStringHelper(startState, visited);
}

bool reg::acceptsEmptyStringHelper(Node* node, std::unordered_set<Node*>& visited) {
    if (node == acceptState) {
        return true;
    }
    if (visited.find(node) != visited.end()) {
        return false;
    }
    visited.insert(node);

    auto transitions = node->getTransitions();
    for (auto transition : transitions) {
        if (transition.c == '_' && acceptsEmptyStringHelper(transition.node, visited)) {
            return true;
        }
    }
    return false;
}

int reg::match(std::string s, int p) {
    std::unordered_set<Node*> visited;
    return matchHelper(startState, s, p, visited);
}

int reg::matchHelper(Node* currentState, const std::string& s, int p, std::unordered_set<Node*>& visited) {
    if(std::isspace(s.at(p)) || s.at(p) == '\"') {
        return -1;
    }
    if (visited.find(currentState) != visited.end()) {
        return -1;  // Avoid cycles
    }
    visited.insert(currentState);
    int endIndex = -1;
    if (currentState == acceptState) {
        endIndex = p - 1;  // Update the endIndex if we reach the accept state
    }

    // Explore epsilon transitions
    auto transitions = currentState->getTransitions();
    std::cout << "Exploring node " << currentState->getId() << "\n";
    for (auto transition : transitions) {
        if (transition.c == '_') {
            std::cout << currentState->getId() << " --" << transition.c << "-> " << transition.node->getId() << "\n";
            int newEndIndex = matchHelper(transition.node, s, p, visited);
            endIndex = std::max(endIndex, newEndIndex);
        }
        if (transition.c == s.at(p)) {
            std::cout << currentState->getId() << " --" << transition.c << "-> " << transition.node->getId() << "\n";
            endIndex = p + 1;
            // std::cout << "Matching: ";
            int newEndIndex = matchHelper(transition.node, s, p+1, visited);
            endIndex = std::max(endIndex, newEndIndex);
        }
    }

    visited.erase(currentState);  // Backtrack
    return endIndex;
}

Node* reg::getStartState() {
    return startState;
}

Node* reg::getAcceptState() {
    return acceptState;
}