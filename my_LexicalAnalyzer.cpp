#include <string>
#include <iostream>
#include "my_LexicalAnalyzer.h"
#include "lexer.h"

struct MyToken;

my_LexicalAnalyzer::my_LexicalAnalyzer(std::vector<reg> r, std::string s) {
    reg_list = r;
    input_string = s;
    p = 1;
}

MyToken my_LexicalAnalyzer::my_GetToken() {
    //keep track of position p in string s, and generate tokens here
    MyToken t;
    if(p >= input_string.size()) {
        t.lexeme = "";
        t.token_type = "EOF";
        return t;
    }
    int max_p = -1;
    std::string chosen_reg_id;
    for(reg r : reg_list) {
        std::cout << input_string.at(p) << "\n";
        while(std::isspace(input_string.at(p))) {
            p++;
        }
        std::cout << "Calling match(" << r.token_def.lexeme << ", " << input_string << ", " << p << ")\n";
        r.print();
        int res = match(r, input_string, p);
        std::cout << "Res: " << res << std::endl;
        if(res == -1) {
            std::cout << "ERROR" << std::endl;
            exit(1);
        }
        if(res > max_p) {
            chosen_reg_id = r.token_def.lexeme;
            std::cout << "Chosen_Reg: " << chosen_reg_id << "\n";
            max_p = res;
        }
    }
    t.lexeme = input_string.substr(p, max_p - p);
    t.token_type = chosen_reg_id;
    p = max_p + 1;
    return t;
}

int my_LexicalAnalyzer::match(reg r, std::string s, int p) {
    return r.match(s, p);
}
