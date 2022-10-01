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

constexpr uint32_t BUF_SIZE = 18;
struct Node {
  Node *left;
  Node *right;
};
class AST {
public:
  AST() = default;
  AST(const std::string path);
  [[nodiscard]] int readSourceToBuffer(std::array<char, BUF_SIZE + 2>& buf);
  void set_file(std::string path);
  // [[nodiscard]] std::string getSource();
  void lex();
  void print_tokens();

private:
  char next{};
  tokenType comp_keywords(std::string &key);
  int lex_buf_push(std::string &key, tokenType type);
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
