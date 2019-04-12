#pragma once

#include "roman.h"

using namespace std;
/*

class Node {
	public:
		Node();
		Node(int type, string lex, Node s1, Node s2, Node s3);
		~Node();
		int NoType;
		string lexeme;
		Node *son1 = NULL;
		Node *son2 = NULL;
		Node *son3 = NULL;
};
*/

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
