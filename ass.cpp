#include "roman.h"

using namespace std;

Ass::Ass(Parser *parser){
	this->parser = parser;
	num_loop = 0;
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
	string out = ".data\n.float0:\n\t.long\n";
	for(int i = 0; i < SIZEID; i++){
		for(int j = 0; j < SIZEI; j++){
			if(parser->id[i][j].Type != 0 && parser->id[i][j].Type != TyString && parser->id[i][j].Type != TyStringname){
				if(parser->id[i][j].Type == TyMasI){
					out += "a";
					out +=  to_string(hash_fn(parser->id[i][j].value));
					out +=  to_string(parser->id[i][j].level);
					out +=  to_string(parser->id[i][j].sublevel);
					out += ": \n";
					for(int k = 0; k < parser->id[i][j].count; k++){
						out += "\t.long ";
						out += to_string(parser->id[i][j].masi[k]);
						out += "\n";
					}
				}else if(parser->id[i][j].Type == TyMasF){
					out += "a";
					out +=  to_string(hash_fn(parser->id[i][j].value));
					out +=  to_string(parser->id[i][j].level);
					out +=  to_string(parser->id[i][j].sublevel);
					out += ": \n";
					for(int k = 0; k < parser->id[i][j].count; k++){
						out += "\t.long 0b";
						myfloat var;
						var.f = parser->id[i][j].masf[k];
						out += printIEEE(var);
						out += "\n";
					}
				}else{
					out += "a";
					out +=  to_string(hash_fn(parser->id[i][j].value));
					out +=  to_string(parser->id[i][j].level);
					out +=  to_string(parser->id[i][j].sublevel);
					out += ": \n\t";
					out += ".long 0";
					out += "\n";
				}
			}
		}
	}	
	out += "\n.prints_format:\n\t.string \"%s\\0\"\n.printi_format:\n\t.string \"%d\\0\"\n.printf_format:\n\t.string \"%f\\0\"\n";
	
	for(int i = 0; i < SIZEID; i++){
		for(int j = 0; j < SIZEI; j++){
			if(parser->id[i][j].Type == TyString){
				out += "a";
				out +=  to_string(hash_fn(parser->id[i][j].value));
				out +=  to_string(parser->id[i][j].level);
				out +=  to_string(parser->id[i][j].sublevel);
				out += ": \n\t.string ";
				out +=  parser->id[i][j].str;
				out += "\n";
			}
		}
	}
	return out;
}

string Ass::findName(node *n, int flag){
	string outt;
	int idn = (parser->hash_fn(n->lexeme))%SIZEID;	
	for(int i = 0; i < SIZEI; i++){
		if(parser->id[idn][i].level == n->level && parser->id[idn][i].sublevel == n->sublevel){
			if(flag)
				return parser->id[idn][i].value;
			outt += "a";
			outt += to_string(hash_fn(parser->id[idn][i].value));
			outt += to_string(parser->id[idn][i].level);
			outt += to_string(parser->id[idn][i].sublevel);
			return outt;
		}
	}
	exit(-1);
}

string Ass::runCode(node *n, int flag){
	switch(n->Type){
		case Manus:{
			if(n->son1 != NULL)
				runCode(n->son1);
			if(n->son2 != NULL)
				runCode(n->son2);
			break;
		}
		case TyDef:{
			out += "\n\t.globl ";
			out += n->son1->lexeme;
			out += "\n\t.def ";
			out += n->son1->lexeme;
			out += ";	.scl	2;	.type	32;	.endef\n";
			out += n->son1->lexeme;
			out += ":\n";
			out += "\tpush\t%rbp\n\tmov\t%rsp, %rbp\n";
			runCode(n->son2);
			break;
		}
		case TyTemp:{
			if(n->son1){
				string name = findName(n->son1);
				out += "\tmov\t";
				switch(flag){
					case(0):{
						out += "%ecx, ";
						break;
					}
					case(1):{
						out += "%edx, ";
						break;
					}
					case(2):{
						out += "%r8d, ";
						break;
					}
					case(3):{
						out += "%r9d, ";
						break;
					}
					default:{
						out += to_string(flag * 8 + 16);
						out += "(%rsp)";
						break;
					}
				}
				out += "(";				
				out += name;
				out += ")\n";
			}
			runCode(n->son2, flag + 1);			
			break;
		}
		case Func:{
			out += "\tmov\t(";
			string name = findName(n->son2);
			out += name;
			out += "), ";
			switch(flag){
				case(0):{
					out += "%ecx\n";
					break;
				}
				case(1):{
					out += "%edx\n";
					break;
				}
				case(2):{
					out += "%r8d\n";
					break;
				}
				case(3):{
					out += "%r9d\n";
					break;
				}
				default:{
					out += to_string(flag * 8);
					out += "(%rsp)";
					break;
				}
			}
			if(n->son1->Type != Func){
				out += "\tcall\t";
				out += findName(n->son1, 1);
				out += "\n";
			}else{
				runCode(n->son1, flag + 1);
			}
			break;
		}
		case TyReturn:{
			out += "\tmov\t%rbp, %rsp\n\tpop\t%rbp\n";
			string name = findName(n->son1);
			out += "\tmov\t(";
			out += name;
			out += "), %eax\n";
			out += "\tret\n";
			break;
		}
		case TyMain:{
			out += "\n\t.text\n\t.globl main\n\t.def	main;	.scl	2;	.type	32;	.endef\nmain:\n\tpushq\t%rbp\n\tmovq\t%rsp, %rbp\n";
			runCode(n->son1);
			out += "\tmovl\t$0, %eax\n\tpopq\t%rbp\n\tret\n";
			break;
		}
		case TyNumberF:{
			myfloat var;
			var.f = stof(n->lexeme);
			string name = printIEEE(var);
			out += "\tmov\t$0b";
			out += name;
			out += ", %eax";
			out += "\n";
			break;
		}
		case TyNumberI:{
			out += "\tmov\t$";
			out += n->lexeme;
			out += ", %eax";
			out += "\n";
			break;
		}
		case TyMasI:
		case TyMasF:{
			string name = findName(n);
			out += "\tmov\t$";
			out += to_string(n->count*4);
			out += ", %ebx\n";
			out += "\tmov\t(";
			out += name;
			out += "), %eax\n";
			break;
		}
	/*	case TyString:{
			out += "\tmov ";
			out += n->lexeme;
			out += ", %eax";
			out += "\n";
			break;
		}*/
		case TyInt:{
			string name = findName(n);
			out += "\tmov\t(";
			out += name;
			out += "), %eax\n";
			break;
		}
		case TyFloat:{
			string name = findName(n);
			out += "\tmov\t(";
			out += name;
			out += "), %eax\n";
			break;
		}
		case TyStringname:{
			string name = findName(n);
			out += "\tmov\t(";
			out += ".";
			out += name;
			out += "), %eax\n";
			break;
		}
		case TyPlus:{	
			runCode(n->son2);
			//from eax to ecx
			out += "\tmov\t%eax, %ecx\n";
			runCode(n->son1);
			out += "\tadd\t%ecx, %eax\n";
			break;
		}
		case TyMinus:{	
			runCode(n->son2);
			out += "\tmov\t%eax, %ecx\n";
			//from eax to ecx
			runCode(n->son1);
			out += "\tsub\t%ecx, %eax\n";
			break;
		}
		case TyMul:{
			runCode(n->son2);
			out += "\tmov\t%eax, %ecx\n";	
			//from eax to ecx
			runCode(n->son1);
			out += "\tmul\t%ecx\n";
			//mul
			break;
		}
		case TyDivision:{
			runCode(n->son2);
			out += "\tmov\t%eax, %ecx\n";	
			//from eax to ecx
			runCode(n->son1);
			out += "\tmov\t$0, %edx\n\tdiv\t%ecx\n";
			//division
			break;
		}
		case TyPrint:{
			if(n->son1->type_num != TyStringname)
				runCode(n->son1);      
			switch(n->son1->type_num){
				case(TyInt):{
					out += "\tsubq\t$32, %rsp\n\tmovq\t%rsp, %rbp\n\tpushq\t%rbp\n\tmov\t%eax, %edx\n\tlea\t.printi_format(%rip), %rcx\n\tcall\tprintf\n\tmov\t$0, %eax\n\taddq\t$32, %rsp\n\tpopq\t%rbp\n";
					break;
				}
				case(TyFloat):{
					out += "\tsubq\t$64, %rsp\n\tmovq\t%rsp, %rbp\n\tpushq\t%rbp\n";
					if(n->son1->Type == TyFloat){
						string name = findName(n->son1);
						out += "\tmovss\t.";
						out += name;
						out += "(%rip), %xmm0\n";
					}else{
						out += "\tmov\t%eax, .float0(%rip)\n\tmovss\t.float0(%rip), %xmm0\n";
					}
    				out += "\tmovss\t%xmm0, -4(%rbp)\n\tcvtss2sd\t-4(%rbp), %xmm0\n\tmovq\t%xmm0, %rax\n\tmovq\t%rax, %rdx\n\tmovq\t%rdx, -24(%rbp)\n\tmovsd\t-24(%rbp), %xmm0\n\tmovapd\t%xmm0, %xmm1\n\tmovq\t%rax, %rdx\n\tlea\t.printf_format(%rip), %rcx\n\tcall\tprintf\n\tmov\t$0, %eax\n\taddq\t$64, %rsp\n\tpopq\t%rbp\n";		
					break;
				}
				case(TyStringname):{
					string name = findName(n->son1);
    				out += "\tsubq\t$48, %rsp\n\tmovq\t%rsp, %rbp\n\tpushq\t%rbp\n\tleaq\t.";
    				out += 	name;
					out += "(%rip), %rdx\n\tleaq\t.prints_format(%rip), %rcx\n\tcall\tprintf\n\tmov\t$0, %eax\n\taddq\t$48, %rsp\n\tpopq\t%rbp\n";
					break;
				}
				case(TyString):{
					string name = findName(n->son1);
    				out += "\tsubq\t$32, %rsp\n\tmovq\t%rsp, %rbp\n\tpushq\t%rbp\n\tleaq\t.";
    				out += 	name;
					out += "(%rip), %rcx\n\tcall\tprintf\n\tmov\t$0, %eax\n\taddq\t$32, %rsp\n\tpopq\t%rbp\n";
					break;
				}
				case(TyMasI):{
					out += "\tsubq\t$32, %rsp\n\tmovq\t%rsp, %rbp\n\tpushq\t%rbp\n\tmov\t%eax, %edx\n\tlea\t.printi_format(%rip), %rcx\n\tcall\tprintf\n\tmov\t$0, %eax\n\taddq\t$32, %rsp\n\tpopq\t%rbp\n";
					break;
				}
			}	
				
			break;
		}
		case TyLess:{
			runCode(n->son2);
			out += "\tmov\t%eax, %ecx\n";
			runCode(n->son1);
			if(flag == 0)
				out += "\tcmp\t%ecx, %eax\n\tjl";
			else
				out += "\tcmp\t%ecx, %eax\n\tjnl";
			break;
		}
		case TyOver:{
			runCode(n->son2);
			out += "\tmov\t%eax, %ecx\n";
			runCode(n->son1);
			if(flag == 0)
				out += "\tcmp\t%ecx, %eax\n\tjg";
			else
				out += "\tcmp\t%ecx, %eax\n\tjng";
			break;
		}
		case TyEql:{
			runCode(n->son1);
			out += "\tmov\t%eax, %ecx\n";
			runCode(n->son2);
			if(flag == 0)
				out += "\tcmp\t%ecx, %eax\n\tje";
			else
				out += "\tcmp\t%ecx, %eax\n\tjne";
			break;
		}
		case TyDo:{
			out += "DO";
			out += to_string(num_loop);	
			out += ":\n";
			runCode(n->son1);
			runCode(n->son2);
			out += " DO";
			out += to_string(num_loop);
			out += "\n";
			num_loop += 1;
			break;
		}
		case TyWhile:{
			out += "WHILE";
			out += to_string(num_loop);	
			out += ":\n";
			runCode(n->son1, 1);
			out += " DO";
			out += to_string(num_loop);
			out += "\n";
			runCode(n->son2);
			out += "\tjmp\tWHILE";
			out += to_string(num_loop);	
			out += "\n";
			out += "DO";
			out += to_string(num_loop);	
			out += ":\n";
			num_loop += 1;
			break;
		}
		case TyIf:{
			runCode(n->son1, 1);
			out += " IF";
			out += to_string(num_loop);	
			out += "\n";
			runCode(n->son2);			
			out += "IF";
			out += to_string(num_loop);	
			out += ":\n";
			num_loop += 1;
			break;
		}
		case TyElse:{
			runCode(n->son1, 1);
			out += " ELSE";
			out += to_string(num_loop);	
			out += "\n";
			runCode(n->son2);
			out += "\tjmp\tIF";	
			out += to_string(num_loop);	
			out += "\n";	
			out += "ELSE";
			out += to_string(num_loop);	
			out += ":\n";
			runCode(n->son3);		
			out += "IF";
			out += to_string(num_loop);	
			out += ":\n";
			num_loop += 1;
			break;
		}
		case SEQ:{
			runCode(n->son1);
			runCode(n->son2);
			break;
		}
		case Expr:{
			runCode(n->son1);
			break;
		}
		case TySet:{
			runCode(n->son2);
			string name = findName(n->son1);
			if(n->son1->Type == TyMasI || n->son1->Type == TyMasF){
				out += "\tmov\t$";
				out += to_string(n->son1->count*4);
				out += ", %ebx\n";
				out += "\tmov\t(";
				out += name;
				out += "), %eax\n\tmov\t$0, %ebx\n";
			}else{
				out += "\tmov\t%eax, (";
				out += name;
				out += ")\n";			
			}
			break;
		}
		default:{
			break;
		}
	}
	return out;		
}
