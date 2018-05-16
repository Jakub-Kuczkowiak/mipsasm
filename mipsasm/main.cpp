#include <iostream>
#include <fstream>
#include "lexer.h"

using namespace std;

const int MAX_LINE = 256;

int main() {
	ifstream file;
	file.open("tests/test1a.in");

	ofstream output;
	output.open("tests/test1a.out");

	vector<string> source;

	while (!file.eof()) {
		char line[MAX_LINE];
		file.getline(line, MAX_LINE);
		source.push_back(line);
	}

	vector< vector<Token> > tokens = lexer(source);
	printTokens(tokens, output);

	file.close();
	output.close();

	return 0;
}