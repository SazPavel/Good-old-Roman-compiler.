#pragma once

#include "roman.h"

using namespace std;

struct Node{
	int Type;
	const char* lexeme;
	struct Node *son1;// = NULL;
	struct Node *son2;// = NULL;
	struct Node *son3;// = NULL;
};

class Parser {
	public:
		Parser(Lexer *lexer);
		~Parser();
		Node Parun(string str);
		Node* step();
		Node* express();	
		Node* par_exp();
		Node* test();
		Node* summa();
		Node* term();
	
	protected:
		int glub;
		Lexer *lexer;
		Token token;
		
};
