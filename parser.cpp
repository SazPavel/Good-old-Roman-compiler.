#include "roman.h"


using namespace std;
/*
Node::Node(){
	NoType = 0;
	lexeme = "";
}

Node::Node(int type, string lex = "", Node s1=Node(), Node s2=Node(), Node s3=Node()){
	NoType = type;
	lexeme = lex;
	son1 = &s1;
	son2 = &s2;
	son3 = &s3;
}

Node::~Node(){
}

*/
Parser::Parser(Lexer *lexer){
	this->lexer = lexer;
}

Parser::~Parser(){
}

Node* Parser::term(){
	printf("term\n");
	if(token.type == TyIdentifier){
		Node *n = (struct Node *)malloc(sizeof(struct Node));
		n->Type = TyIdentifier;
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		return n;
	}
	if(token.type == TyNumber){
		Node *n = (struct Node *)malloc(sizeof(struct Node));
		n->Type = TyNumber;
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		return n;
	}else{
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		return par_exp();
	}
}

Node* Parser::summa(){
	printf("summa\n");
	Node *n = (struct Node *)malloc(sizeof(struct Node));
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
	n = term();
	while(token.type == TyMinus || token.type == TyPlus){
		n->son1 = n;			
		n->Type = token.type;
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son2 = term();
	}
	return n;
}

Node* Parser::test(){	
	printf("test\n");
	Node *n = (struct Node *)malloc(sizeof(struct Node));
	glub += 1;
	for(int i = 0 ; i < glub; i++)printf("  ");
	n = summa();
	if(token.type == TyLess){
		n->son1 = n;		
		n->Type = token.type;	
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		if(token.type == TyEqual){
			n->Type = LessEq;
			n->lexeme = "<=";
			token = lexer->GetToken();
		}
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son2 = summa();			
	}else if(token.type == TyOver){
		n->son1 = n;		
		n->Type = token.type;	
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		if(token.type == TyEqual){
			n->Type = OverEq;
			n->lexeme = ">=";
			token = lexer->GetToken();
		}
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son2 = summa();			
	}else if(token.type == TyEql){
		n->son1 = n;		
		n->Type = token.type;	
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son2 = summa();	
	}
	return n;
}

Node* Parser::express(){
	printf("exp\n");
	if(token.type != TyIdentifier){
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		return test();
	}
	Node *n = (struct Node *)malloc(sizeof(struct Node));
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
	n = test();
	if(n->Type == TyIdentifier && token.type == TyEqual){
		token = lexer->GetToken();
		n->son1 = n;		
		n->Type = TySet;	
		n->lexeme = "set";
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son2 = express();
	}	
	return n;
}
	
Node* Parser::par_exp(){
	printf("par_exp\n");
	Node *n = (struct Node *)malloc(sizeof(struct Node));
	if(token.type != TyLpar){
		cout << "( expected " << " string " << token.str << " position " << token.pos << "    " << token.type << endl;
		exit(-1);
	}
	token = lexer->GetToken();
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
	n = express();
	if(token.type != TyRpar){
		cout << ") expected " << " string " << token.str << " position " << token.pos << endl;
		exit(-1);
	}
	token = lexer->GetToken();
	return n;	
}

Node* Parser::step(){
	printf("step\n");
	glub = 0;
	Node *n = (struct Node *)malloc(sizeof(struct Node));
	if(token.type == TyIf){
		n->Type = TyIf;
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son1 = par_exp();
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son2 = step();
		if(token.type == TyElse){
			n->Type = TyElse;
			n->lexeme = token.lexeme.c_str();
			token = lexer->GetToken();
			glub += 1;
			for(int i = 0 ; i < glub; i++)printf("  ");
			n->son3 = step();
		}
	}else if(token.type == TyWhile){
		n->Type = TyWhile;
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		n->son1 = par_exp();		
		n->son2 = step();
	}else if(token.type == TyDo){
		n->Type = TyDo;
		n->lexeme = token.lexeme.c_str();
		token = lexer->GetToken();
		n->son1 = step();
		if(token.type != TyWhile){
			cout << "while expected " << " string " << token.str << " position " << token.pos << endl;
			exit(-1);
		}
		token = lexer->GetToken();		
		n->son2 = par_exp();
		if(token.type != TySemicolon){
			cout << "; expected " << " string " << token.str << " position " << token.pos << endl;
			exit(-1);
		}
	}else if(token.type == TySemicolon){
		n->Type = 0;
		n->lexeme = "";
		token = lexer->GetToken();			
	}else if(token.type == TyLbra){
		n->Type = 0;
		n->lexeme = token.lexeme.c_str();
		while(token.type != TyRbra){
			n->son1 = n;
			n->Type = SEQ;
			n->son2 = step();
		}
		n->lexeme = token.lexeme.c_str();
		
	}else{
		n->Type = Expr;
		n->lexeme = "expression";
		glub += 1;
		for(int i = 0 ; i < glub; i++)printf("  ");
		n->son1 = express();		
		if(token.type != TySemicolon){
			cout << "; expected " << " string " << token.str << " position " << token.pos << endl;
			exit(-1);
		}
		token = lexer->GetToken();
	}		
	return n;
}

Node Parser::Parun(string s){
	lexer->start(s);
	Node node;
	glub = 0;
	token = lexer->GetToken();
	if(token.type == 22){
		node.Type = 22;
		node.lexeme = "main";
		token = lexer->GetToken();
		node.son1 = step();
	}
	if(token.type != TyEOF){
		cout << "strange error" << endl;
		exit(-1);		
	}
	return node;	
}

