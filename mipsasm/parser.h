#pragma once

#include <vector>
#include "lexer.h"

using namespace std;

vector<Expression> parser(vector< vector<Token> >& tokens, bool* parserSuccess);
pair<Expression, bool> parseLine(vector<Token>& tokens);

pair<int, Error> instr(vector<Token>& tokens, Instruction& instruction);
pair<int, Error> arguments(Instruction& instruction, vector<Token>& tokens, int index, vector<Argument>& args);
pair<int, Error> comments(vector<Token>& tokens, int index, Comment& comment);

void printExpressions(vector<Expression>& expressions, ostream& output);