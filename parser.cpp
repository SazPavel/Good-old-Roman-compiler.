#include "roman.h"


using namespace std;

void cop(node *n1, node *n){
	n1->lexeme = n->lexeme;
	n1->son1 = n->son1;
	n1->son2 = n->son2;
	n1->son3 = n->son3;
	n1->Type = n->Type;
	n->lexeme = "";
	n->son1 = NULL;
	n->son2 = NULL;
	n->son3 = NULL;
	n->Type = 0;
}


int Parser::found_id(Token *token, node *n, int f){
	int idn = (hash_fn(token->lexeme))%SIZEID;
	if(id[idn][0].Type == 0){
		if(f == 0){
			return -1;
		}else{		
			id[idn][0].Type = token->type;
			id[idn][0].BaseType = token->type;
			id[idn][0].count = 1;
			id[idn][0].value = token->lexeme;
			id[idn][0].level = level;
			id[idn][0].sublevel = sublevel;
			return idn;
		}	
	}else{
		for(int i = 0; i < SIZEI; i++){
			if(f == 1){
				if(id[idn][i].level == level && id[idn][i].sublevel == sublevel)
					return -1;				
				
				if(id[idn][i].Type == 0){		
					id[idn][i].Type = token->type;
					id[idn][i].BaseType = token->type;
					id[idn][i].count = 1;
					id[idn][i].value = token->lexeme;
					id[idn][i].level = level;
					id[idn][i].sublevel = sublevel;
					return idn;
				}		
			}else{
				if(token->lexeme == id[idn][i].value){
					n->lexeme = id[idn][i].value;
					n->Type = id[idn][i].Type;
					return idn;
				}		
			}	
		}	
		return -1;
	}
}


Parser::Parser(Lexer *lexer, Token *token){
	this->token = token;
	this->lexer = lexer;
}

Parser::~Parser(){
}

node* Parser::term(){
//	printf("term\n");
//TODO
    if(token->type == TyInt){
		node *n = new node;
		n->Type = TyInt;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type != TyIdentifier){
			cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}	
		token->type = n->Type;	
    	if(found_id(token, n, 1) == -1){
			cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		n->lexeme = token->lexeme;
		*token = lexer->GetToken();
		return n;   	
    }
    if(token->type == TyFloat){
		node *n = new node;
		n->Type = TyFloat;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type != TyIdentifier){
			cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}		
		token->type = n->Type;	
    	if(found_id(token, n, 1) == -1){
			cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		n->lexeme = token->lexeme;
		*token = lexer->GetToken();
		return n;   	
    }
	if(token->type == TyIdentifier){
		node *n = new node;
		//n->Type = TyIdentifier;
		//n->lexeme = token->lexeme;
    	if(found_id(token, n, 0) == -1){
			cout << token->lexeme << " was not declarated " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}
	if(token->type == TyNumber){
		node *n = new node;
		n->Type = TyNumber;
		n->lexeme = token->lexeme;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}else{
		return par_exp(0);
	}
}

node* Parser::summa(){
//	printf("summa\n");
	node *n = new node;
	n = term();
	while(token->type == TyMinus || token->type == TyPlus){
		node *n1 = new node;
		cop(n1, n);
		n->son1 = n1;		
		n->Type = token->type;
		n->lexeme = token->lexeme;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son2 = term();
	}
	return n;
}

node* Parser::test(){	
//	printf("test\n");
	node *n = new node;
	n = summa();
	node *n1 = new node;
	if(token->type == TyLess){
		cop(n1, n);
		n->son1 = n1;		
		n->Type = token->type;	
		n->lexeme = "<";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type == TyEqual){
			n->Type = LessEq;
			n->lexeme = "<=";
			token_old = token->lexeme;
			*token = lexer->GetToken();
		}
		n->son2 = summa();			
	}else if(token->type == TyOver){
		cop(n1, n);
		n->son1 = n1;		
		n->Type = token->type;	
		n->lexeme = ">";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type == TyEqual){
			n->Type = OverEq;
			n->lexeme = ">=";
			token_old = token->lexeme;
			*token = lexer->GetToken();
		}
		n->son2 = summa();					
	}else if(token->type == TyEql){
		cop(n1, n);
		n->son1 = n1;
		n->Type = token->type;	
		n->lexeme = "==";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son2 = summa();			
	}
	return n;
}

node* Parser::express(){
//	printf("exp\n");
	if(token->type != TyIdentifier && token->type != TyInt && token->type != TyFloat){
		return test();
	}
	node *n = new node;
	n = test();
	if((n->Type == TyInt || n->Type == TyFloat ) && token->type == TyEqual){
		node *n1 = new node;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		cop(n1, n);
		n->son1 = n1;		
		n->Type = TySet;	
		n->lexeme = "set";
		n->son2 = express();
	}	
	return n;
}
	
node* Parser::par_exp(int t){
//	printf("par_exp\n");
	node *n = new node;
	if(token->type != TyLpar){
		if (t)
			cout << "( expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
		else
			cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
		exit(-1);
	}
	token_old = token->lexeme;
	*token = lexer->GetToken();
	n = express();
	if(token->type != TyRpar){
		if(t)
			cout << ") expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
		else
			cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			
		exit(-1);
	}
	token_old = token->lexeme;
	*token = lexer->GetToken();
	return n;	
}

node* Parser::step(int flag){
//	printf("step %d\n", token->type);
	if(flag == 1 && token->type != TyLbra){
		cout << "{ expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
		exit(-1);
	}
	node *n = new node;
	if(token->type == TyIf){
		n->Type = TyIf;
		n->lexeme = "si";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son1 = par_exp(1);
		n->son2 = step(0);
		if(token->type == TyElse){
			n->Type = TyElse;
			n->lexeme = "aliud";
			token_old = token->lexeme;
			*token = lexer->GetToken();
			n->son3 = step(0);
		}
	}else if(token->type == TyWhile){
		n->Type = TyWhile;
		n->lexeme = "dum";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son1 = par_exp(1);		
		n->son2 = step(0);
	}else if(token->type == TyDo){
		n->Type = TyDo;
		n->lexeme = "facite";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son1 = step(0);
		if(token->type != TyWhile){
			cout << "dum expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}
		token_old = token->lexeme;
		*token = lexer->GetToken();		
		n->son2 = par_exp(1);
		if(token->type != TySemicolon){
			cout << "; expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}
		token_old = token->lexeme;
		*token = lexer->GetToken();	
	}else if(token->type == TySemicolon){
		n->Type = 0;
		n->lexeme = ";";
		token_old = token->lexeme;
		*token = lexer->GetToken();			
	}else if(token->type == TyLbra){
		level += 1;		
		token_old = token->lexeme;
		*token = lexer->GetToken();		
		n = step(0);	
		while(token->type != TyRbra){
			if(lexer->position >= lexer->length){
				cout << "} expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
			node *n1 = new node;
	   		cop(n1, n);
			n->son1 = n1;
			n->lexeme = "SEQ";
			n->Type = SEQ;
			n->son2 = step(0);
		}
		level -= 1;
		sublevel += 1;
		token_old = token->lexeme;
		*token = lexer->GetToken();			
	}else{
		n->Type = Expr;
		n->lexeme = "expr";
		n->son1 = express();	
		if(token->type != TySemicolon){
			cout << "; expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}
		token_old = token->lexeme;
		*token = lexer->GetToken();
	}		
	return n;
}

node Parser::Parun(string s){
	lexer->start(s);
	node node;
	lexer->nustring = 0;
	token_old = token->lexeme;
	*token = lexer->GetToken();
	level = 0;
	sublevel = 0;
	if(token->type == TyMain){
		node.Type = TyMain;
		node.lexeme = "SPQR";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		node.son1 = step(1);
	}
	if(token->type != TyEOF){
		cout << "Invalid statement syntax  " << token->type << "    " << token->lexeme << endl;
		exit(-1);		
	}
	return node;	
}

