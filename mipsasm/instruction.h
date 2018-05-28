#pragma once

#include <vector>

using namespace std;

class Error {
public:
	Error() {
		line = column = -1;
	}

	Error(string reason, int line, int column) : reason{ reason }, line{ line }, column{ column } {};

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
	RD,
	RS,
	RT,
	Shamt,
	SInt16
};

class Instruction {
public:
	string name;
	vector<ArgumentType> arguments;
	Format format;
	int opcode;
	int funct;

	Instruction() {}

	Instruction(string name, vector<ArgumentType> arguments, Format format, int opcode, int funct = 0) {
		this->name = name;
		this->arguments = arguments;
		this->format = format;
		this->opcode = opcode;
		this->funct = funct;
	}
};