#pragma once

#include <vector>

using namespace std;

enum Format
{
	R,
	I,
	J
};

enum Argument {
	Register,
	SInt16
};

class Instruction {
public:
	string name;
	vector<Argument> arguments;

	Instruction() {}

	Instruction(string name, vector<Argument> arguments) {
		this->name = name;
		this->arguments = arguments;
	}
};