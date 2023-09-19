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
#include <unordered_set>
#include "parser.h"
#include "reg.h"

using namespace std;

// this syntax error function needs to be 
// modified to produce the appropriate message
void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type) {
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

reg* Parser::parse_expr() {
    Token t = lexer.peek(1);
    reg* r = new reg(); // Dynamically allocate memory for a reg objects
    if (t.token_type == CHAR || t.token_type == UNDERSCORE) {
        if(t.token_type == UNDERSCORE) {
            *r = reg('_');
        } else {
            t = lexer.GetToken();
            char c = t.lexeme[0];
            *r = reg(c);
        }
    } else if (t.token_type == LPAREN) {
        lexer.GetToken();  // Consume the token
        reg* expr1 = parse_expr();
        expect(RPAREN);
        Token op = lexer.peek(1);
        if (op.token_type == DOT || op.token_type == OR) {
            lexer.GetToken();  // Consume the token
            expect(LPAREN);
            reg* expr2 = parse_expr();
            expect(RPAREN);
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
            syntax_error();
        }
    } else {
        syntax_error();
    }
    return r;
}

void Parser::parse_token() {
    Token t = expect(ID);
    std::cout << "Parsed ID Lexeme: " + t.lexeme + "\n";
    reg* r = parse_expr();
    r->print();
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
        syntax_error();
    }
}


void Parser::parse_tokens_section() {
    parse_token_list();
    //if we do not see this hash we should simply output "SYNTAX ERROR"
    expect(HASH);
}

void Parser::parse_input() {
    parse_tokens_section();
    Token t = expect(INPUT_TEXT);
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

int main()
{
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
