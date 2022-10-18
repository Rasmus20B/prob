#include "ast.h"
#include "src/token.h"

// namespace prob {

// ast_node *add_node(ast_node *head, Token t) {
//   // PRECEDENCE
//   ast_node *n = new ast_node;
//   if(t.m_type == tokenType::NUM_LITERAL) {
//   }
//   n->t = t;
//   switch(t.m_type) {
//     case tokenType::ADD:
//       n->type = t.m_type;
//       n->children.push_back(head);
//       lodge::log.info("added binary op {} to tree", n->type);
//       return n;
//     case tokenType::NUM_LITERAL:
//       if(!head->type) {
//         head = n;
//       } else if (head->type == tokenType::ADD) {
//         head->children.push_back(n);
//       }
//       return head;
//     case tokenType::IDENTIFIER:
//       if(!head->type) {
//         head = n;
//       } else if (head->type == tokenType::ADD) {
//         head->children.push_back(n);
//         lodge::log.info("added identifier: {} to tree", t.m_stype);
//       }
//       return head;
//     default:
//       break;
//   }
//   return head;
// }
//
// ast_node *add_node(ast_node *head, tokenType type, tokenType ptype, std::string identifier) {
//   ast_node *n = new ast_node;
//   switch(type) {
//     case FUNCTION_DECL:
//       // add it as a function declaration
//       // function declarations are all top level. i.e. 1 down from head
//       n->t.m_type = ptype;
//       n->t.val = identifier;
//       n->type = FUNCTION_DECL;
//       head->children.push_back(n);
//       lodge::log.info("added function {} to ast", std::get<std::string>(n->t.val));
//       break;
//     case DECL_STAT:
//       break;;
//     case COMPOUND_STAT:
//       break;
//     default:
//       break;
//   }
//   return head;
// }
//
// void print_tree(ast_node *head) {
//   if(head) {
//     print_tree(head->children[1]);
//     lodge::log.info("FOUND {}", head->t.m_stype);
//     print_tree(head->children[2]);
//   }
// }
//
// //
// // void delete_node(Token t, ast_node head) {
// //   return;
// // }
// }
