/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <unordered_map>
#include "parser.h"
#include "my_LexicalAnalyzer.h"
#include "reg.h"

using namespace std;

// this syntax error function needs to be 
// modified to produce the appropriate message
void Parser::syntax_error(string errorMsg) {
    cout << errorMsg;
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type, string errorMsg) {
    Token t = lexer.GetToken();
    if (t.token_type != expected_type) {
        syntax_error(errorMsg);
    }
    return t;
}

reg* Parser::parse_expr(Token token_def) {
    string errorMsg = "SYNTAX ERROR IN EXPRESSION OF " + token_def.lexeme + "\n";
    Token t = lexer.peek(1);
    reg* r = new reg(); // Dynamically allocate memory for a reg objects
    if (t.token_type == CHAR || t.token_type == UNDERSCORE) {
        //in either case we will need to consume the token
        t = lexer.GetToken();
        if(t.token_type == UNDERSCORE) {
            *r = reg(token_def, '_');
        } else {
            char c = t.lexeme[0];
            *r = reg(token_def, c);
        }
    } else if (t.token_type == LPAREN) {
        lexer.GetToken();  // Consume the token
        reg* expr1 = parse_expr(token_def);
        expect(RPAREN, errorMsg);
        Token op = lexer.peek(1);
        if (op.token_type == DOT || op.token_type == OR) {
            lexer.GetToken();  // Consume the token
            expect(LPAREN, errorMsg);
            reg* expr2 = parse_expr(token_def);
            expect(RPAREN, errorMsg);
            if(op.token_type == DOT) {
                *r = expr1->concatenate(*expr2);
            } else if(op.token_type == OR) {
                *r = expr1->unionWith(*expr2);
            }
        } else if (op.token_type == STAR) {
            //consume the star token
            lexer.GetToken();
            *r = expr1->applyKleeneStar();
        } else {
            syntax_error(errorMsg);
        }
    } else {
        syntax_error(errorMsg);
    }
    return r;
}

void Parser::parse_token() {
    string e = "SNYTAX ERORR\n";
    Token t = expect(ID, e);
    // std::cout << "Parsed ID Lexeme: " + t.lexeme + "\n";
    reg* r = parse_expr(t);
    //add the reg to the list of reges
    reg_list.push_back(*r);
}

void Parser::parse_token_list() {
    parse_token();
    Token t = lexer.peek(1);
    if (t.token_type == COMMA) {
        lexer.GetToken();  // Consume the token
        parse_token_list();
    } else if (t.token_type == HASH) {
        // Do nothing, we will consume HASH in parse_tokens_section
    } else {
        string e = "SNYTAX ERORR\n";
        syntax_error(e);
    }
}


void Parser::parse_tokens_section() {
    parse_token_list();
    string e = "SNYTAX ERORR\n";
    expect(HASH, e);
}

void Parser::parse_input() {
    parse_tokens_section();
    string e = "SNYTAX ERORR\n";
    Token t = expect(INPUT_TEXT, e);
    vector<string> regsThatAcceptEpsilon;
    std::unordered_map<std::string, int> lexeme_to_line;
    bool dup = false;
    for (const auto& reg : reg_list) {
        const auto& lexeme = reg.token_def.lexeme;
        const auto& line_no = reg.token_def.line_no;
        if (lexeme_to_line.find(lexeme) != lexeme_to_line.end()) {
            std::cout << "Line " << line_no << ": " << lexeme << " already declared on line " << lexeme_to_line[lexeme] << std::endl;
            dup = true;
        } else {
            lexeme_to_line[lexeme] = line_no;
        }
    }
    if(dup) {
        exit(1);
    }
    for(reg reg : reg_list) {
        if(reg.acceptsEmptyString()) {
            regsThatAcceptEpsilon.push_back(reg.token_def.lexeme);
        }
    }
    if(regsThatAcceptEpsilon.size() > 0) {
        string errorMsg = "EPSILON IS NOOOOOOOT A TOKEN !!! ";
        for(const auto& s : regsThatAcceptEpsilon) {
            errorMsg += " " + s;
        }
        errorMsg += "\n";
        syntax_error(errorMsg);
    }

    //brokennnn
    string input_string = t.lexeme;
    my_LexicalAnalyzer myLexer = my_LexicalAnalyzer(reg_list, input_string);
    while(true) {
        MyToken t = myLexer.my_GetToken();
        cout << t.token_type << ", " << "\"" << t.lexeme << "\"" << std::endl;
        if(t.token_type.compare("EOF")) {
            break;
        }
    }
}

// This function simply reads and prints all tokens
// I included it as an example. You should compile the provided code
// as it is and then run ./a.out < tests/test0.txt to see what this function does
// This function is not needed for your solution and it is only provided to
// illustrate the basic functionality of getToken() and the Token type.

void Parser::readAndPrintAllInput() {
    Token t;

    // get a token
    t = lexer.GetToken();

    // while end of input is not reached
    while (t.token_type != END_OF_FILE) 
    {
        t.Print();         	// print token
        t = lexer.GetToken();	// and get another one
    }
        
    // note that you should use END_OF_FILE and not EOF
}

int main() {
    // note: the parser class has a lexer object instantiated in it (see file
    // parser.h). You should not be declaring a separate lexer object. 
    // You can access the lexer object in the parser functions as shown in 
    // the example  method Parser::readAndPrintAllInput()
    // If you declare another lexer object, lexical analysis will 
    // not work correctly
    Parser parser;
    parser.parse_input();
    // parser.readAndPrintAllInput();
	
}
