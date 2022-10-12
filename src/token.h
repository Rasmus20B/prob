#pragma once

#include <variant> 
#include <array>
#include <string>
#include <unordered_map> 

namespace prob {
  enum tokenType { 
    FUNCTION_DECL = 0, 
    FUNCTION_CALL = 1, 
    COMPOUND_STAT = 2, 
    DECL_STAT = 3, 
    IDENTIFIER = 4,
    OPERATOR = 5,
    NUM_LITERAL = 6,
    STRING_LITERAL = 7,
    IF = 8,
    ELSE = 9,
    FOR = 10,
    DO = 11,
    WHILE = 12,
    BREAK = 13,
    CASE = 14,
    CHAR = 15,
    COMMA = 16,
    CONST = 17,
    CONTINUE = 18,
    DEFAULT = 19,
    DOUBLE = 20,
    ENUM = 21,
    EXTERN = 22,
    FLOAT = 23,
    GOTO = 24,
    INT = 25,
    LONG = 26,
    MAIN = 27,
    REGISTER = 28,
    RETURN = 29,
    SHORT = 30,
    SIGNED = 31,
    SIZEOF = 32,
    STATIC = 33,
    STRUCT = 34,
    SWITCH = 35,
    TYPEDEF = 36,
    UNION = 37,
    UNSIGNED = 38,
    VOID = 39,
    VOLATILE = 40,
    EQ = 41,
    ADD = 42,
    MINUS = 43,
    DIVIDE = 44,
    MULTIPLY = 45,
    NOTEQ = 46,
    SEMICOLON = 47,
    OP_PAREN = 48,
    END_PAREN = 49,
    OP_CURL = 50,
    END_CURL = 51,
    DOT = 52,
    NA = 53 };

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
    { "}"       , END_CURL },
    { "*"       , MULTIPLY },
    { "-"       , MINUS },
    { ";"       , SEMICOLON }
  };

  struct Token {
  Token() = default;
  std::variant<int, std::string, char, float, long, double, short>  val;
  tokenType m_type = NA;
  std::string m_stype{};
  int line{};
  };
}
