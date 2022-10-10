#pragma once

#include <stdint.h>
#include <vector>

#include <lodge.h>

#include "token.h"

namespace prob {
struct ast_node {
  std::vector<ast_node> children;
  uint16_t id;
  tokenType type{};
  Token t;

};
extern void add_node(ast_node head, tokenType type, tokenType ptype, std::string identifier);
void delete_node(Token t, ast_node head);
}
