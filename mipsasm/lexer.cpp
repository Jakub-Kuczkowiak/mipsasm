#include "lexer.h"
#include "instruction.h"
#include <iostream>
#include <string.h>

string find_word(const string& text, int index) {
	string word("");
	for (size_t i = index; i < text.length(); i++) {
		if (text[i] == '\n' || text[i] == ' ' || text[i] == '\t' || text[i] == ',') break;
		word += text[i];
	}

	return word;
}

__inline bool is_digit(char c) {
	return (c >= '0' && c <= '9');
}

__inline bool is_letter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool tryparse_number(const string& word, int& number, int index) {
	int value = 0;
	bool bNeg = false;
	if (word[index] == '-')
		bNeg = true;

	int startIndex = (bNeg ? index + 1 : index);

	for (size_t i = startIndex; i < word.length(); i++) {
		if (is_digit(word[i])) {
			value = value * 10;
			value = value + (word[i] - '0');
		}
		else {
			return false;
		}
	}

	number = bNeg ? -value : value;
	return true;
}

bool tryparse_instruction(const string& word, Token& token) {
	for (size_t i = 0; i < instructions.size(); i++) {
		if (instructions[i].name == word) {
			token.type = TOK_INSTR;
			token.instruction = instructions[i];
			return true;
		}
	}

	token.error = Error("Incorrect instruction", token.line, token.column);
	token.type = TOK_UNKNOWN;
	token.value = word;
	return false;
}

vector< vector<Token> > lexer(const vector<string>& source, bool* bSuccess) {
	vector< vector<Token> > tokens;
	for (size_t i = 0; i < source.size(); i++) {
		vector<Token> lineTokens = lexerLine((int)i, source[i]);
		if (lineTokens.size() == 0) continue;

		// here we want to check if tokens are fine.
		for (auto& token : lineTokens) {
			if (token.error.isError()) {
				cout << "Lexer error: '" << token.error.reason << "'. Line: " << token.error.line << ", Column: " << token.error.column;
				*bSuccess = false;
				return tokens;
			}
		}

		tokens.push_back(lineTokens);
	}

	*bSuccess = true;
	return tokens;
}

vector<Token> lexerLine(int lineNumber, const string& line) {
	vector<Token> tokens;

	for (size_t i = 0; i < line.length();) {
		if (line[i] == '#') { // handling comments
			string comment = line.substr(i, line.size() - i + 1);
			tokens.push_back(Token(TOK_COMMENT, comment, lineNumber, i));
			break;
		}
		else if (line[i] == '\n') {
			break;
		}
		else if (line[i] == ',') {
			tokens.push_back(Token(TOK_COMMA, lineNumber, i));
		}
		else if (line[i] == ' ' || line[i] == '\t') {
			tokens.push_back(Token(TOK_SPACE, lineNumber, i));
			size_t j;
			for (j = i + 1; j < line.length(); j++) {
				if (line[j] != ' ' && line[j] != '\t') break;
			}
			i = j;
			continue;
		}
		else { // case for another thing
			string word = find_word(line, i);
			int number;
			if (word[0] == '$') { // register candidate
				const char* word_str = word.c_str();
				if (!strcmp(word_str, "$gp")) { tokens.push_back(Token(TOK_REGISTER, word_str, 28, lineNumber, i)); }
				else if (!strcmp(word_str, "$sp")) { tokens.push_back(Token(TOK_REGISTER, word_str, 29, lineNumber, i)); }
				else if (!strcmp(word_str, "$fp")) { tokens.push_back(Token(TOK_REGISTER, word_str, 30, lineNumber, i)); }
				else if (!strcmp(word_str, "$ra")) { tokens.push_back(Token(TOK_REGISTER, word_str, 31, lineNumber, i)); }
				else if (!strcmp(word_str, "$zero")) { tokens.push_back(Token(TOK_REGISTER, word_str, 0, lineNumber, i)); }
				else if (!strcmp(word_str, "$at")) { tokens.push_back(Token(TOK_REGISTER, word_str, 1, lineNumber, i)); }
				else if (tryparse_number(word, number, 1)) {
					if (number >= 0 && number <= 31) { tokens.push_back(Token(TOK_REGISTER, word_str, number, lineNumber, i)); }
					else { tokens.push_back(Token(Error("Incorrect register name", lineNumber, i))); }
				}
				else if (word.size() == 3) {
					if (word[1] == 'v') {
						if (word[2] < '0' || word[2] > '1') {
							tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
							return tokens;
						}
						else {
							tokens.push_back(Token(TOK_REGISTER, word_str, 2 + (word[2] - '0'), lineNumber, i));
						}
					}
					else if (word[1] == 'a') {
						if (word[2] < '0' || word[2] > '3') {
							tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
							return tokens;
						}
						else {
							tokens.push_back(Token(TOK_REGISTER, word_str, 4 + (word[2] - '0'), lineNumber, i));
						}
					}
					else if (word[1] == 't') {
						if (word[2] < '0' || word[2] > '9') {
							tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
							return tokens;
						}
						else {
							if (word[2] <= '7') {
								tokens.push_back(Token(TOK_REGISTER, word_str, 8 + (word[2] - '0'), lineNumber, i));
							}
							else {
								tokens.push_back(Token(TOK_REGISTER, word_str, 24 + (word[2] - '8'), lineNumber, i));
							}
						}
					}
					else if (word[1] == 's') {
						if (word[2] < '0' || word[2] > '7') {
							tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
							return tokens;
						}
						else {
							tokens.push_back(Token(TOK_REGISTER, word_str, 16 + (word[2] - '0'), lineNumber, i));
						}
					}
					else if (word[1] == 'k') {
						if (word[2] < '0' || word[2] > '1') {
							tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
							return tokens;
						}
						else {
							tokens.push_back(Token(TOK_REGISTER, word_str, 26 + (word[2] - '0'), lineNumber, i));
						}
					}
					else {
						tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
						return tokens;
					}
				}
				else {
					tokens.push_back(Token(Error("Incorrect register name", lineNumber, i)));
					return tokens;
				}
			}
			else if (word[0] == '-' || is_digit(word[0])) { // number candidate
				int value;
				if (tryparse_number(word, value, 0)) {
					Token token(lineNumber, i);
					token.type = TOK_INT;
					token.intValue = value;
					tokens.push_back(token);
				}
				else {
					tokens.push_back(Token(Error("Incorrect number: " + word, lineNumber, i)));
					return tokens;
				}
			}
			else { // instruction
				Token token(lineNumber, i);
				if (tryparse_instruction(word, token)) {
					tokens.push_back(token);
				}
				else {
					tokens.push_back(Token(Error("Incorrect instruction", lineNumber, i)));
					return tokens;
				}
			}

			i = i + (int)word.size();
			continue;
		}

		i++;
	}

	return tokens;
}

void printTokens(vector< vector<Token> >& tokens, ofstream& file) {
	for (size_t i = 0; i < tokens.size(); i++) {
		for (size_t j = 0; j < tokens[i].size(); j++) {
			tokens[i][j].print(file);
			file << " ";
		}
		file << endl;
	}
}