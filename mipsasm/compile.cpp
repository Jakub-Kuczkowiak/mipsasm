#include "compile.h"
#include "parser.h"

string compileExpression(int address, Expression& expression, int& outAddress) {
	char addressStr[9];
	_itoa_s(address, addressStr, 8, 16);
	string SAddress(addressStr);

	int bytes = 0;
	if (expression.instruction.format == R) {
		int opcode = expression.instruction.opcode;
		int rs = 0;
		int rt = 0;
		int rd = 0;
		int shamt = 0;
		int funct = expression.instruction.funct;

		for (int i = 0; i < expression.instruction.arguments.size(); i++) {
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

		for (int i = 0; i < expression.instruction.arguments.size(); i++) {
			if (expression.instruction.arguments[i] == RS) {
				rs = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == RT) {
				rt = expression.arguments[i].registerValue;
			}
			else if (expression.instruction.arguments[i] == ArgumentType::SInt16) {
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

	char bytesString[9];
	_itoa_s(bytes, bytesString, 8, 16);
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