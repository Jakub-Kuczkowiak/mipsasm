#include "compile.h"
#include "parser.h"

string compileExpression(int address, Expression& expression, int& outAddress) {
	string line = "";
	char addressStr[9];
	itoa(address, addressStr, 16);
	string SAddress(addressStr);

	int bytes = 0;
	if (expression.instruction.format == R) {
		int opcode = expression.instruction.opcode;
		int rs = expression.arguments[0].registerValue;
		int rt = expression.arguments[1].registerValue;
		int rd = expression.arguments[2].registerValue;
		int shamt = expression.instruction.shamt;
		int funct = expression.instruction.funct;

		bytes |= (opcode << 26);
		bytes |= (rs << 21);
		bytes |= (rt << 16);
		bytes |= (rd << 11);
		bytes |= (shamt << 6);
		bytes |= (funct);
	}
	else if (expression.instruction.format == I) {
		int opcode = expression.instruction.opcode;

		int rs;
		int rt;
		int immediate;

		bytes |= (opcode << 26);
		bytes |= (rs << 21);
		bytes |= (rt << 16);
		bytes |= (immediate);
	}
	else if (expression.instruction.format == J) {
		int opcode = expression.instruction.opcode;
		int address;

		bytes |= (opcode << 26);
		bytes |= (address);
	}

	char bytesString[9];
	itoa(bytes, bytesString, 16);
	string SBytes(bytesString);

	outAddress = address + 4;

	string line = SAddress + " " + SBytes + "; " + expression.comment;
	return line;
}

vector<string> compile(vector<Expression>& expressions)
{
	vector<string> lines;
	int startAddress = 0;
	for (int i = 0; i < expressions.size(); i++) {
		string line = compileExpression(startAddress, expressions[i], startAddress);
		lines.push_back(line);
	}

	return lines;
}

void printCompiled(vector<string>& lines, ostream& output) {
	for (int i = 0; i < lines.size(); i++) {
		output << lines[i];
	}
}