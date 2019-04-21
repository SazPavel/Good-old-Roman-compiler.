#include "lexer.cpp"
#include "parser.cpp"

using namespace std;

void treeprint(node *tree, int n) {
  if (tree) {
	if(n > 1) cout << "|" ;
	for(int i = 1; i < n; i++)
		cout << " ";
	if(n > 1) cout << "=> " ;
	cout << tree->lexeme << endl;
    treeprint(tree->son1, n + 3);	
    treeprint(tree->son2, n + 3);
    treeprint(tree->son3, n + 3);
  }
}

int main() {
	ifstream fin;
	fin.open("aaf.txt");
	if(!fin.is_open()) return -1;
	Lexer lexer;
	Lexer lexer1;
	Token token;
	Parser parser(&lexer, &token);
	string h;
	getline (fin, h, '\0' );
	//cout << h << endl << endl;
	//cout << h.max_size() << endl;
	cout << "----------LEXER---------" << endl;
	string f = lexer1.run(h);
//	cout << h << endl;
	cout << "---------PARSER---------" << endl;
	node n = parser.Parun(h);
	treeprint(&n, 0);
	cout << "---------Table of id---------" << endl;
	cout << "Type\tvalue\tlevel\tsublevel" << endl;
	for(int i = 0; i < SIZEID; i++){
		for(int j = 0; j < SIZEI; j++){
			if(parser.id[i][j].Type != 0)
				cout << parser.id[i][j].Type << "\t" << parser.id[i][j].value << "\t" << parser.id[i][j].level << "\t" << parser.id[i][j].sublevel << endl;
		}
	}
	//system("pause");
	return 0;
}
