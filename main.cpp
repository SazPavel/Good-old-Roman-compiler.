#include "lexer.cpp"
#include "parser.cpp"

using namespace std;

void treeprint(node *tree, int n) {
  if (tree) {
    //cout << tree->Type << "   " << tree->lexeme << endl;
	//cout << "left      ";
    treeprint(tree->son2, n + 5);
	//cout << "right     ";
	for(int i = 0; i< n; i++)
		cout << " ";
	cout << tree->lexeme << endl;	
    treeprint(tree->son1, n + 5);
    treeprint(tree->son3, n + 5);
  }
}

int main() {
	ifstream fin;
	fin.open("aaf.txt");
	if(!fin.is_open()) printf("g");
	Lexer lexer;
	Lexer lexer1;
	Token token;
	Parser parser(&lexer, &token);
	string h;
	getline (fin, h, '\0' );
	//cout << h << endl << endl;
	cout << "LEXER" << endl;
	string f = lexer1.run(h);
//	cout << h << endl;
	cout << "PARSER" << endl;
	node n = parser.Parun(h);
	treeprint(&n, 0);
	//system("pause");
	return 0;
}
