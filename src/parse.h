#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <lodge.h>

namespace prob {
struct Node {
  Node *left;
  Node *right;
};
class AST {
public:
  AST() = default;
  AST(const std::string path);
  [[nodiscard]] int readSource(const std::string path);
  [[nodiscard]] std::string getSource();
  void parse();

private:
  Node *head;
  char next{};
  std::string source{};
};
} // namespace prob
