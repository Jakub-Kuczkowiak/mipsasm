#pragma once

#include <vector>
#include "lexer.h"

using namespace std;

typedef string Comment;

class Argument {
public:
	int intValue;
	int registerValue;
	string registerName;
};

class Expression
{
public:
	Instruction instruction;
	vector<Argument> arguments;
	Comment comment;
	Error error;

private:

};

vector<Expression> parser(vector< vector<Token> >& tokens, bool* parserSuccess);
Expression parseLine(vector<Token>& tokens);

pair<int, Error> instr(vector<Token>& tokens, Instruction& instruction);
pair<int, Error> arguments(Instruction& instruction, vector<Token>& tokens, int index, vector<Argument>& args);
pair<int, Error> comments(vector<Token>& tokens, int index, Comment& comment);

void printExpressions(vector<Expression>& expressions, ostream& output);