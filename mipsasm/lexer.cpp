#include "lexer.h"
#include "instruction.h"

string find_word(const string& text, int index) {
	string word("");
	for (int i = index; i < text.length(); i++) {
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

bool tryparse_number(const string& word, Token& token) {
	int value = 0;
	bool bNeg = false;
	if (word[0] == '-')
		bNeg = true;

	int startIndex = (bNeg ? 1 : 0);

	for (int i = startIndex; i < word.length(); i++) {
		if (is_digit(word[i])) {
			value = value * 10;
			value = value + (word[i] - '0');
		}
		else {
			token.type = TOK_UNKNOWN;
			token.message = "Incorrect number";
			token.value = word;
			return false;
		}
	}

	token.type = TOK_INT;
	token.intValue = bNeg ? -value : value;
	return true;
}

bool tryparse_instruction(const string& word, Token& token) {
	for (int i = 0; i < instructions.size(); i++) {
		if (instructions[i].name == word) {
			token.type = TOK_INSTR;
			token.instruction = instructions[i];
			return true;
		}
	}

	token.type = TOK_UNKNOWN;
	token.message = "Incorrect instruction";
	token.value = word;
	return false;
}

vector< vector<Token> > lexer(const vector<string>& source) {
	vector< vector<Token> > tokens;
	for (int i = 0; i < source.size(); i++) {
		vector<Token> lineTokens = lexerLine(i, source[i]);
		tokens.push_back(lineTokens);
	}

	return tokens;
}

vector<Token> lexerLine(int lineNumber, const string& line) {
	vector<Token> tokens;

	for (int i = 0; i < line.length();) {
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
			int j;
			for (j = i + 1; j < line.length(); j++) {
				if (line[j] != ' ' && line[j] != '\t') break;
			}
			i = j;
			continue;
		}
		else { // case for another thing
			string word = find_word(line, i);
			if (word[0] == '$') { // register candidate
				const char* word_str = word.c_str();
				if (!strcmp(word_str, "$gp") || !strcmp(word_str, "$sp") ||
					!strcmp(word_str, "$fp") || !strcmp(word_str, "$ra") ||
					!strcmp(word_str, "$zero") || !strcmp(word_str, "$at")) {
					tokens.push_back(Token(TOK_REGISTER, word_str, lineNumber, i));
				}
				else if (word.size() == 3) {
					if (word[1] == 'v') {
						if (word[2] < '0' || word[2] > '1') {
							tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
							return tokens;
						}
					}
					else if (word[1] == 'a') {
						if (word[2] < '0' || word[2] > '3') {
							tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
							return tokens;
						}
					}
					else if (word[1] == 't') {
						if (word[2] < '0' || word[2] > '9') {
							tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
							return tokens;
						}
					}
					else if (word[1] == 's') {
						if (word[2] < '0' || word[2] > '7') {
							tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
							return tokens;
						}
					}
					else if (word[1] == 'k') {
						if (word[2] < '0' || word[2] > '1') {
							tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
							return tokens;
						}
					}
					else {
						tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
						return tokens;
					}

					tokens.push_back(Token(TOK_REGISTER, word, lineNumber, i));
				}
				else {
					tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect register name", lineNumber, i));
					return tokens;
				}
			}
			else if (word[0] == '-' || is_digit(word[0])) { // number candidate
				Token token(lineNumber, i);
				if (tryparse_number(word, token)) {
					tokens.push_back(token);
				}
				else {
					tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect number", lineNumber, i));
					return tokens;
				}
			}
			else { // instruction
				Token token(lineNumber, i);
				if (tryparse_instruction(word, token)) {
					tokens.push_back(token);
				}
				else {
					tokens.push_back(Token(TOK_UNKNOWN, word, "Incorrect instruction", lineNumber, i));
					return tokens;
				}
			}

			i = i + word.size();
			continue;
		}

		i++;
	}

	return tokens;
}

void printTokens(vector< vector<Token> >& tokens, ofstream& file) {
	for (int i = 0; i < tokens.size(); i++) {
		for (int j = 0; j < tokens[i].size(); j++) {
			tokens[i][j].print(file);
			file << " ";
		}
		file << endl;
	}
}