#include "parse.h"
#include "src/token.h"
#include <cstddef>
#include <string>

namespace prob {

void AST::set_file(std::string_view path) { m_path = path;}

void AST::print_tokens() {
  
  for(auto i: toks ) {
    if(i.m_type == NUM_LITERAL) {
      lodge::log.info("{}, {}", i.m_type, std::get<int>(i.val));
    }
    else if(i.m_type == STRING_LITERAL) {
    lodge::log.info("{}, {}", i.m_type, std::get<std::string>(i.val));
    } else {
      lodge::log.info("{}, {}", i.m_stype, i.m_type);
    }
  }
}

int AST::parse_int() {
  t_it++;
  switch(t_it->m_type) {
    case tokenType::IDENTIFIER:
      lodge::log.info("FOUND AN IDENTIFIER");
      parse_identifier();
      return 0;
    default:
      lodge::log.error("Expected identifier after typename!");
      return -1;
      break;
  }
}

int AST::parse_function_decl(tokenType ptype, std::string id) {
    
  //Add function declaration to tree as id and type   
  lodge::log.info("Found function {}", id);

  head->children[func_count] = add_node(head, FUNCTION_DECL, ptype, id);
  func_count++;
  while(t_it->m_type != END_PAREN) { t_it++; }

  lodge::log.info("Found end parenthesis");

  t_it++;
  switch(t_it->m_type) {
    case tokenType::OP_CURL:
      //start compound statement
      lodge::log.info("FOUND start of compound statement");
      parse_compound_statement();
      break;
    case tokenType::SEMICOLON:
      lodge::log.info("Found declaration without definition");
      return 0;
    default:
      lodge::log.error("Expected Semicolon or statement, found {}:{}", t_it->m_stype, t_it->m_type);
      return -1;
  }
  return 0;
}

ast_node * AST::parse_primary_expr() {
  switch(t_it->m_type) {
    case tokenType::IDENTIFIER:
      return parse_identifier_expr();
    case tokenType::NUM_LITERAL:
      return parse_num_literal_expr();
    case tokenType::OP_CURL:
      return parse_paren_expr();
    default:
      lodge::log.error("Invalid token for expression");
      return nullptr;
  }
}

ast_node * AST::parse_bin_op(int prec, ast_node* lhs) {

  Stack<Token> ops;
  while(true) {
    auto p = precs.find(t_it->m_type);
    if(p->second < prec) {
      return lhs;
    }

    t_it++;
    ast_node *rhs = new ast_node;
    rhs = parse_primary_expr();
    if(!rhs) {
      return nullptr;
    }

    auto n_p = precs.find(t_it->m_type);
    if(p->second < n_p->second) {
      rhs = parse_bin_op(p->second+1, rhs);
      if(!rhs){
        return nullptr;
      }
    }
    lhs->children.push_back(std::move(lhs));
    lhs->children.push_back(rhs);
    lhs->type = n_p->first;
  }
}

ast_node * AST::parse_paren_expr() {
  ast_node *num = new ast_node;
  num->type = NUM_LITERAL;
  t_it++;
  return num;
}
ast_node * AST::parse_identifier_expr() {
  ast_node *num = new ast_node;
  num->type = IDENTIFIER;
  t_it++;

  return num;
}

ast_node * AST::parse_num_literal_expr() {
  ast_node *num = new ast_node;
  num->type = NUM_LITERAL;
  return num;
}

ast_node *AST::parse_expr() {
  auto lhs = parse_primary_expr();
  if(lhs == nullptr) {
    return nullptr;
  }
  return parse_bin_op(0, lhs);
}

int AST::parse_var_declr_statement(tokenType type, std::string id) {
  t_it++;
  std::vector<Token> vals{};
  ast_node *e = parse_expr();
  std::cout << e->type << std::endl;
  delete e;
  return 0;
}


int AST::parse_identifier() {
  
  t_it++;
  switch(t_it->m_type) {
    case tokenType::SEMICOLON:
      lodge::log.info("Found a semicolon");
      return 0;
    case tokenType::OP_PAREN:
      lodge::log.info("Found opening paren");
      // if there is a type written before id, it's a declaration
      if((t_it - 2)->m_type == INT) {
        parse_function_decl((t_it - 2)->m_type, (t_it - 1)->m_stype);
      } else {
        // if its not, it's a function call
        while(t_it->m_type != END_PAREN) {t_it++;}
      }
      return 0;
    case tokenType::EQ:
      lodge::log.info("Found an assignment operator(=)");
      if((t_it - 2)->m_type == INT) {
        parse_var_declr_statement((t_it - 2)->m_type, (t_it - 1)->m_stype);
      } else {

      }
      return 0;
    default:
      return -1;
  }
  return 0;

}

int AST::parse_compound_statement() { 
  Scope scope{};
  auto p = s.peek();
  if(p.has_value()) {
    scope.parent = p->addr;
  }
  s.push(scope);
  while(t_it->m_type != END_CURL) { parse_statement(); }
  lodge::log.info("Finished compound statement");
  return 0; 
}

int AST::parse_var_assignment_statement() {
  return 0;
}

int AST::parse_function_or_assignment() {
  t_it++;
  switch(t_it->m_type) {
    case tokenType::OP_PAREN:
      lodge::log.info("Found function call");
      //its a function call
      return 0;
    case tokenType::EQ:
      if((t_it - 2)->m_type == INT) {
        parse_var_declr_statement((t_it - 2)->m_type, (t_it - 1)->m_stype);
      } else {
        // var assignment rather than decl
        parse_var_assignment_statement();
      }
      break;
    default:
      lodge::log.error("Expected paren or equals");
      return -1;
  }
  return 0;
}

int AST::parse_return_statement() {

  std::vector<Token> vals{};
  t_it++;
  auto e = parse_expr();
  std::cout << "root: " << e->type << std::endl;
  if(e->type == tokenType::SEMICOLON) {
    std::cout << e->children[0]->type << std::endl;
    std::cout << e->children[1]->type << std::endl;
  }
  delete e;
  return 0;
}

int AST::parse_statement() {

  t_it++;
  switch(t_it->m_type) {
    case tokenType::OP_CURL:
      parse_compound_statement();
      return 0;
    case tokenType::IF:
      // parse_if_statement();
      return 0;
    case tokenType::SEMICOLON:
      return 0;
    case tokenType::IDENTIFIER:
      // Check if it's a function or a variable assignment via '('
      parse_function_or_assignment();
      return 0;
    case tokenType::NUM_LITERAL:
      lodge::log.info("FOUND A NUM LITERAL {}", std::get<int>(t_it->val));
      return 0;
    case tokenType::RETURN:
      lodge::log.info("Found a return statement");
      parse_return_statement();
      return 0;
    default:
      return 0;
  }

  return 0;

}

int AST::parse_token() {

  /* When an { is found, this is the start of a compoudnd statement */
  switch(t_it->m_type) {
    case tokenType::IDENTIFIER:
      parse_identifier();
      break;
    case tokenType::INT:
      parse_int();
      break;
    case tokenType::NUM_LITERAL:
      break;
    case tokenType::OP_CURL:
      //Begin compound statement
      break;
    default:
      return -1;
      break;
  }
  t_it++;
  return 0;
}
 
int AST::parse_program() {
  t_it = toks.begin();
  while(true) {
    if(parse_token() == -1) {
      /* PROBLEM SEG FAULT */
      // print_tree(head);
      return -1;
    }
    t_it++;
  }
  return 0;
}

#ifdef __linux__
int AST::readSourceToBuffer(std::array<char, BUF_SIZE + 2>& buf) {
  buf.fill(0);
  auto fd = open(m_path.c_str(), O_RDONLY);
  if (!fd) {
    lodge::log.error("Error opening file");
    return -1;
  }
  if(lseek(fd, bytes_read, SEEK_SET ) == -1) {
    lodge::log.error("Unable to seek input offset");
    return -1;
  }

  posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);

  if((bytes_read += (read(fd, buf.begin(), BUF_SIZE + 2))) == (size_t)-1) {
    lodge::log.error("Unable to read file");
    return -1;
  }
  auto i = buf.end();
  *i = EOF;
  return 0;
}
#elif __WIN32__
int AST::readSourceToBuffer(std::array<char, BUF_SIZE + 2>& buf) {

  buf.fill(0);
  std::ifstream ifs;
  ifs.open(source);

  ifs.read(buf.begin, BUF_SIZE + 2);
  bytes_read += ifs.gcount();

  auto i = buf.end();
  *i = EOF;
  return 0;
}
#endif


tokenType AST::comp_keywords(std::string &key) {
  auto s = TKeywords.find(key);
  if(s != TKeywords.end()) {
    return s->second;
  } else {
    return IDENTIFIER;
  }
}
int AST::lex_buf_push(std::string &key, tokenType type, int num) {

  Token t{};
  if(key.size() > 0) {
    t.m_type = comp_keywords(key);
    t.m_stype = key;
    t.line = lc;
    toks.push_back(t);
  }
  key.clear();
  /* We dont care about newlines, spaces, etc */
  if(type == NA) {
    return 0;
  }
  t.m_type = type;
  t.m_stype.clear();
  t.val = num;
  for(auto &i: TOps) {
    if(t.m_type == i.second) {
      t.m_stype = i.first;
    }
  }
  toks.push_back(t);
  return 0;
}
int AST::lex_buf_push(std::string &key, tokenType type) {

  Token t{};
  if(key.size() > 0) {
    t.m_type = comp_keywords(key);
    t.m_stype = key;
    t.line = lc;
    toks.push_back(t);
  }
  key.clear();
  /* We dont care about newlines, spaces, etc */
  if(type == NA) {
    return 0;
  }
  t.m_type = type;
  t.m_stype.clear();

  // get the string for the given toke type
  for(auto &i: TOps) {
    if(t.m_type == i.second) {
      t.m_stype = i.first;
    }
  }
  toks.push_back(t);
  return 0;
}

int AST::readInt() {
  std::string intBuf{};
  while(*fp >= '0' && *fp <= '9') {
    intBuf += *fp;
    fp++;
  }
  fp--;
  return std::stoi(intBuf);
}

void AST::lex() {

  if(readSourceToBuffer(buf1) == -1) {
    lodge::log.error("Failed to Read Source File");
  }
  // FIRST NEED TO GET TO THE FIRST CHARACTER (PAST WHITESPACE)
  //
  std::string tokBuf;
  int count = 0;

  fp = bp;
  while(*fp || *fp == EOF) {
    switch (*fp) {
      case '(':
        lex_buf_push(tokBuf, OP_PAREN);
        count = 0;
        break;
      case ')':
        lex_buf_push(tokBuf, END_PAREN);
        count = 0;
        break;
      case '*':
        lex_buf_push(tokBuf, MULTIPLY);
        count = 0;
        break;
      case '+':
        lex_buf_push(tokBuf, ADD);
        count = 0;
        break;
      case '/':
        lex_buf_push(tokBuf, DIVIDE);
        count = 0;
        break;
      case ',':
        lex_buf_push(tokBuf, COMMA);
        count = 0;
        break;
      case '-':
        lex_buf_push(tokBuf, MINUS);
        count = 0;
        break;
      case '.':
        lex_buf_push(tokBuf, DOT);
        count = 0;
        break;
      case '=':
        lex_buf_push(tokBuf, EQ);
        count = 0;
        break;
      case '\n':
        lex_buf_push(tokBuf, NA);
        lc++;
        count = 0;
        break;
      case ' ':
        lex_buf_push(tokBuf, NA);
        count = 0;
        break;
      case '{':
        lex_buf_push(tokBuf, OP_CURL);
        count = 0;
        break;
      case '}':
        lex_buf_push(tokBuf, END_CURL);
        count = 0;
        break;
      case ';':
        lex_buf_push(tokBuf, SEMICOLON);
        count = 0;
        break;
      case EOF:
        // Check if we are at the end of a buffer in pair
        if(fp == buf1.end() ) {
          /* load in second buffer */
          if(readSourceToBuffer(buf2) == -1) {
            lodge::log.error("Failed to Read Source File");
          }
          fp = buf2.begin();
          lex_buf_push(tokBuf, NA);
          count = 0;

        } else if (fp == buf2.end() ) {
          /* reload the first buffer */
          if(readSourceToBuffer(buf1) == -1) {
            lodge::log.error("Failed to Read Source File");
          }
          fp = buf1.begin();
          lex_buf_push(tokBuf, NA);
          count = 0;

        } else {
          /* terminate lexer */
          lex_buf_push(tokBuf, NA);
          count = 0;
        }
        break;
      case '0':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '1':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '2':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '3':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '4':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '5':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '6':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '7':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '8':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      case '9':
        lex_buf_push(tokBuf, NUM_LITERAL, readInt());
        count = 0;
        break;
      // DEFAULT WILL ACT AS KEYWORDS AND IDENTIFIERS
      default:
        tokBuf += *fp;
        count++;
        break;
      }
    fp++;
    }
}
} // namespace prob
