#include "roman.h"


using namespace std;

void cop(node *n1, node *n){
	n1->lexeme = n->lexeme;
	n1->son1 = n->son1;
	n1->son2 = n->son2;
	n1->son3 = n->son3;
	n1->Type = n->Type;
	n1->type_num = n->type_num;
	n1->level = n->level;
	n1->sublevel = n->sublevel;
	n->lexeme = "";
	n->son1 = NULL;
	n->son2 = NULL;
	n->son3 = NULL;
	n->Type = 0;
}


int Parser::found_id(Token *token, node *n, int f, int mas){
	int idn = (hash_fn(token->lexeme))%SIZEID;
	if(f == 1){			
		for(int i = 0; i < SIZEI; i++){
			if(id[idn][i].level == level && id[idn][i].sublevel == sublevel)
				return -1;	
			if(id[idn][i].Type == 0){		
				id[idn][i].Type = token->type;
				id[idn][i].BaseType = token->type;
				id[idn][i].count = 1;
				id[idn][i].value = token->lexeme;
				id[idn][i].level = level;
				id[idn][i].sublevel = sublevel;
				n->level = id[idn][i].level;
				n->sublevel = id[idn][i].sublevel;
				return idn;
			}
		}
	}else{
		for(int i = SIZEI - 1; i >= 0; i--){
			if(token->lexeme == id[idn][i].value && id[idn][i].level <= level){
				n->lexeme = id[idn][i].value;
				n->type_num = n->Type = id[idn][i].Type;
				n->level = id[idn][i].level;
				n->sublevel = id[idn][i].sublevel;
				return idn;
			}		
		}	
	}
	return -1;
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
		n->type_num = n->Type = TyInt;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type != TyIdentifier){
			if(token->type == TyMas){
				token->type = n->Type;	
    			if(found_id(token, n, 1, 1) == -1){
					cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
					exit(-1);
				}
			}else{
				cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
		}
		//todo
	/*	if(token->Type == TySlpar){
			*token = lexer->GetToken();
			if(token->type != TyNumberI){
				cout << "array size must be integer" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
			mas = token->lexeme;
			*token = lexer->GetToken();
		}*/
		token->type = n->Type;	
    	if(found_id(token, n, 1, 0) == -1){
			cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		n->lexeme = token->lexeme;
		*token = lexer->GetToken();
		return n;   	
    }
    if(token->type == TyFloat){
		node *n = new node;
		n->type_num = n->Type = TyFloat;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type != TyIdentifier){
			if(token->type == TyMas){
				token->type = n->Type;	
    			if(found_id(token, n, 1, 1) == -1){
					cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
					exit(-1);
				}
			}else{
				cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
		}		
		token->type = n->Type;	
    	if(found_id(token, n, 1, 0) == -1){
			cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		n->lexeme = token->lexeme;
		*token = lexer->GetToken();
		return n;   	
    }
    if(token->type == TyStringname){
    	node *n = new node;
    	n->type_num = n->Type = TyStringname;
    	token_old = token->lexeme;
		*token = lexer->GetToken();
		if(token->type != TyIdentifier){
			cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}
		token->type = n->Type;
		if(found_id(token, n, 1, 0) == -1){
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
    	if(found_id(token, n, 0, 0) == -1){
			cout << token->lexeme << " was not declarated " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}
	if(token->type == TyString){
		node *n = new node;
		n->type_num = TyStringname;
		n->Type = TyString;
		n->lexeme = token->lexeme;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}
	if(token->type == TyNumberI){
		node *n = new node;
		n->type_num = TyInt;
		n->Type = TyNumberI;
		n->lexeme = token->lexeme;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}
	if(token->type == TyNumberF){
		node *n = new node;
		n->type_num = TyFloat;
		n->Type = TyNumberF;
		n->lexeme = token->lexeme;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}
	if(token->type == TyMas){
		node *n = new node;
		//todo где хранить инфу о номере в масс?
    	if(found_id(token, n, 0, 1) == -1){
			cout << token->lexeme << " was not declarated " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		token_old = token->lexeme;
		*token = lexer->GetToken();
		return n;
	}
	return par_exp(0);
	
}

node* Parser::summa(){
//	printf("summa\n");
	node *n = new node;
	n = term();
	while(token->type == TyMinus || token->type == TyPlus || token->type == TyMul || token->type == TyDivision){
		node *n1 = new node;
		if(n->son2 == NULL || token->type == TyMinus || token->type == TyPlus){
			cop(n1, n);
			n->son1 = n1;
			
			n->Type = token->type;
			n->lexeme = token->lexeme;
			token_old = token->lexeme;
			*token = lexer->GetToken();
			n->son2 = term();
			
			if(n->son2->type_num != n->type_num){
				string flex;
				switch(n->type_num){
					case(TyInt):{
						flex = "Int";
						break;
					}
					case(TyFloat):{
						flex = "Float";
						break;
					}
					case(TyString):{
						flex = "String";
						break;
					}
				}
				cout << "cannot convert " << n->son2->lexeme << " to " << flex << " string "<< token->str << " position " << token->pos << endl;
				exit(-1);			
			}
		}else{
			n1->son1 = n->son2;
			n->son2 = n1;
			n1->type_num = n1->son1->type_num;
			n1->Type = token->type;
			n1->lexeme = token->lexeme;
			token_old = token->lexeme;
			*token = lexer->GetToken();
			n1->son2 = term();		
			
			if(n1->son2->type_num != n1->type_num){
				string flex;
				switch(n1->type_num){
					case(TyInt):{
						flex = "Int";
						break;
					}
					case(TyFloat):{
						flex = "Float";
						break;
					}
					case(TyString):{
						flex = "String";
						break;
					}
				}
				cout << "cannot convert " << n1->son2->lexeme << " to " << flex << " string "<< token->str << " position " << token->pos << endl;
				exit(-1);			
			}
	
		}
	}
	
	return n;
}

node* Parser::test(){	
//	printf("test\n");
	node *n = new node;
	n = summa();
	node *n1 = new node;
	int f = 0;
	if(token->type == TyLess){
		f = 1;
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
		f = 1;
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
		f = 1;
		cop(n1, n);
		n->son1 = n1;
		n->Type = token->type;	
		n->lexeme = "==";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son2 = summa();			
	}
	if(f && n->son2->type_num != n->type_num){
		string flex;
		switch(n->type_num){
			case(TyInt):{
				flex = "Int";
				break;
			}
			case(TyFloat):{
				flex = "Float";
				break;
			}
			case(TyString):{
				flex = "String";
				break;
			}
		}
		cout << "cannot convert " << n->son2->lexeme << " to " << flex <<  " string "<< token->str << " position " << token->pos << endl;
		exit(-1);			
	}
	return n;
}

node* Parser::express(){
//	printf("exp\n");
	if(token->type != TyIdentifier && token->type != TyInt && token->type != TyFloat && token->type != TyStringname){
		return test();
	}
	node *n = new node;
	n = test();
	if((n->Type == TyInt || n->Type == TyFloat || n->Type == TyStringname) && token->type == TyEqual){
		node *n1 = new node;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		cop(n1, n);
		n->son1 = n1;		
		n->Type = TySet;	
		n->lexeme = "set";
		n->son2 = express();
	}	
	if(n->son2->type_num != n->type_num){
		string flex;
		switch(n->type_num){
			case(TyInt):{
				flex = "Int";
				break;
			}
			case(TyFloat):{
				flex = "Float";
				break;
			}
			case(TyString):{
				flex = "String";
				break;
			}
		}
		cout << "cannot convert " << n->son2->lexeme << " to " << flex << " string "<< token->str << " position " << token->pos << endl;
		exit(-1);			
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
	}else if(token->type == TyPrint){
		n->Type = TyPrint;
		n->lexeme = "print";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son1 = term();	
		if(token->type != TySemicolon){
			cout << "; expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
			exit(-1);
		}
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

