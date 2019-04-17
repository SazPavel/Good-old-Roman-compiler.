#pragma once

#include "roman.h"

using namespace std;

struct node{
	int Type;
	string lexeme;
	node *son1 = NULL;
	node *son2 = NULL;
	node *son3 = NULL;
};

class Parser {
	public:
		Parser(Lexer *lexer, Token *token);
		~Parser();
		node Parun(string str);
		node* step(int flag);
		node* express();	
		node* par_exp(int t);
		node* test();
		node* summa();
		node* term();
	
	protected:
		int glub;
		Lexer *lexer;
		Token *token;
		
};
