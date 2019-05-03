#include "roman.h"

using namespace std;

Ass::Ass(Parser *parser){
	this->parser = parser;
}

Ass::~Ass(){
}

string replaceDot(string& str){
	int pos = str.find('.');
	return str.replace(pos, 1, ",");
}

string Ass::runTable(){
	string out = ".data\nprintf_format:\n\t.string \"%d\\n\"\n";
	for(int i = 0; i < SIZEID; i++){
		for(int j = 0; j < SIZEI; j++){
			if(parser->id[i][j].Type != 0){
				out +=  parser->id[i][j].value;
				out +=  to_string(parser->id[i][j].level);
				out +=  to_string(parser->id[i][j].sublevel);
				out += ": \n\t";
				switch(parser->id[i][j].Type){
					case TyStringname:{
						out += ".string";
						break;
					}
					case TyInt:{
						out += ".long";
						break;
					}
					case TyFloat:{
						out += ".float";
						break;
					}
				}
				out += "\n";
			}
		}
	}	
	return out;
}

string Ass::findName(node *n){
	string outt;
	int idn = (parser->hash_fn(n->lexeme))%SIZEID;	
	for(int i = 0; i < SIZEI; i++){
		if(parser->id[idn][i].level == n->level && parser->id[idn][i].sublevel == n->sublevel){
			outt +=  parser->id[idn][i].value;
			outt +=  to_string(parser->id[idn][i].level);
			outt +=  to_string(parser->id[idn][i].sublevel);
			return outt;
		}
	}
	cout << "error in table of id"  << endl;
	exit(-1);
}

string Ass::runCode(node *n){
	switch(n->Type){
		case TyMain:{
			out += ".globl _main\n.section .text  \n   .def	_main;	.scl	2;	.type	32;	.endef\n_main:\n";
			runCode(n->son1);
			break;
		}
		case TyNumberF:{
			string name = replaceDot(n->lexeme);
			out += "\tmov $";
			out += name;
			out += ", %eax";
			out += "\n";
			break;
		}
		case TyNumberI:{
			out += "\tmov $";
			out += n->lexeme;
			out += ", %eax";
			out += "\n";
			break;
		}
		case TyInt:{
			string name = findName(n);
			out += "\tmov ";
			out += "(";
			out += name;
			out += "), %eax\n";
			break;
		}
		case TyFloat:{
			string name = findName(n);
			out += "\tmov ";
			out += "(";
			out += name;
			out += "), %eax\n";
			break;
		}
		case TyPlus:{	
			runCode(n->son2);
			//from eax to edx
			out += "\tmov %eax, %edx\n";
			runCode(n->son1);
			out += "\tadd %edx, %eax\n";
			break;
		}
		case TyMinus:{	
			runCode(n->son2);
			out += "\tmov %eax, %edx\n";
			//from eax to edx
			runCode(n->son1);
			out += "\tsub %edx, %eax\n";
			break;
		}
		case TyMul:{
			runCode(n->son1);	
			//from eax to ?
			runCode(n->son2);
			//mul
			break;
		}
		case TyDivision:{
			runCode(n->son1);	
			//from eax to ?
			runCode(n->son2);
			//division
			break;
		}
		case TyPrint:{
			runCode(n->son1);
			out += "\tpush %eax\n";      
    		out += "\tpush $printf_format\n";
		  	out += "\tcall  _printf\n"; 			
			break;
		}
		case SEQ:{
			runCode(n->son1);
			runCode(n->son2);
			break;
		}
		case Expr:{
			runCode(n->son1);
			//hmm
			break;
		}
		case TySet:{
			runCode(n->son2);	
			//eax		
			string name = findName(n->son1);
			out += "\tmov %eax, (";
			out += name;
			out += ")\n";
			break;
		}
	}
	return out;		
}
