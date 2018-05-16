#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

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

	bool lexerSuccess;
	vector< vector<Token> > tokens = lexer(source, &lexerSuccess);
	printTokens(tokens, output);

	if (!lexerSuccess) {
		cout << "Fatal lexer compiliation" << endl;
		file.close();
		output.close();
		system("PAUSE");
		return 0;
	}

	bool parserSuccess;
	vector<Expression> expressions = parser(tokens, &parserSuccess);
	printExpressions(expressions, output);




	file.close();
	output.close();

	system("PAUSE");

	return 0;
}

// TODO: Implement registers by numbers in LEXER