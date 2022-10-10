#include "ast.h"
#include "src/token.h"

namespace prob {

void add_node(ast_node head, tokenType type, tokenType ptype, std::string identifier) {
  ast_node n{};
  switch(type) {
    case FUNCTION_DECL:
      // add it as a function declaration
      // function declarations are all top level. i.e. 1 down from head
      n.t.m_type = ptype;
      n.t.val = identifier;
      n.type = FUNCTION_DECL;
      head.children.push_back(n);
      lodge::log.info("added function {} to ast", std::get<std::string>(n.t.val));
      return;
    case DECL_STAT:
      return;
    case COMPOUND_STAT:
      return;
    default:
      return;
  }
}
void delete_node(Token t, ast_node head) {
  return;
}
}
