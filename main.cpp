#include "roman.h"

using namespace std;

int main(){
	ifstream fin;
	string name;
	cout << "Enter file name" << endl;
	cin >> name; 
	fin.open(name + ".txt");
	if(!fin.is_open()){
		cout << "NO FILE" << endl;
		return -1;
	} 
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
	fout.open(name + ".S");
	fout << asst; 
	string assc = ass.runCode(&n);
	fout << assc;
    fout.close(); 
	system("pause");
	return 0;
}
