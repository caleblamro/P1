/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <unordered_map>
#include <string>
#include "reg.h"
#include "lexer.h"

class Parser {
  public:
    void parse_input();
    void readAndPrintAllInput();
  private:
    std::unordered_map<std::string, reg> token_list;
    std::vector<reg> reg_list;
    LexicalAnalyzer lexer;
    void syntax_error(std::string);
    Token expect(TokenType expected_type, std::string);
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();
    reg* parse_expr(Token);
};

#endif

