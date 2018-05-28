#pragma once

#include <vector>
#include <string>
#include "instruction.h"
#include <fstream>

using namespace std;

enum Type
{
	TOK_INSTR,
	TOK_REGISTER,
	TOK_INT,
	TOK_COMMA,
	TOK_SPACE,
	TOK_COMMENT,
	TOK_NEWLINE,
	TOK_UNKNOWN
};

class Token
{
public:
	Type type;
	int intValue;
	string value;
	string message;
	int line;
	int column;
	Instruction instruction;
	Error error;

	Token(Error err) : error{ err } {};
	Token(int line, int column) : line{ line }, column{ column }, error{ Error() } {};
	Token(Type type, int line, int column) : type{ type }, line{ line }, column{ column }, error{ Error() } {};
	Token(Type type, string value, int line, int column) : type{ type }, value{ value }, line{ line }, column{ column }, error{ Error() } {};
	Token(Type type, string value, int intValue, int line, int column) : type{ type }, value{ value }, intValue{ intValue }, line { line }, column{ column }, error{ Error() } {};
	Token(Type type, string value, string message, int line, int column) : type{ type }, value{ value }, message{ message }, line{ line }, column{ column }, error{ Error() } {};

	void print(ofstream& file) {
		switch (type)
		{
		case TOK_INSTR:
			file << "TOK_INSTR";
			break;
		case TOK_REGISTER:
			file << "TOK_REGISTER(" << value << ")";
			break;
		case TOK_INT:
			file << "TOK_INT(" << intValue << ")";
			break;
		case TOK_COMMA:
			file << "TOK_COMMA";
			break;
		case TOK_SPACE:
			file << "TOK_SPACE";
			break;
		case TOK_COMMENT:
			file << "TOK_COMMENT(" << value << ")";
			break;
		case TOK_UNKNOWN:
			file << "TOK_UNKNOWN(" << value << ", " << message << ")";
			break;
		default:
			break;
		}
	}
};

__inline bool is_digit(char c);
__inline bool is_letter(char c);
bool tryparse_number(const string& word, Token& token);
bool tryparse_instruction(const string& word, Token& token);
string find_word(const string& text, int index);

vector< vector<Token> > lexer(const vector<string>& source, bool* lexerSuccess);
vector<Token> lexerLine(int lineNumber, const string& line);

void printTokens(vector< vector<Token> >& tokens, ofstream& file);


// TODO: This should be moved into instruction.h
//add:reg,reg,reg:R:0/%0/%1/%2/0/32 
//addi:reg,reg,sint16:I:8/%1/%0/%2
const vector<Instruction> instructions = {
	Instruction("lui", {RT, SInt16}, I, 0xF),
	Instruction("add", {RD, RS, RT}, R, 0x20, 0x00),
	Instruction("addi", {RT, RS, SInt16}, I, 0x08),
	Instruction("addiu", {RT, RS, SInt16}, I, 0x09),
	Instruction("slti", {RT, RS, SInt16}, I, 0xA),
	Instruction("sltiu", {RT, RS, SInt16}, I, 0xB),
	Instruction("andi", {RT, RS, SInt16}, I, 0xC), // ZeroExtImm zamiast Immediete
	Instruction("ori", {RT, RS, SInt16}, I, 0xD), // ZeroExtImm zamiast Immediete
	//Instruction("xori". {}, ), // TEGO W OGOLE NIE MA
	Instruction("sll", {RD, RT, Shamt}, R, 0x0, 0x0),
	Instruction("srl", {RD, RT, Shamt}, R, 0x0, 0x02),
	Instruction("sra", {RD, RT, Shamt}, R, 0x0, 0x03),
	//Instruction("sllv"), // TO JEST W KOLEJNEJ TABELCE O CO CHODZI?
	//Instruction("srlv"), // TO TEZ
	//Instruction("srav"), // TO TEZ
	Instruction("mfhi", {RD}, R, 0x0, 0x10), // TO TEZ I W OBU
	//Instruction("mthi"), // TO TEZ
	Instruction("mflo", {RD}, R, 0x0, 0x12), // TO TEZ I W OBU
	//Instruction("mtlo"), // TO TEZ I W OBU
	Instruction("mult", {RS, RT}, R, 0x0, 0x18), // TO TEZ I W OBU
	Instruction("multu", {RS, RT}, R, 0x0, 0x19),
	Instruction("div", {RS, RT}, R, 0x0, 0x1A),
	Instruction("divu", {RS, RT}, R, 0x0, 0x1B),
	Instruction("addu", {RD, RS, RT}, R, 0x0, 0x21),
	Instruction("sub", {RD, RS, RT}, R, 0x0, 0x22),
	Instruction("subu", {RD, RS, RT}, R, 0x0, 0x23),
	Instruction("and", {RD, RS, RT}, R, 0x0, 0x24),
	Instruction("or", {RD, RS, RT}, R, 0x0, 0x25),
	//Instruction("xor"), // XOR TYLKO W OSTATNIEJ?!
	Instruction("nor", {RD, RS, RT}, R, 0x0, 0x27),
	Instruction("slt", {RD, RS, RT}, R, 0x0, 0x2A),
	Instruction("sltu", {RD, RS, RT}, R, 0x0, 0x2B)
};