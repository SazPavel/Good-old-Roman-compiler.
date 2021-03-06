#include "roman.h"

using namespace std;

string type_to_str(int type){
	switch(type){
		case TyError: return "error";
		case TyIdentifier: return "identif";
		case TyNumberI: return "num i";
		case TyNumberF: return "num f";
		case TyMas: return "mas";
		case TyMasS: return "mas";
		case TyString: return "string";
		case TyLbra: return "LBRA";
		case TyRbra: return "RBRA";
		case TyEqual: return "equal";
		case TyLpar: return "Lpar";
		case TyRpar: return "Rpar";
		case TyPlus: return "plus";
		case TyMinus: return "minus";
		case TyMul: return "mul";
		case TyDivision: return "division";
		case TyLess: return "less";
		case TyOver: return "over";
		case TyIf: return "if";
		case TyElse: return "else";
		case TyDo: return "do";
		case TyWhile: return "while";
		case TyPrint: return "print";
		case TySemicolon: return ";";
		case TyInt: return "int";
		case TyFloat: return "float";
		case TyStringname: return "string";
		case TyReturn: return "return";
		case TyMain: return "main";
		case TySet: return "set";
		case TyEql: return "eql";
		case TyEOF: return "EOF";
		case TyDef: return "define";
	}

	return "wat";
}

Token::Token(){
}


Token::~Token(){
}

Lexer::Lexer(){
	numbers = ".0123456789";
	letters = "_ABCDEFGHIJKLMNOPQRSTUYWXYZabcdefghijklmnopqrstuvwxyz";
	specials = "(){};=+-*/";
	mode = LeNormal;
	length = position = nustring = pos = 0;
	IsLF = IsSlash = false;
}

Lexer::~Lexer(){
}

void Lexer::start(string s){
	this->s = s;
	length = s.length();
	position = 0;
	position = nustring = pos = 0;
	IsLF = IsSlash = false;
}

bool Lexer::IsLetter(char a){
	for(unsigned int i = 0; i < letters.length(); i++){
		if(letters[i] == a)
			return true;
	}
	return false;
}

bool Lexer::IsNum(char a){
	for(unsigned int i = 0; i < numbers.length(); i++){
		if(numbers[i] == a)
			return true;
	}
	return false;
}

bool Lexer::IsSpecial(char a){
	for(unsigned int i = 0; i < specials.length(); i++){
		if(specials[i] == a)
			return true;
	}
	return false;
}

string Lexer::step(){
	string a;
	while(position < length){
		int c = s[position];
		if(c == 10){
			pos = 0;
			nustring ++;
			IsLF = true;
		}else{
			IsLF = false;	
		}
		
		if(c < 32) 
			c = 32;//��� �� �������� ������� �� 32 ascii?
		
		if(mode == LeNormal){
			if(a.length() > 0 && !IsNum(c) && !IsLetter(c)){
				if(IsLF)
					nustring --;
				int tmp = s[position];
			
				if(tmp =='['){
					a = a + (char)tmp;
					position++;
					while(IsNum(s[position]) || IsLetter(s[position])){
						a = a + (char)s[position];
						position++;
					}
					if(s[position] == ']'){
						a = a + (char)s[position];
						position++;
					}
				}

				return a;
			}
			pos++;
			position++;
			
			if(c == '\"'){
				a = a + (char)c;
				mode = LeString;
				continue;
			}	
		
			if(c == '#'){
				mode = LeComment;
				continue;
			}
						
			if(c != 32)
				a = a + (char)c;
				
			if(a.length() > 0 && !IsNum(c) && !IsLetter(c)){
				int tmp = s[position];
				if(a == "=" && tmp == '='){
					a = a + (char)tmp;
					position++;
				}
				return a;					
			}
		}else if(mode == LeComment){
			position++;
			pos++;
			if(IsLF){
				nustring ++;
				mode = LeNormal;
			}
							
		}else if(mode == LeString){
			position++;
			pos++;
			a = a + (char)c;
			if(c == '\\'){
				IsSlash = true;
				continue;
			}
			if(c == '\"'){
				if(IsSlash == true){
					IsSlash = false;
				}else{
					mode = LeNormal;
					return a;
				}
			}else{
				IsSlash = false;
			}
		}
	}
	return a;
}

bool Lexer::IsMas(string lexeme){
	int len = lexeme.length();
	if(len == 0) return false;	
	int i;
	for (i = 0; i < len; i++) {
		if(!IsLetter(lexeme[i])){
			if(lexeme[i] != '[') return false;
			else{
				i ++;
				break;
			}
		}
	}
	for(; i< len; i++){
		if(!IsNum(lexeme[i])){
			if(lexeme[i] == ']'){
				return true;
			} 
			else
				return false;
		}	
	}
	return false;
}

bool Lexer::IsMasS(string lexeme){
	int len = lexeme.length();
	if(len == 0) return false;	
	int i;
	for (i = 0; i < len; i++) {
		if(!IsLetter(lexeme[i])){
			if(lexeme[i] != '[') return false;
			else{
				i ++;
				break;
			}
		}
	}
	for(; i< len; i++){
		if(!IsLetter(lexeme[i])){
			if(lexeme[i] == ']'){
				return true;
			} 
			else
				return false;
		}	
	}
	return false;
}

bool Lexer::IsIdentif(string lexeme) {
	int len = lexeme.length();
	if (len == 0) return false;
	
	if (!IsLetter(lexeme[0])) return false;
	
	for (int i = 1; i < len; i++) {
		if (!IsLetter(lexeme[i]) && !IsNum(lexeme[i])) return false;
	}

	return true;
}
bool Lexer::IsNumberI(string lexeme){
	int len = lexeme.length();
	if(len == 0) return false;
	for(int i = 0; i < len; i++){
		if(!isdigit(lexeme[i])) return false;
	}
	return true;
}

bool Lexer::IsNumberF(string lexeme){
	int len = lexeme.length();
	if (len == 0) return false;
	int f = 0, j = 0;
	for (int i = 0; i < len; i++) {
		if (!isdigit(lexeme[i])){
			if(lexeme[i] != '.' || f == 1) return false;	
			else		
				f = 1;
		}else
			j ++;
	}
	if(j < 1) return false;	
	return true;
}

bool Lexer::IsString(string lexeme) {
	int len = lexeme.length();
	if (len < 2) return false;

	if (lexeme[0] != '\"') return false;
	if (lexeme[len - 1] != '\"') return false;

	return true;
}

LexType Lexer::getLexemeType(string lexeme) {
		if(lexeme == "{") return TyLbra;
		if(lexeme == "}") return TyRbra;
		if(lexeme == "=") return TyEqual;
		if(lexeme == "(") return TyLpar;
		if(lexeme == ")") return TyRpar;
//		if(lexeme == "[") return TySlpar;
//		if(lexeme == "]") return TySrpar;
		if(lexeme == "+") return TyPlus;
		if(lexeme == "-") return TyMinus;
		if(lexeme == "*") return TyMul;
		if(lexeme == "/") return TyDivision;
		if(lexeme == "<" || lexeme == "minor") return TyLess;
		if(lexeme == ">" || lexeme == "major") return TyOver;
		if(lexeme == ";") return TySemicolon;
		if(lexeme == "si") return TyIf;
		if(lexeme == "aliud") return TyElse;
		if(lexeme == "facite") return TyDo;
		if(lexeme == "dum") return TyWhile;
		if(lexeme == "eventus") return TyPrint;
		if(lexeme == "totus") return TyInt;
		if(lexeme == "verum") return TyFloat;
		if(lexeme == "filum") return TyStringname;
		if(lexeme == "redi") return TyReturn;
		if(lexeme == "SPQR") return TyMain;
		if(lexeme == "definire") return TyDef;
		if(lexeme == "==" || lexeme == "paritas") return TyEql;
		if(IsIdentif(lexeme)) return TyIdentifier;
		if(IsNumberI(lexeme)) return TyNumberI;
		if(IsNumberF(lexeme)) return TyNumberF;
		if(IsString(lexeme)) return TyString;
		if(IsMas(lexeme)) return TyMas;
		if(IsMasS(lexeme)) return TyMasS;
		if(lexeme == "\0") return TyEOF;
		return TyError;
}

Token Lexer::GetTokentest(string lex){
	Token token;
	token.lexeme = lex;
	token.type = getLexemeType(lex);
	return token;
}

string Lexer::run(string s){
	start(s);
	string result;
	while(position < length){	
		string lex = step();
		result += lex + "\n";
		Token token = GetTokentest(lex);
		token.str = nustring;
		token.pos = pos - lex.length();
		cout << token.type << "\t:type  " << type_to_str(token.type) << "\t" << token.lexeme << "\t :lexeme\t" <<  token.str << " :string\t" <<  token.pos << " :position" << endl;
	}
	return result;
}

Token Lexer::GetToken(){
	string lex = this->step();
	Token token;
	token.lexeme = lex;
	token.type = getLexemeType(lex);
	token.str = nustring;
	token.pos = this->pos - token.lexeme.length();
//	cout << token.pos << "  pos  " << token.lexeme << "  lexeme " << "  string " << token.str << endl;
	if(token.type == TyError){
		cout << "Lexer error in string " << token.str << " position " << token.pos <<  "  " << token.lexeme <<endl;
		exit(1);
	}
	return token;
}

