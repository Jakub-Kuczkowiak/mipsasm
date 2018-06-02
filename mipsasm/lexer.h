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
	//string message;
	int line;
	int column;
	Instruction instruction;
	Error error;

	Token(Error err) : error{ err } {};
	Token(int line, int column) : line{ line }, column{ column }, error{ Error() } {};
	Token(Type type, int line, int column) : type{ type }, line{ line }, column{ column }, error{ Error() } {};
	Token(Type type, string value, int line, int column) : type{ type }, value{ value }, line{ line }, column{ column }, error{ Error() } {};
	Token(Type type, string value, int intValue, int line, int column) : type{ type }, value{ value }, intValue{ intValue }, line{ line }, column{ column }, error{ Error() } { };

	void print(ofstream& file) {
		switch (type)
		{
		case TOK_INSTR:
			file << "TOK_INSTR(" << instruction.name << ")";
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
			file << "TOK_UNKNOWN(" << value;
			break;
		default:
			break;
		}
	}
};

__inline bool is_digit(char c);
__inline bool is_letter(char c);
bool tryparse_number(const string& word, int& number, int index);
bool tryparse_instruction(const string& word, Token& token);
string find_word(const string& text, int index);

vector< vector<Token> > lexer(const vector<string>& source, bool* lexerSuccess);
vector<Token> lexerLine(int lineNumber, const string& line);

void printTokens(vector< vector<Token> >& tokens, ofstream& file);