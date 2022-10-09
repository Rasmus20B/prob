#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <array>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <lodge.h>

#include "token.h"

namespace prob {

constexpr uint32_t BUF_SIZE = 4096;
struct Node {
  Node *left;
  Node *right;
};
class AST {
public:
  AST() = default;
  AST(const std::string &path);
  [[nodiscard]] int readSourceToBuffer(std::array<char, BUF_SIZE + 2>& buf);
  void set_file(std::string_view path);
  void lex();
  void print_tokens();
  int parse_program();

private:
  tokenType comp_keywords(std::string &key);
  int lex_buf_push(std::string &key, tokenType type);
  int parse_token(Token t);
  int parse_function(Token t);
  int parse_identifier(Token t);
  int parse_number(Token t);
  int parse_punctuation(Token t);
  int parse_keyword(Token t);

private:
  char next{};
  std::string m_path;
  std::size_t bytes_read = 0; 
  std::array<char, BUF_SIZE+2> buf1;
  std::array<char, BUF_SIZE+2> buf2;
  std::array<char, BUF_SIZE+2>::iterator bp = buf1.begin();
  std::array<char, BUF_SIZE+2>::iterator fp = buf1.begin();
  std::vector<Token> toks{};
  std::size_t lc{0};
};
} // namespace prob
