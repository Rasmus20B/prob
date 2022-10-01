#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <array>

#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <lodge.h>

#include "token.h"

namespace prob {

constexpr uint32_t BUF_SIZE = 4 * 1024;
struct Node {
  Node *left;
  Node *right;
};
class AST {
public:
  AST() = default;
  AST(const std::string path);
  [[nodiscard]] int readSourceToBuffer(const std::string path);
  // [[nodiscard]] std::string getSource();
  void lex();

private:
  Node *head;
  char next{};
  std::array<char, BUF_SIZE+1> source{};
  std::array<char, BUF_SIZE+1>::iterator bp = source.begin();
  std::array<char, BUF_SIZE+1>::iterator fp = source.begin();
};
} // namespace prob
