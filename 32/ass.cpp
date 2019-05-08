#include "roman.h"

using namespace std;

Ass::Ass(Parser *parser){
	this->parser = parser;
}

Ass::~Ass(){
}

typedef union { 
  
    float f; 
    struct
    { 
	    unsigned int mantissa : 23; 
        unsigned int exponent : 8; 
        unsigned int sign : 1; 
  
    } raw; 
} myfloat; 
   
string printBinary(int n, int i) 
{ 
	string out = "";
    int k; 
    for (k = i - 1; k >= 0; k--) { 
  
        if ((n >> k) & 1) 
            out += "1";
        else
            out += "0"; 
    } 
    return out;
} 

string printIEEE(myfloat var) 
{ 
    string out = "";
    out += to_string(var.raw.sign);
    out += printBinary(var.raw.exponent, 8); 
    out += printBinary(var.raw.mantissa, 23); 
    return out;
} 
  
string Ass::runTable(){
	string out = ".section .data\nprinti_format:\n\t.string \"%d\\n\"\nprintf_format:\n\t.string \"%f\\n\"\n";
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
			out += ".globl _main\n.extern _printf\n.section .text  \n   .def	_main;	.scl	2;	.type	32;	.endef\n_main:\n";
			runCode(n->son1);
			break;
		}
		case TyNumberF:{
			myfloat var;
			var.f = stof(n->lexeme);
			string name = printIEEE(var);
			out += "\tmov $0b";
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
			//from eax to ebx
			out += "\tmov %eax, %ebx\n";
			runCode(n->son1);
			out += "\tadd %ebx, %eax\n";
			break;
		}
		case TyMinus:{	
			runCode(n->son2);
			out += "\tmov %eax, %ebx\n";
			//from eax to ebx
			runCode(n->son1);
			out += "\tsub %ebx, %eax\n";
			break;
		}
		case TyMul:{
			runCode(n->son2);
			out += "\tmov %eax, %ebx\n";	
			//from eax to ebx
			runCode(n->son1);
			out += "\tmul %ebx\n";
			//mul
			break;
		}
		case TyDivision:{
			runCode(n->son2);
			out += "\tmov %eax, %ecx\n";	
			//from eax to ecx
			runCode(n->son1);
			out += "\tmov $0, %edx\n\tdiv %ecx\n";
			//division
			break;
		}
		case TyPrint:{
			runCode(n->son1);      
			switch(n->son1->type_num){
				case(TyInt):{
					out += "\tpush %eax\n";
    				out += "\tpush $printi_format\n";
					break;
				}
				case(TyFloat):{
    				out += "\tpushl %ebp\n\tmovl %esp, %ebp\n";//т.к. превращается в double надо записать в стек еще одно число, которое будет перезаписано
					out += "\tpush %eax\n";
    				out += "\tflds (%esp)\n\tfstpl (%esp)\n\tpush $printf_format\n";
					break;
				}
				case(TyString):{
    				out += "\tpush $prints_format\n";
					break;
				}
			}
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
