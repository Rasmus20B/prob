#pragma once

#include <stdint.h>
#include <vector>

#include <lodge.h>

#include "token.h"

namespace prob {

struct Zone {

};

struct ast_node {
  ast_node() {}
  std::vector<ast_node*> children{};
  uint16_t id;
  tokenType type{};
  Token t;

};
extern ast_node *add_node(ast_node *head, tokenType type, tokenType ptype, std::string identifier);
extern ast_node *add_node(ast_node *head, Token t);
extern ast_node *merge_nodes(ast_node *a, ast_node *b);
extern void delete_node(ast_node head);
extern void print_tree(ast_node *head);
}
