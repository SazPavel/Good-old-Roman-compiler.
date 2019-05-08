#include "lexer.cpp"
#include "parser.cpp"
#include "ass.cpp"

using namespace std;

int main(){
	ifstream fin;
	fin.open("aaf.txt");
	if(!fin.is_open()) return -1;
	Lexer lexer;
	Lexer lexer1;
	Token token;
	Parser parser(&lexer, &token);
	string h;
	getline (fin, h, '\0' );
    fin.close(); 
	//cout << h << endl << endl;
	//cout << h.max_size() << endl;
	cout << "----------LEXER---------" << endl;
	string f = lexer1.run(h);
//	cout << h << endl;
	cout << "---------PARSER---------" << endl;
	node n = parser.Parun(h);
	parser.treeprint(&n, 0);
	cout << "---------Table of id---------" << endl;
	cout << "Type\tvalue\tlevel\tsublevel" << endl;
	for(int i = 0; i < SIZEID; i++){
		for(int j = 0; j < SIZEI; j++){
			if(parser.id[i][j].Type != 0)
				cout << parser.id[i][j].Type << "\t" << parser.id[i][j].value << "\t" << parser.id[i][j].level << "\t" << parser.id[i][j].sublevel << "\t" << parser.id[i][j].str<< endl;
		}
	}
	ofstream fout;
	Ass ass(&parser);
	string asst = ass.runTable();
	fout.open("aaf.S");
	fout << asst; 
	string assc = ass.runCode(&n);
	assc += "\tret\n";
	fout << assc;
    fout.close(); 
	//system("pause");
	return 0;
}
