//
// Created by Katsu Matsuda on 2020/04/17.
//

#include "Parser/Lexer.hpp"

using namespace xylo;

Lexer::Lexer(std::string source)
	: source(std::move(source))
{
}

void Lexer::PushSourceUnit(std::string& src, uint64_t line, uint64_t col)
{
	if (src.empty())
		return;

	source_units.push_back({ src, { line, col }});
	src = "";
}

void Lexer::PushToken(const Token& token)
{
	tokens.push_back(token);
}

bool Lexer::Divide()
{
	uint64_t idx = 0;

	uint64_t line = 0;

	uint64_t col = 0;

	std::string cur;

	while (idx < source.size())
	{
		const char ch = source[idx];

		switch (ch)
		{
		case '\n':
			PushSourceUnit(cur, line, col - cur.size());
			line++;
			col = 0;
			idx++;
			break;

		case ' ':
		case '\t':
			PushSourceUnit(cur, line, col - cur.size());
			idx++;
			col++;
			break;

		case '"':
			PushSourceUnit(cur, line, col - cur.size());
			cur = ch;
			idx += 1;
			col++;

			while (idx < source.size())
			{
				if (source.substr(idx, 2) == "\\\\")
				{
					cur += "\\";
					idx += 2;
					col += 2;
					continue;
				}

				if (source.substr(idx, 2) == "\\\"")
				{
					cur += "\"";
					idx += 2;
					col += 2;
					continue;
				}

				if (source[idx] == '"')
					break;

				if (source[idx] == '\n')
					break;

				cur += source[idx];
				idx++;
				col++;
			}

			if (source.size() <= idx || source[idx] != '"')
				return false;

			cur += '"';
			PushSourceUnit(cur, line, col - cur.size() + 1);

			idx++;
			col++;

			break;

		case '/':
			PushSourceUnit(cur, line, col - cur.size());

			if (source.substr(idx, 2) == "//")
			{
				idx += 2;
				col += 2;

				while (idx < source.size())
				{
					if (source[idx] == '\n')
					{
						idx--;
						break;
					}

					idx++;
					col++;
				}

				break;
			}

			if (source.substr(idx, 2) == "/*")
			{
				idx += 1;

				while (idx < source.size())
				{
					if (source[idx] == '\n')
					{
						line++;
						col = 0;
						continue;
					}

					if (source.substr(idx, 2) == "*/")
					{
						idx += 2;
						col += 2;
						break;
					}

					idx++;
					col++;
				}

				break;
			}

			cur = "/";
			PushSourceUnit(cur, line, col);
			idx++;
			col++;

			break;

		case '=':
		case '!':
		case '>':
		case '<':
			if (idx < source.size() - 1 && source[idx + 1] == '=')
			{
				PushSourceUnit(cur, line, col - cur.size());

				cur = source.substr(idx, 2);
				PushSourceUnit(cur, line, col);

				idx += 2;
				col += 2;

				break;
			}

		case '+':
		case '-':
		case '*':
		case '%':
		case '&':
		case '|':
		case '~':
		case '?':
		case ',':
		case '(':
		case ')':
		case '{':
		case '}':
			PushSourceUnit(cur, line, col - cur.size());
			cur = source[idx];
			PushSourceUnit(cur, line, col);
			idx++;
			col++;

			break;

		case '.':
		{
			bool left = false;
			bool right = false;

			if (0 <= idx - 1)
			{
				auto num = source[idx - 1] - '0';
				if (0 <= num && num <= 9)
					left = true;
			}

			if (idx + 1 < source.size())
			{
				auto num = source[idx + 1] - '0';
				if (0 <= num && num <= 9)
					right = true;
			}

			if (left)
			{
				cur += '.';
				idx++;
				col++;

				if (!right)
					PushSourceUnit(cur, line, col - cur.size());

				break;
			}

			PushSourceUnit(cur, line, col - cur.size());

			if (right)
			{
				cur += '.';
				idx++;
				col++;
				break;
			}

			cur = source[idx];
			PushSourceUnit(cur, line, col);
			idx++;
			col++;
		}
			break;

		default:
			cur += source[idx];
			idx++;
			col++;
			break;
		}
	}

	PushSourceUnit(cur, line, col - cur.size());

	return true;
}

Lexer::Result Lexer::Tokenize()
{
	if (!Divide())
	{
		puts("failed to tokenize source");
		return { false, tokens };
	}

	for (const auto& unit : source_units)
	{
		Token token;
		token.pos = unit.pos;
		//printf("%s (%llu,%llu)\n", unit.source.c_str(), unit.pos.line, unit.pos.col);

		if (unit.source == "func")
		{
			token.Set(Reserved::FUNC, unit.source);
		}
		else if (unit.source == "return")
		{
			token.Set(Reserved::RETURN, unit.source);
		}
		else if (unit.source == "repeat")
		{
			token.Set(Reserved::REPEAT, unit.source);
		}
		else if (unit.source == "for")
		{
			token.Set(Reserved::FOR, unit.source);
		}
		else if (unit.source == "while")
		{
			token.Set(Reserved::WHILE, unit.source);
		}
		else if (unit.source == "if")
		{
			token.Set(Reserved::IF, unit.source);
		}
		else if (unit.source == "else")
		{
			token.Set(Reserved::ELSE, unit.source);
		}
		else if (unit.source == "+")
		{
			token.Set(Symbol::PLUS, unit.source);
		}
		else if (unit.source == "+-")
		{
			token.Set(Symbol::MINUS, unit.source);
		}
		else if (unit.source == "*")
		{
			token.Set(Symbol::MUL, unit.source);
		}
		else if (unit.source == "/")
		{
			token.Set(Symbol::DIV, unit.source);
		}
		else if (unit.source == "%")
		{
			token.Set(Symbol::MOD, unit.source);
		}
		else if (unit.source == "&")
		{
			token.Set(Symbol::BIN_AND, unit.source);
		}
		else if (unit.source == "|")
		{
			token.Set(Symbol::BIN_OR, unit.source);
		}
		else if (unit.source == "^")
		{
			token.Set(Symbol::BIN_XOR, unit.source);
		}
		else if (unit.source == "~")
		{
			token.Set(Symbol::BIN_NOT, unit.source);
		}
		else if (unit.source == "&&")
		{
			token.Set(Symbol::AND, unit.source);
		}
		else if (unit.source == "||")
		{
			token.Set(Symbol::OR, unit.source);
		}
		else if (unit.source == "!")
		{
			token.Set(Symbol::NOT, unit.source);
		}
		else if (unit.source == "==")
		{
			token.Set(Symbol::EQUAL, unit.source);
		}
		else if (unit.source == "!=")
		{
			token.Set(Symbol::NOT_EQUAL, unit.source);
		}
		else if (unit.source == ">")
		{
			token.Set(Symbol::GREATER, unit.source);
		}
		else if (unit.source == ">=")
		{
			token.Set(Symbol::GREATER_EQ, unit.source);
		}
		else if (unit.source == "<")
		{
			token.Set(Symbol::LESS, unit.source);
		}
		else if (unit.source == ">=")
		{
			token.Set(Symbol::LESS_EQ, unit.source);
		}
		else if (unit.source == "=")
		{
			token.Set(Symbol::ASSIGN, unit.source);
		}
		else if (unit.source == "?")
		{
			token.Set(Symbol::QUE, unit.source);
		}
		else if (unit.source == ",")
		{
			token.Set(Symbol::COMMA, unit.source);
		}
		else if (unit.source == ".")
		{
			token.Set(Symbol::DOT, unit.source);
		}
		else if (unit.source == "(")
		{
			token.Set(Symbol::LPAREN, unit.source);
		}
		else if (unit.source == ")")
		{
			token.Set(Symbol::RPAREN, unit.source);
		}
		else if (unit.source == "{")
		{
			token.Set(Symbol::LBRACKET, unit.source);
		}
		else if (unit.source == "}")
		{
			token.Set(Symbol::RBRACKET, unit.source);
		}
		else
		{
			auto ival = Str2Int(unit.source);
			if (std::get<0>(ival))
			{
				token.Set(std::get<1>(ival), unit.source);
				PushToken(token);
				continue;
			}

			auto dval = Str2Float(unit.source);
			if (std::get<0>(dval))
			{
				token.Set(std::get<1>(dval), unit.source);
				PushToken(token);
				continue;
			}

			token.Set(unit.source);
		}

		PushToken(token);
	}

	return { true, tokens };
}

std::tuple<bool, int64_t> Lexer::Str2Int(const std::string& str)
{
	int64_t res = 0;

	int64_t d = 1;

	for (int64_t i = str.size() - 1; i >= 0; i--)
	{
		auto num = str[i] - '0';

		if (num < 0 || 9 < num)
			return std::make_tuple(false, 0);

		res += d * num;
		d *= 10;
	}

	return std::make_tuple(true, res);
}

std::tuple<bool, double> Lexer::Str2Float(const std::string& str)
{
	uint64_t dot_idx = str.size();

	for (int64_t i = 1; i < str.size(); i++)
	{
		if (str[i] == '.')
		{
			dot_idx = i;
			break;
		}
	}

	if (dot_idx == str.size())
		return std::make_tuple(false, 0);

	double res = 0;

	double d = 1;
	for (int64_t i = dot_idx - 1; i >= 0; i--)
	{
		auto num = str[i] - '0';

		if (num < 0 || 9 < num)
			return std::make_tuple(false, 0);

		res += d * num;
		d *= 10;
	}

	d = 0.1;
	for (int64_t i = dot_idx + 1; i < str.size(); i++)
	{
		auto num = str[i] - '0';

		if (num < 0 || 9 < num)
			return std::make_tuple(false, 0);

		res += d * num;
		d *= 0.1;
	}

	return std::make_tuple(true, res);
}
