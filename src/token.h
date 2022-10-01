#pragma once

#include <variant> 

namespace prob {
  enum tokenType { FUNCTION, IDENTIFIER, OPERATOR, IF, ELSE, FOR, DO, WHILE, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DOUBLE, ENUM, EXTERN, FLOAT, GOTO, INT, LONG, REGISTER, RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF, UNION, UNSIGNED, VOID, VOLATILE, EQ, ADD, MINUS, DIVIDE, MULTIPLY, NOTEQ, SEMICOLON, OP_PAREN, END_PAREN, OP_CURL, END_CURL };

  struct Token {
public:
  Token() = default;
private:
  std::variant<int, char, float, long, double, short>  val;
  tokenType m_type;
  };
}
