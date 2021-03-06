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
	string str = " ";
	int level = -1;
	int sublevel = -1;
	float *masf;
	int *masi;
};

struct node{
	int Type;
	int type_num = 0;
	int level = 0;
	int sublevel = 0;
	int count = 0;
	string lexeme;
	struct identif id;
	node *son1 = NULL;
	node *son2 = NULL;
	node *son3 = NULL;
};


class Parser {
	public:
		Parser(Lexer *lexer, Token *token);
		~Parser();
		void treeprint(node *tree, int n);
		node Parun(string str);
		node* step(int flag);
		node* express();	
		node* par_exp(int t);
		node* test();
		node* summa();
		node* term();
		node* func();
		int array(int flag, node *n, Token *token);
		int found_id(Token *token, node *n, int f, int mas);
		int found(node *n);
		
    	hash <std::string> hash_fn;		
		identif id[SIZEID][SIZEI] = {};
	protected:
		int level, sublevel[10], levelflag;
		string token_old;
		int found_str_id(node *tree);
		Lexer *lexer;
		Token *token;
		
};
