#pragma once

#include <variant> 
#include <array>
#include <string>
#include <unordered_map> 

namespace prob {
  enum tokenType { FUNCTION, IDENTIFIER, OPERATOR, IF, ELSE, FOR, DO, WHILE, BREAK, CASE, CHAR, COMMA, CONST, CONTINUE, DEFAULT, DOUBLE, ENUM, EXTERN, FLOAT, GOTO, INT, LONG, MAIN, REGISTER, RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF, UNION, UNSIGNED, VOID, VOLATILE, EQ, ADD, MINUS, DIVIDE, MULTIPLY, NOTEQ, SEMICOLON, OP_PAREN, END_PAREN, OP_CURL, END_CURL, DOT, NA };

  const std::unordered_map<std::string, tokenType> TKeywords = {
    { "break"   , BREAK },
    { "case"    , CASE },
    { "char"    , CHAR },
    { "continue", CONTINUE },
    { "default" , DEFAULT },
    { "do"      , DO },
    { "double"  , DOUBLE },
    { "else"    , ELSE },
    { "enum"    , ENUM },
    { "extern"  , EXTERN },
    { "float"   , FLOAT },
    { "goto"    , GOTO },
    { "if"      , IF },
    { "int"     , INT },
    { "long"    , LONG },
    { "main"    , MAIN },
    { "register", REGISTER},
    { "return"  , RETURN },
    { "short"   , SHORT },
    { "signed"  , SIGNED },
    { "sizeof"  , SIZEOF },
    { "static"  , STATIC },
    { "STRUCT"  , STRUCT },
    { "SWITCH"  , SWITCH },
    { "typedef" , TYPEDEF },
    { "union"   , UNION },
    { "unsigned", UNSIGNED },
    { "void"    , VOID },
    { "volatile", VOLATILE },
    { "while"   , WHILE }
  };

  const std::unordered_map<std::string, tokenType> TOps = {
    { "+"       , ADD },
    { "="       , EQ },
    { "("       , OP_PAREN },
    { ")"       , END_PAREN },
    { "{"       , OP_CURL },
    { "*"       , MULTIPLY },
    { "-"       , MINUS },
    { ";"       , SEMICOLON }
  };

  struct Token {
  Token() = default;
  std::variant<int, char, float, long, double, short>  val;
  tokenType m_type;
  std::string m_stype;
  };
}
