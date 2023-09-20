#ifndef MY_LEXICALANALYZER_H
#define MY_LEXICALANALYZER_H

#include <vector>
#include <string>
#include "reg.h"
#include "lexer.h"


struct MyToken {
    std::string token_type;
    std::string lexeme;
};


class my_LexicalAnalyzer{
    public:
        my_LexicalAnalyzer(std::vector<reg>, std::string);
        MyToken my_GetToken();
        int match(reg, std::string, int);
    private:
        int p;
        std::vector<reg> reg_list;
        std::string input_string;
};

#endif