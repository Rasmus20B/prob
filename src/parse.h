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

#include "scope.h"
#include "token.h"
#include "ast.h"

namespace prob {

constexpr uint32_t BUF_SIZE = 4096;
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
  int lex_buf_push(std::string &key, tokenType type, int num);
  int lex_buf_push(std::string &key, tokenType type, std::string& s_literal);
  int readInt();
  int parse_token();
  int parse_function_decl(tokenType type, std::string id);
  int parse_statement();
  int parse_compound_statement();
  int parse_if_statement();
  int parse_var_declr_statement(tokenType type, std::string id);
  int parse_var_assignment_statement();
  int parse_function_or_assignment();
  ast_node *parse_expr();
  ast_node *parse_bin_op(int prec, ast_node* lhs);
  ast_node *parse_identifier_expr();
  ast_node *parse_num_literal_expr();
  ast_node *parse_primary_expr();
  ast_node *parse_paren_expr();
  int parse_identifier();
  int parse_return_statement();
  int parse_number();
  int parse_punctuation();
  int parse_keyword();
  int parse_int();

private:
  char next{};
  std::string m_path;
  std::size_t bytes_read = 0; 
  std::array<char, BUF_SIZE+2> buf1;
  std::array<char, BUF_SIZE+2> buf2;
  std::array<char, BUF_SIZE+2>::iterator bp = buf1.begin();
  std::array<char, BUF_SIZE+2>::iterator fp = buf1.begin();
  std::vector<Token> toks{};
  std::vector<Token>::iterator t_it = toks.begin();
  std::size_t lc{0};
  Stack<Scope> s;
  uint16_t func_count = 0;
  ast_node *head = new ast_node;
  std::unordered_map<tokenType, int> precs = { 
    {tokenType::IDENTIFIER, -1},
    {tokenType::NUM_LITERAL, -1},
    {tokenType::SEMICOLON, -1},
    {tokenType::ADD, 1},
  };
};
} // namespace prob
