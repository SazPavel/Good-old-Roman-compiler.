#pragma once

#include "roman.h"

using namespace std;

class Ass {
	public:
		Ass(Parser *parser);
		~Ass();
		
		string runTable();
		string runCode(node *n, int flag = 0);
		string findName(node *n);
	protected:
    	hash <std::string> hash_fn;
		int num_loop;
		string out = "\n";
		Parser *parser;
};
