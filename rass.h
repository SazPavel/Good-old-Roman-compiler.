#pragma once

#include "roman.h"

using namespace std;

class Ass {
	public:
		Ass(Parser *parser);
		~Ass();
		
		string runTable();
		string runCode(node *n);
		string findName(node *n);
	protected:
		string out = "\n";
		Parser *parser;
};
