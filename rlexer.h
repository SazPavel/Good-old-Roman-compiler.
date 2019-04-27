#pragma once

#include <string>

using namespace std;

// специальные символы языка
// '{': LBRA, '}': RBRA, '=': EQUAL, ';': SEMICOLON, '(': LPAR, ')': RPAR, '+': PLUS, '-': MINUS, '<' minor: LESS '>'major: OVER

// ключевые слова
// 'si': IF, 'aliud': ELSE, 'facite': DO, 'dum': WHILE, 'totus': INT, 'verum': FLOAT 'QED': return 'SPQR': main, 'filum': string, 'paritas': equal


enum LexType { TyError, TyIdentifier, TyNumberF, TyNumberI, TyMas,  TyString, TyLbra, TyRbra, TyEqual, TyLpar, TyRpar, TyPlus, TyMinus, TyMul, TyDivision, TyLess, TyOver, TyIf, TyElse, TyDo, TyWhile, TySemicolon, TyInt, TyFloat, TyStringname, TyReturn, TyMain, TySet, TyEql, TyEOF, Expr, SEQ, LessEq, OverEq};
	
enum LexerMode { LeNormal, LeString, LeComment };
class Token {
	public:
		Token();
		~Token();
		string lexeme;
		int type;
		int str;
		int pos;
};
	
class Lexer {
	public:
		Lexer();
		~Lexer();	
		
		bool IsLetter(char a);
		bool IsNum(char a);
		bool IsSpecial(char a);
		
		void start(string s);
		string step();
		string run(string s);
		
		bool IsMas(string lexeme);
		bool IsIdentif(string lexeme);
		bool IsNumberI(string lexeme);
		bool IsNumberF(string lexeme);
		bool IsString(string lexeme);
		
		LexType getLexemeType(string lexeme);
		
		Token GetToken();
		
		Token GetTokentest(string lex);
		int pos;//позиция в строке
		int length;//длина полученной строки
		int nustring;//номер строки
		int position;//общая позиция
	protected:
		string s;
		string numbers, letters, specials;
		LexerMode mode;
		bool IsLF, IsSlash;
};


