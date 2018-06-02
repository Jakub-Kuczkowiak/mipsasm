#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "compile.h"

using namespace std;

const int MAX_LINE = 256;

int main() {
	//generateTests(20000);

	ifstream file;
	file.open("tests/testrandom.in");

	ofstream output;
	output.open("tests/testrandom.out");

	vector<string> source;

	while (!file.eof()) {
		char line[MAX_LINE];
		file.getline(line, MAX_LINE);
		source.push_back(line);
	}

	bool lexerSuccess;
	vector< vector<Token> > tokens = lexer(source, &lexerSuccess);
	//printTokens(tokens, output);

	if (!lexerSuccess) {
		cout << "Fatal lexer compiliation" << endl;
		file.close();
		output.close();
		system("PAUSE");
		return 0;
	}

	bool parserSuccess;
	vector<Expression> expressions = parser(tokens, &parserSuccess);
	//printExpressions(expressions, output);

	if (!parserSuccess) {
		cout << "Fatal parser error" << endl;
		file.close();
		output.close();
		system("PAUSE");
		return 0;
	}

	vector<string> compileLines = compile(expressions);
	printCompiled(compileLines, output);

	file.close();
	output.close();

	system("PAUSE");

	return 0;
}