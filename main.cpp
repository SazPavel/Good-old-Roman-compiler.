
#include "lexer.cpp"
#include "parser.cpp"

using namespace std;

int main() {
	ifstream fin;
	fin.open("aaf.txt");
	if(!fin.is_open()) return(-1);
	Lexer lexer;
	Parser parser(&lexer);
	string h;// ="SPQR si(a paritas 3) a = 3;";
	getline (fin, h, '\0' );
	cout << h << endl << endl;
	h = lexer.run(h);
	//cout << h << endl;
	cout << endl;
	Node n = parser.Parun(h);
	//system("pause");
	return 0;
}
