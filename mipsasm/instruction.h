#pragma once

#include <vector>

using namespace std;

typedef string Comment;

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
};

const vector<Instruction> instructions = {
	Instruction("lui",{ RT, SInt16 }, I, 0xF),
	Instruction("add",{ RD, RS, RT }, R, 0x0, 0x20),
	Instruction("addi",{ RT, RS, SInt16 }, I, 0x08),
	Instruction("addiu",{ RT, RS, SInt16 }, I, 0x09),
	Instruction("slti",{ RT, RS, SInt16 }, I, 0xA),
	Instruction("sltiu",{ RT, RS, SInt16 }, I, 0xB),
	Instruction("andi",{ RT, RS, SInt16 }, I, 0xC), // ZeroExtImm zamiast Immediete
	Instruction("ori",{ RT, RS, SInt16 }, I, 0xD), // ZeroExtImm zamiast Immediete
	//Instruction("xori". {}, ), // TEGO W OGOLE NIE MA
	Instruction("sll",{ RD, RT, Shamt }, R, 0x0, 0x0),
	Instruction("srl",{ RD, RT, Shamt }, R, 0x0, 0x02),
	Instruction("sra",{ RD, RT, Shamt }, R, 0x0, 0x03),
	//Instruction("sllv"), // TO JEST W KOLEJNEJ TABELCE O CO CHODZI?
	//Instruction("srlv"), // TO TEZ
	//Instruction("srav"), // TO TEZ
	Instruction("mfhi",{ RD }, R, 0x0, 0x10), // TO TEZ I W OBU
	//Instruction("mthi"), // TO TEZ
	Instruction("mflo",{ RD }, R, 0x0, 0x12), // TO TEZ I W OBU
	//Instruction("mtlo"), // TO TEZ I W OBU
	Instruction("mult",{ RS, RT }, R, 0x0, 0x18), // TO TEZ I W OBU
	Instruction("multu",{ RS, RT }, R, 0x0, 0x19),
	Instruction("div",{ RS, RT }, R, 0x0, 0x1A),
	Instruction("divu",{ RS, RT }, R, 0x0, 0x1B),
	Instruction("addu",{ RD, RS, RT }, R, 0x0, 0x21),
	Instruction("sub",{ RD, RS, RT }, R, 0x0, 0x22),
	Instruction("subu",{ RD, RS, RT }, R, 0x0, 0x23),
	Instruction("and",{ RD, RS, RT }, R, 0x0, 0x24),
	Instruction("or",{ RD, RS, RT }, R, 0x0, 0x25),
	//Instruction("xor"), // XOR TYLKO W OSTATNIEJ?!
	Instruction("nor",{ RD, RS, RT }, R, 0x0, 0x27),
	Instruction("slt",{ RD, RS, RT }, R, 0x0, 0x2A),
	Instruction("sltu",{ RD, RS, RT }, R, 0x0, 0x2B)
};