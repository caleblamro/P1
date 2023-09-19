#ifndef MY_LEXICALANALYZER_H
#define MY_LEXICALANALYZER_H

#include <vector>
#include <string>
#include <utility>  // for std::pair
#include "reg.h"
#include "lexer.h"

class my_LexicalAnalyzer{
    public:
        my_LexicalAnalyzer(std::vector<std::pair<std::string, reg>> r, std::string s, int p);
        Token my_GetToken();
        int match(reg, std::string, int);
};

#endif