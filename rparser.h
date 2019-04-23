#pragma once

#include "roman.h"

#define SIZEID 50
#define SIZEI 5

using namespace std;

struct identif{
	int Type;
	int BaseType;
	int count;
	string value;
	int level;
	int sublevel;
};

struct node{
	int Type;
	int type_num;
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
		int found_id(Token *token, node *n, int f);
		
    	hash <std::string> hash_fn;		
		identif id[SIZEID][SIZEI] = {};
	protected:
		int level, sublevel, levelflag;
		string token_old;
		Lexer *lexer;
		Token *token;
		
};
