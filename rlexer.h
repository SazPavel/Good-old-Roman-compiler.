#pragma once

#include <string>

using namespace std;

// ����������� ������� �����
// '{': LBRA, '}': RBRA, '=': EQUAL, ';': SEMICOLON, '(': LPAR, ')': RPAR, '+': PLUS, '-': MINUS, '<': LESS '>': OVER

// �������� �����
// 'si': IF, 'aliud': ELSE, 'facite': DO, 'dum': WHILE, 'totus': INT, 'verum': FLOAT 'QED': return 'SPQR': main, 'filum': string


enum LexType { TyError, TyIdentifier, TyNumber, TyString, TyLbra, TyRbra, TyEqual, TyLpar, TyRpar, TyPlus, TyMinus, TyLess, TyOver, TyIf, TyElse, TyDo, TyWhile, TySemicolon, TyInt, TyFloat, TyStringname, TyReturn, TyMain, TySet, TyEOF, Expr};
	
enum LexerMode { LeNormal, LeString, LeComment };
class Token {
	public:
		string lexeme;
		LexType type;
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
	
		bool IsIdentif(string lexeme);
		bool IsNumber(string lexeme);
		bool IsString(string lexeme);
		
		LexType getLexemeType(string lexeme);
		
		Token GetToken();
		
		Token GetTokentest(string lex);
	protected:
		int pos;//������� � ������
		int length;//����� ���������� ������
		int nustring;//����� ������
		int position;//����� �������
		string s;
		string numbers, letters, specials;
		LexerMode mode;
		bool IsLF, IsSlash;
};


