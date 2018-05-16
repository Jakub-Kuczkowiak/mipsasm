#pragma once

#include <vector>

using namespace std;

class Error {
public:
	Error() {
		line = column = -1;
	}

	bool isError() const { return line != -1; }

	string reason;
	int line;
	int column;
};

enum Format
{
	R,
	I,
	J
};

enum ArgumentType {
	Register,
	SInt16
};

class Instruction {
public:
	string name;
	vector<ArgumentType> arguments;
	Format format;

	Instruction() {}

	Instruction(string name, vector<ArgumentType> arguments, Format format) {
		this->name = name;
		this->arguments = arguments;
		this->format = format;
	}
};