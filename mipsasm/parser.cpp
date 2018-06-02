#include "parser.h"
#include "lexer.h"
#include <vector>
#include <iostream>

using namespace std;

pair<Expression, bool> parseLine(vector<Token>& tokens)
{
	// line can be just a comment as well
	Comment commentTest;
	pair<int, Error> resTest = comments(tokens, 0, commentTest);
	if (resTest.first != -1) {
		return pair<Expression, bool>(Expression(), false);
	}

	// if not, it's a full expression
	Expression expression;

	Instruction instruction;
	pair<int, Error> res = instr(tokens, instruction);
	if (res.first == -1) {
		expression.error = res.second;
		return pair<Expression, bool>(expression, true);
	}

	expression.instruction = instruction;

	vector<Argument> args;
	res = arguments(instruction, tokens, res.first, args);
	if (res.first == -1) {
		expression.error = res.second;
		return pair<Expression, bool>(expression, true);
	}

	expression.arguments = args;

	Comment comment;
	res = comments(tokens, res.first, comment);
	if (res.first == -1) {
		expression.error = res.second;
		return pair<Expression, bool>(expression, true);
	}

	expression.comment = comment;

	return pair<Expression, bool>(expression, true);
}

void findWarnings(Expression& expression) {
	// here we add all exception to be checked
	
}

vector<Expression> parser(vector< vector<Token> >& tokens, bool* parserSuccess) {
	vector<Expression> result;
	for (int i = 0; i < tokens.size(); i++) {
		pair<Expression, bool> exp = parseLine(tokens[i]);
		if (exp.second) { // it's not a comment
			Expression& expression = exp.first;
			if (expression.error.isError()) {
				*parserSuccess = false;
				cout << "Parser error: '" << expression.error.reason << "'. Line: " << expression.error.line << ", Column: " << expression.error.column;
				return result;
			}

			result.push_back(expression);
		}
	}

	*parserSuccess = true;
	return result;
}

pair<int, Error> instr(vector<Token>& tokens, Instruction& instruction) {
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].type == TOK_SPACE) continue;
		else if (tokens[i].type == TOK_INSTR) {
			instruction = tokens[i].instruction;
			return pair<int, Error>(i + 1, Error());
		}
		else {
			Error error;
			error.column = tokens[i].column;
			error.line = tokens[i].line;
			error.reason = "Expected instruction.";
			return pair<int, Error>(-1, error);
		}
	}

	return pair<int, Error>(-1, Error());
}

pair<int, Error> arguments(Instruction& instr, vector<Token>& tokens, int index, vector<Argument>& arguments) {
	int argsProcessed = 0;
	bool bSearchComma = false;
	for (int i = index; i < tokens.size(); i++) {
		if (argsProcessed == instr.arguments.size()) {
			return pair<int, Error>(i + 1, Error());
		}
		if (tokens[i].type == TOK_SPACE) continue;

		if (bSearchComma) {
			if (tokens[i].type == TOK_COMMA) {
				bSearchComma = false;
				continue;
			}
			else {
				return pair<int, Error>(-1, Error("Comma expected", tokens[i].line, tokens[i].column));
			}
		}

		if (instr.arguments[argsProcessed] == ArgumentType::RD || instr.arguments[argsProcessed] == ArgumentType::RS || 
			instr.arguments[argsProcessed] == ArgumentType::RT) {
			if (tokens[i].type == TOK_REGISTER) {
				Argument argument;
				argument.registerName = tokens[i].value;
				argument.registerValue = tokens[i].intValue;
				arguments.push_back(argument);
				argsProcessed++;
				bSearchComma = true;
			}
			else {
				return pair<int, Error>(-1, Error("Expected register argument.", tokens[i].line, tokens[i].column));
			}

		}
		else if (instr.arguments[argsProcessed] == ArgumentType::SInt16) {
			if (tokens[i].type == TOK_INT) {
				if (tokens[i].intValue > INT16_MAX || tokens[i].intValue < INT16_MIN) {
					return pair<int, Error>(-1, Error("Immediate argument must be in interval -32767 - 1 to 32767", tokens[i].line, tokens[i].column));
				}

				Argument argument;
				argument.intValue = tokens[i].intValue;
				arguments.push_back(argument);
				argsProcessed++;
				bSearchComma = true;
			}
			else {
				return pair<int, Error>(-1, Error("Expected immediate argument.", tokens[i].line, tokens[i].column));
			}
		}
		else if (instr.arguments[argsProcessed] == ArgumentType::UInt16) {
			if (tokens[i].type == TOK_INT) {
				if (tokens[i].intValue < 0 || tokens[i].intValue > UINT16_MAX) {
					return pair<int, Error>(-1, Error("Zero-Immediate argument must be in interval 0 to 65535", tokens[i].line, tokens[i].column));
				}

				Argument argument;
				argument.intValue = tokens[i].intValue;
				arguments.push_back(argument);
				argsProcessed++;
				bSearchComma = true;
			}
			else {
				return pair<int, Error>(-1, Error("Expected immediate argument.", tokens[i].line, tokens[i].column));
			}
		}
		else if (instr.arguments[argsProcessed] == ArgumentType::Shamt) {
			if (tokens[i].type == TOK_INT) {
				if (tokens[i].intValue > 31 || tokens[i].intValue < 0) {
					return pair<int, Error>(-1, Error("Shamt argument must be in interval 0 to 31", tokens[i].line, tokens[i].column));
				}

				Argument argument;
				argument.intValue = tokens[i].intValue;
				arguments.push_back(argument);
				argsProcessed++;
				bSearchComma = true;
			}
			else {
				return pair<int, Error>(-1, Error("Expected shamt argument.", tokens[i].line, tokens[i].column));
			}
		}
	}

	if (argsProcessed == instr.arguments.size()) {
		return pair<int, Error>(tokens.size(), Error());
	}
	return pair<int, Error>(-1, Error());
}

pair<int, Error> comments(vector<Token>& tokens, int index, Comment& comment) {
	for (int i = index; i < tokens.size(); i++) {
		if (tokens[i].type == TOK_SPACE) continue;

		if (tokens[i].type == TOK_COMMENT) {
			comment = tokens[i].value; // TODO: check if its .value here
			return pair<int, Error>(i + 1, Error());
		}
		else {
			Error error;
			error.column = tokens[i].column;
			error.line = tokens[i].line;
			error.reason = "Too many arguments here";
			return pair<int, Error>(-1, error);
		}
	}

	return pair<int, Error>(tokens.size(), Error());
}

void printExpressions(vector<Expression>& expressions, ostream& output) {
	for (int i = 0; i < expressions.size(); i++) {

	}
}