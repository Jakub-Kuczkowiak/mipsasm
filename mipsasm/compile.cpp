#include "compile.h"
#include "parser.h"

#include <time.h>

void generateTests(int testsNumber) {
	ofstream output;
	output.open("tests/testrandom.in");

	srand(time(NULL));

	for (int i = 0; i < testsNumber; i++) {
		int instrId = rand() % instructions.size();
		output << instructions[instrId].name << " ";

		for (int j = 0; j < instructions[instrId].arguments.size(); j++) {
			if (instructions[instrId].arguments[j] == RT || instructions[instrId].arguments[j] == RS
				|| instructions[instrId].arguments[j] == RD) {
				// register choice
				int registerNum = rand() % 32;
				output << "$" << registerNum;
			}
			else if (instructions[instrId].arguments[j] == ArgumentType::Shamt || 
				instructions[instrId].arguments[j] == ArgumentType::SInt16) {
				int num = rand() % INT16_MAX;
				output << num;
			}

			if (j != instructions[instrId].arguments.size() - 1)
				output << ", ";
		}

		int shouldComment = rand() % 2;
		if (shouldComment) {
			output << " # comment(" << to_string(i) << ")";
		}

		output << endl;
	}

	output.close();
}

#include <algorithm>
string compileExpression(int address, Expression& expression, int& outAddress) {
	char addressStr[10];
	_itoa_s(address, addressStr, 9, 16);
	string SAddress(addressStr);
	transform(SAddress.begin(), SAddress.end(), SAddress.begin(), toupper);
	for (int i = SAddress.length(); i < 8; i++) SAddress = "0" + SAddress;

	int bytes = 0;
	if (expression.instruction.format == R) {
		int opcode = expression.instruction.opcode;
		int rs = 0;
		int rt = 0;
		int rd = 0;
		int shamt = 0;
		int funct = expression.instruction.funct;

		for (size_t i = 0; i < expression.instruction.arguments.size(); i++) {
			if (expression.instruction.arguments[i] == RS) {
				rs = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == RT) {
				rt = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == RD) {
				rd = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == Shamt) {
				shamt = expression.arguments[i].intValue;
			}
		}

		bytes |= (opcode << 26);
		bytes |= (rs << 21);
		bytes |= (rt << 16);
		bytes |= (rd << 11);
		bytes |= (shamt << 6);
		bytes |= (funct);
	}
	else if (expression.instruction.format == I) {
		int opcode = expression.instruction.opcode;

		int rs = 0;
		int rt = 0;
		int immediate = 0;

		for (size_t i = 0; i < expression.instruction.arguments.size(); i++) {
			if (expression.instruction.arguments[i] == RS) {
				rs = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == RT) {
				rt = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == ArgumentType::SInt16) {
				if (expression.arguments[i].intValue < 0)
					immediate = 65536 + expression.arguments[i].intValue;
				else
					immediate = expression.arguments[i].intValue;
			}
		}

		bytes |= (opcode << 26);
		bytes |= (rs << 21);
		bytes |= (rt << 16);
		bytes |= (immediate);
	}
	// J format is not present for now
	/*else if (expression.instruction.format == J) {
		int opcode = expression.instruction.opcode;
		int address;

		bytes |= (opcode << 26);
		bytes |= (address);
	}*/

	char bytesString[10];
	_itoa_s(bytes, bytesString, 9, 16);
	string SBytes(bytesString);
	transform(SBytes.begin(), SBytes.end(), SBytes.begin(), toupper);
	for (int i = SBytes.length(); i < 8; i++) SBytes = "0" + SBytes;

	outAddress = address + 4;

	string line = SAddress + " " + SBytes + " " + expression.instruction.name + " ";
	for (int i = 0; i < expression.instruction.arguments.size(); i++) {
		if (expression.instruction.arguments[i] == ArgumentType::RD || expression.instruction.arguments[i] == ArgumentType::RS
			|| expression.instruction.arguments[i] == ArgumentType::RT) {
			line += "$" + to_string(expression.arguments[i].registerValue);
		}
		else if (expression.instruction.arguments[i] == ArgumentType::Shamt || expression.instruction.arguments[i] == ArgumentType::SInt16) {
			line += to_string(expression.arguments[i].intValue);
		}

		if (i != expression.instruction.arguments.size() - 1) line += ", ";
	}

	if (expression.comment.length() > 0) line += "; " + expression.comment;
	return line;
}

vector<string> compile(vector<Expression>& expressions)
{
	vector<string> lines;
	int startAddress = 0;
	for (size_t i = 0; i < expressions.size(); i++) {
		string line = compileExpression(startAddress, expressions[i], startAddress);
		lines.push_back(line);
	}

	return lines;
}

void printCompiled(vector<string>& lines, ostream& output) {
	output << ".text" << endl;
	for (size_t i = 0; i < lines.size(); i++) {
		output << lines[i] << endl;
	}
}