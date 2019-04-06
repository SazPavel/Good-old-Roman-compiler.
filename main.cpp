#include <string>
#include <iostream>
#include <fstream>
#include "lexer.cpp"

using namespace std;

int main() {
	ifstream fin;
	fin.open("yyysuk");
	//if(!fin.is_open()) printf("g");
	Lexer lexer;
	string h="";
	getline (fin, h, '\0' );
	cout << h << endl << endl;
	h = lexer.run(h);
	//cout << h;
	//system("pause");
	return 0;
}
