#pragma once

#include "parser.h"

void generateTests(int testsNumber);

string compileExpression(int address, Expression& expression, int& outAddress);
vector<string> compile(vector<Expression>& expressions);
void printCompiled(vector<string>& lines, ostream& output);