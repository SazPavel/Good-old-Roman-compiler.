#include "roman.h"


using namespace std;

string mastoi(string str, int *count){
	int found = str.find("[");
	int found2 = str.find("]");
	string counts = str.substr(found+1, found2-2);
	*count = stoi(counts);
	str = str.substr(0, found);
	return str;
}

string itofl(string str){
	return str +".0";
}

string ftoin(string str){
	int found = str.find(".");
	str = str.substr(0, found);
	return str;
}

void cop(node *n1, node *n){
	n1->lexeme = n->lexeme;
	n1->son1 = n->son1;
	n1->son2 = n->son2;
	n1->son3 = n->son3;
	n1->Type = n->Type;
	n1->count = n->count;
	n1->type_num = n->type_num;
	n1->level = n->level;
	n1->sublevel = n->sublevel;
	n->lexeme = "";
	n->count = 0;
	n->son1 = NULL;
	n->son2 = NULL;
	n->son3 = NULL;
	n->Type = 0;
}


int Parser::found_str_id(node *tree){
	int idn = (hash_fn(tree->son1->lexeme))%SIZEID;
	for(int i = SIZEI - 1; i >= 0; i--){
		if((tree->son1->lexeme == id[idn][i].value || tree->son1->lexeme == id[idn][i].str) && id[idn][i].level <= tree->son1->level){
		//	cout << tree->son1->lexeme << "   " << id[idn][i].value << endl;
			id[idn][i].str = tree->son2->lexeme;
			return idn;
		}		
	}
	for(int i = 0; i < SIZEI; i++){
		if(id[idn][i].Type == 0){
			id[idn][i].Type = TyString;
			id[idn][i].BaseType = TyString;
			id[idn][i].str = tree->son1->lexeme;
			id[idn][i].value = tree->son1->lexeme;
			id[idn][i].level = 0;
			id[idn][i].sublevel = 0;
			return idn;
		}
	}
	return -1;
}

void Parser::treeprint(node *tree, int n){
    if (tree) {
    	if(tree->Type == Expr && (tree->son1->Type == TyMasI || tree->son1->Type == TyMasF) && tree->son1->son1 == NULL)
    		tree->son1->Type = 0;
  		if(n > 1) cout << "|" ;
  		for(int i = 1; i < n; i++)
			cout << " ";
		if(n > 1) cout << "=> " ;
			cout << tree->lexeme << "  " << tree->Type << "  " << tree->type_num << "  " << tree->count << endl;
    	treeprint(tree->son1, n + 3);	
    	treeprint(tree->son2, n + 3);
    	treeprint(tree->son3, n + 3);
		if((tree->Type == TySet && tree->son1->Type == TyStringname && tree->son2->Type == TyString) || (tree->Type == TyPrint && tree->son1->Type == TyString)){
			int s = found_str_id(tree);
			if(s == -1){
				cout << " Strange error " << endl;
			}else{
				if(tree->Type == TySet){
					tree->son1 = NULL;
					tree->son2 = NULL;
					tree->Type = 0;
				}
			}
			
		}
    }
}

int Parser::found_id(Token *token, node *n, int f, int mas){
	int idn = (hash_fn(token->lexeme))%SIZEID;
	if(f){			
		for(int i = 0; i < SIZEI; i++){
			if(id[idn][i].level == level && id[idn][i].sublevel == sublevel[level])
				return -1;	
			if(id[idn][i].Type == 0){
				
				id[idn][i].Type = token->type;
				id[idn][i].count = 1;				
				id[idn][i].BaseType = token->type;
				id[idn][i].value = token->lexeme;
				id[idn][i].level = level;
				id[idn][i].sublevel = sublevel[level];
				n->level = id[idn][i].level;
				n->sublevel = id[idn][i].sublevel;
				if(mas){
					if(id[idn][i].Type == TyInt){
						id[idn][i].Type = TyMasI;
						id[idn][i].masi = new int[mas];
						id[idn][i].count = mas;
						token_old = token->lexeme;
						*token = lexer->GetToken();
						if(token->lexeme != "="){
							cout << "need inicialization" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
							exit(-1);
						}
						for(int j = 0; j < mas; j++){
							token_old = token->lexeme;
							*token = lexer->GetToken();
							if(token->type != TyNumberI){
								cout << "need integer" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
								exit(-1);
							}
							id[idn][i].masi[j] = stoi(token->lexeme);
						}
						token_old = token->lexeme;
						*token = lexer->GetToken();
				/*		for(int j = 0; j < mas; j++)
							cout << id[idn][i].masi[j] << " ";
						cout << endl;*/
					}
					if(id[idn][i].Type == TyFloat){
						id[idn][i].Type = TyMasF;
						id[idn][i].masf = new float[mas];
						id[idn][i].count = mas;
						token_old = token->lexeme;
						*token = lexer->GetToken();
						if(token->lexeme != "="){
							cout << "need inicialization" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
							exit(-1);
						}
						for(int j = 0; j < mas; j++){
							token_old = token->lexeme;
							*token = lexer->GetToken();
							if(token->type != TyNumberF){
								cout << "need float" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
								exit(-1);
							}
							id[idn][i].masf[j] = stof(token->lexeme);
						}
						token_old = token->lexeme;
						*token = lexer->GetToken();
				/*		for(int j = 0; j < mas; j++)
							cout << id[idn][i].masf[j] << " ";
						cout << endl;*/
					}
				}
				
				return idn;
			}
		}
	}else{
		for(int i = SIZEI - 1; i >= 0; i--){
			if(token->lexeme == id[idn][i].value && id[idn][i].level <= level){
				if(mas > id[idn][i].count){
					cout << "overlimited mass" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
					exit(-1);
				}
				if(mas){
					n->count = mas;
				}
				n->lexeme = id[idn][i].value;
				n->Type = id[idn][i].Type;
				if(id[idn][i].Type == TyStringname)
					id[idn][i].Type = TyString;
				n->type_num  = id[idn][i].BaseType;
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
	level = 0;
	levelflag = 1;
	for(int i = 0; i < 10; i++){
		sublevel[i] = 0;
	}
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
		int mas = 0;
		if(token->type != TyIdentifier){
			if(token->type == TyMas){
				token->lexeme = mastoi(token->lexeme, &mas);
			}else{
				cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
		}
		token->type = n->Type;
    	if(found_id(token, n, 1, mas) == -1){
			cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		if(mas){
			n->Type = TyMasI;	
		}else{
			n->lexeme = token->lexeme;
			*token = lexer->GetToken();
		}
		return n;   	
    }
    if(token->type == TyFloat){
		node *n = new node;
		n->type_num = n->Type = TyFloat;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		int mas = 0;
		if(token->type != TyIdentifier){
			if(token->type == TyMas){
				token->lexeme = mastoi(token->lexeme, &mas);
			}else{
				cout << "term expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
		}		
		token->type = n->Type;	
    	if(found_id(token, n, 1, mas) == -1){
			cout << token->lexeme << " redeclaration of " << " string " << token->str << " position " << token->pos << " after " << token_old  << endl;
			exit(-1);
		}		
		if(mas){
			n->Type = TyMasF;	
		}else{
			n->lexeme = token->lexeme;
			*token = lexer->GetToken();
		}
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
		int mas = 0;
		token->lexeme = mastoi(token->lexeme, &mas);
    	if(found_id(token, n, 0, mas) == -1){
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
			if(n->type_num == TyString && n->Type != TyPlus){
				cout << "error string arithemetics" << " string "<< token->str << " position " << token->pos << endl;
				exit(-1);
			}
			if(n->son2->type_num != n->type_num){
				if(n->type_num == TyString || n->son2->type_num == TyString){
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
				if(n->son2->type_num == TyInt && n->type_num == TyFloat){
					n->son2->lexeme = itofl(n->son2->lexeme);
					n->son2->Type = TyNumberF;
					n->son2->type_num = TyFloat;
				}
				if(n->son2->type_num == TyFloat && n->type_num == TyInt){
					n->son2->lexeme = ftoin(n->son2->lexeme);
					n->son2->Type = TyNumberI;
					n->son2->type_num = TyInt;
				}
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
			if(n->type_num == TyString){
				cout << "error string arithemetics" << " string "<< token->str << " position " << token->pos << " after " << token_old  << endl;
				exit(-1);
			}
			
			if(n1->son2->type_num != n1->type_num){
				if(n1->type_num == TyString || n1->son2->type_num == TyString){
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
				if(n1->son2->type_num == TyInt && n1->type_num == TyFloat){
					n1->son2->lexeme = itofl(n1->son2->lexeme);
					n->son2->Type = TyNumberF;
					n->son2->type_num = TyFloat;
				}
				if(n1->son2->type_num == TyFloat && n1->type_num == TyInt){
					n->son2->lexeme = ftoin(n1->son2->lexeme);
					n->son2->Type = TyNumberI;
					n->son2->type_num = TyInt;
				}
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
	if(token->type != TyIdentifier && token->type != TyInt && token->type != TyFloat && token->type != TyStringname && token->type != TyMas){
		return test();
	}
	node *n = new node;
	n = test();
	if((n->Type == TyInt || n->Type == TyFloat || n->Type == TyStringname || n->Type == TyMasI || n->Type == TyMasF) && token->type == TyEqual){
		node *n1 = new node;
		token_old = token->lexeme;
		*token = lexer->GetToken();
		cop(n1, n);
		n->son1 = n1;		
		n->Type = TySet;
		n->lexeme = "set";
		n->son2 = express();
	}
	
	if((n->Type == TyInt || n->Type == TyFloat || n->Type == TyMas) && token->type == TyLpar){
		token_old = token->lexeme;
		*token = lexer->GetToken();
		while(token->type != TyRpar){
			node *n1 = new node;
			cop(n1, n);
			n->son1 = n1;		
			n->Type = Func;
			n->lexeme = "func";
			n->son2 = term();
			levelflag += 1;
			if(n->son2->Type != TyInt && n->son2->Type != TyFloat && n->son2->Type != TyMasI && n->son2->Type != TyMasF && n->son2->Type != TyNumberI && n->son2->Type != TyNumberF){
				cout << "Verum or totus expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
			if(n->son1->Type != Func){
				n->son1->count = levelflag;
				levelflag = 0;
			}
		}
		token_old = token->lexeme;
		*token = lexer->GetToken();
	}
	if(n->son2 != NULL){
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
		if(level > 9){
			cout << "error array id" << endl;
			exit(-1);
		}
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
		sublevel[level] += 1;
		level -= 1;
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
	}else if(token->type == TyReturn){
		n->Type = TyReturn;
		n->lexeme = "return";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		n->son1 = term();
		if(token->type != TySemicolon || (n->son1->Type != TyInt && n->son1->Type != TyFloat)){
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

node* Parser::func(){
	node *node1 = new node;
	if(token->type != TyDef){
		return NULL;
	}	
	node1->Type = Manus;
	node1->lexeme = "Manus";
	token_old = token->lexeme;
	*token = lexer->GetToken();
	node *n1 = new node;
	node1->son1 = n1;
	n1->Type = TyDef;
	n1->lexeme = "Def";
	n1->son1 = term();
	if(n1->son1->Type != TyInt && n1->son1->Type != TyFloat){
		cout << "Verum or totus expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
		exit(-1);
	}
	int tmp = 0;
	node *n2 = new node;
	if(token->lexeme == "("){
		level += 1;		
		token_old = token->lexeme;
		*token = lexer->GetToken();
		while(token->lexeme != ")"){
			node *n3 = new node;
			cop(n3, n2);
			n2->son1 = term();
            n2->son2 = n3;
			n2->lexeme = "temp";
			n2->Type = TyTemp;
			if(n2->son1->Type != TyInt && n2->son1->Type != TyFloat){
				cout << "Verum or totus expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
				exit(-1);
			}
			tmp++;
		}
		level -= 1;
	}else{
		cout << "( expected" << " string " << token->str << " position " << token->pos << " between " << token_old << " & " << token->lexeme << endl;
		exit(-1);
	}
	token_old = token->lexeme;
	*token = lexer->GetToken();
	node1->son1->count = tmp;
    node1->son1->son2 = n2;
    while(n2->son2->Type == TyTemp){
    	n2 = n2->son2;
	}
	n2->son2 = step(1);
	node1->son2 = func();
	return node1;
}

node Parser::Parun(string s){
	lexer->start(s);
	node node1;
	lexer->nustring = 0;
	token_old = token->lexeme;
	*token = lexer->GetToken();
	level = 0;
	node1.son2 = func();
	if(token->type == TyMain){
		node *nod1 = new node;
		node1.son1 = nod1;
		node1.Type = Manus;
		node1.lexeme = "Manus";
		
		nod1->Type = TyMain;
		nod1->lexeme = "SPQR";
		token_old = token->lexeme;
		*token = lexer->GetToken();
		nod1->son1 = step(1);
	}
	if(token->type != TyEOF){
		cout << "Invalid statement syntax  " << token->type << "    " << token->lexeme << endl;
		exit(-1);		
	}
	return node1;	
}

