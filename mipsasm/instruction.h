#pragma once

#include <vector>

using namespace std;

typedef string Comment;

class Error {
public:
	Error() {
		line = column = -1;
		reason = "";
	}

	Error(string reason, int line, int column) : reason{ reason }, line{ line }, column{ column } {};

	bool isError() const { return line != -1; }

	string reason;
	int line;
	int column;
};

class Warning {
public:
	Warning() {}
	Warning(string reason, int line, int column) : reason{ reason }, line{ line }, column{ column } {};

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
	SInt16,
	UInt16
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
	Warning warning; // TODO: not implemented in this version, but they should support things like: dividing by zero!
	Error error;
};

const vector<Instruction> instructions = {
	Instruction("lui",{ RT, UInt16 }, I, 0xF),
	Instruction("add",{ RD, RS, RT }, R, 0x0, 0x20),
	Instruction("addi",{ RT, RS, SInt16 }, I, 0x08),
	Instruction("addiu",{ RT, RS, SInt16 }, I, 0x09),
	Instruction("slti",{ RT, RS, SInt16 }, I, 0xA),
	Instruction("sltiu",{ RT, RS, SInt16 }, I, 0xB),
	Instruction("andi",{ RT, RS, UInt16 }, I, 0xC),
	Instruction("ori",{ RT, RS, UInt16 }, I, 0xD),
	Instruction("xori", { RT, RS, UInt16 }, I, 0xE),
	Instruction("sll", { RD, RT, Shamt }, R, 0x0, 0x0),
	Instruction("srl", { RD, RT, Shamt }, R, 0x0, 0x02),
	Instruction("sra", { RD, RT, Shamt }, R, 0x0, 0x03),
	Instruction("sllv", {RD, RT, RS }, R, 0x0, 0x04),
	Instruction("srlv", {RD, RT, RS}, R, 0x0, 0x06),
	Instruction("srav", {RD, RT, RS}, R, 0x0, 0x07),
	Instruction("mfhi",{ RD }, R, 0x0, 0x10),
	Instruction("mthi", { RS }, R, 0x0, 0x11),
	Instruction("mflo",{ RD }, R, 0x0, 0x12),
	Instruction("mtlo", {RS}, R, 0x0, 0x13),
	Instruction("mult",{ RS, RT }, R, 0x0, 0x18),
	Instruction("multu",{ RS, RT }, R, 0x0, 0x19),
	Instruction("div",{ RS, RT }, R, 0x0, 0x1A),
	Instruction("divu",{ RS, RT }, R, 0x0, 0x1B),
	Instruction("addu",{ RD, RS, RT }, R, 0x0, 0x21),
	Instruction("sub",{ RD, RS, RT }, R, 0x0, 0x22),
	Instruction("subu",{ RD, RS, RT }, R, 0x0, 0x23),
	Instruction("and",{ RD, RS, RT }, R, 0x0, 0x24),
	Instruction("or",{ RD, RS, RT }, R, 0x0, 0x25),
	Instruction("xor", {RD, RS, RT}, R, 0x0, 0x26),
	Instruction("nor",{ RD, RS, RT }, R, 0x0, 0x27),
	Instruction("slt",{ RD, RS, RT }, R, 0x0, 0x2A),
	Instruction("sltu",{ RD, RS, RT }, R, 0x0, 0x2B)
};