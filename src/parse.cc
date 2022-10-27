#include "parse.h"
#include "src/ast.h"
#include "src/token.h"
#include <cstddef>
#include <memory>
#include <string>

namespace prob {

void AST::set_file(std::string_view path) { m_path = path;}

void AST::print_expression(NodeBinary* e) {
  if(e) {
       print_expression(static_cast<NodeBinary*>(e->c1.get()));
       switch(e->type) {
         case tokenType::NUM_LITERAL:
           lodge::log.info("Literal: {}", std::get<int>(e->val));
           break;
         case tokenType::IDENTIFIER:
           lodge::log.info("Identifier: {}", e->name);
           break;
         default:
           lodge::log.info("BinaryExpr: {}", e->valType);
           break;
       }
       print_expression(static_cast<NodeBinary*>(e->c2.get()));
  }

}

void AST::print_compound(NodeNary* e) {

  lodge::log.info("Compound Stat:");
  for(auto &i : e->ci) {
    if(i.get() == nullptr) {
      lodge::log.info("End of Compound Stat");
      break;
    } else if(static_cast<NodeUnary*>(i.get())->type == tokenType::DECL_STAT) {
      lodge::log.info("Variable Decl: {} {}", static_cast<NodeUnary*>(i.get())->valType, std::get<std::string>(static_cast<NodeUnary*>(i.get())->val));
      print_expression(static_cast<NodeBinary*>(static_cast<NodeUnary*>(i.get())->c1.get()));
    } else if(static_cast<NodeUnary*>(i.get())->type == tokenType::RETURN) {
      lodge::log.info("Return: ");
      print_expression(static_cast<NodeBinary*>(static_cast<NodeUnary*>(i.get())->c1.get()));
    } else {
      lodge::log.info("{}", static_cast<NodeUnary*>(i.get())->type);
    }
  }
}

void AST::print_tree() {

  for(auto &i: head->ci) {
    auto e = static_cast<NodeUnary*>(i.get());
    switch(e->type) {
      case tokenType::FUNCTION_DECL:
        if(!e->c1) {
          lodge::log.info( "function declaration: {}, {}", e->valType, std::get<std::string>(e->val));
        } else if(static_cast<NodeNary*>(e->c1.get())->type == tokenType::COMPOUND_STAT) {
          lodge::log.info( "function definition: {}, {}", e->valType, std::get<std::string>(e->val));
          print_compound(static_cast<NodeNary*>(e->c1.get()));
        }
        break;
      default:
        lodge::log.info("Unknown");
        break;
    }
  }

}

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

std::unique_ptr<NodeBase> AST::parse_expr_u() {
  auto lhs = parse_primary_expr_u();
  if(lhs == nullptr) {
    return nullptr;
  }
  return parse_binop_u(0, std::move(lhs));

}

std::unique_ptr<NodeBase> AST::parse_binop_u(int prec, std::unique_ptr<NodeBase> lhs) {
  while(true) {
    auto p = precs.find(t_it->m_type);
    if(p->second < prec) {
      return lhs;
    }

    t_it++;
    auto rhs = parse_primary_expr_u();
    if(!rhs) {
      return nullptr;
    }

    auto n_p = precs.find((t_it)->m_type);
    if(p->second < n_p->second) {
      rhs = parse_binop_u(p->second+1, std::move(rhs));
      if(!rhs){
        return nullptr;
      }
    }
    lhs = std::make_unique<NodeBinary>(p->first, std::move(lhs), std::move(rhs));
  }
}

std::unique_ptr<NodeBase> AST::parse_identifier_u() {
  auto num = std::make_unique<NodeBinary>(tokenType::IDENTIFIER, nullptr, nullptr);
  num->type = tokenType::IDENTIFIER;
  num->name = t_it->name;
  t_it++;
  return num;
}

std::unique_ptr<NodeBase> AST::parse_num_literal_u() {
  auto num = std::make_unique<NodeBinary>(tokenType::NUM_LITERAL, nullptr, nullptr);
  num->type = tokenType::NUM_LITERAL;
  num->val = std::get<int>(t_it->val);
  num->valType = std::to_string(std::get<int>(num->val));
  t_it++;
  return num;
}
std::unique_ptr<NodeBase> AST::parse_paren_expr_u() {
  auto num = std::make_unique<NodeUnary>(tokenType::NUM_LITERAL, nullptr);
  num->type = tokenType::IDENTIFIER;
  t_it++;
  return num;
}
std::unique_ptr<NodeBase> AST::parse_primary_expr_u() {
switch(t_it->m_type) {
    case tokenType::IDENTIFIER:
      return parse_identifier_u();
    case tokenType::NUM_LITERAL:
      return parse_num_literal_u();
    case tokenType::OP_CURL:
      return parse_paren_expr_u();
    default:
      lodge::log.error("Invalid token for expression");
      return nullptr;
  }
}

std::unique_ptr<NodeBase> AST::parse_int() {
  t_it++;
  switch(t_it->m_type) {
    case tokenType::IDENTIFIER:
      return parse_identifier();
    default:
      lodge::log.error("Expected identifier after typename!");
      return nullptr;
      break;
  }
}

std::unique_ptr<NodeBase> AST::parse_function_decl(tokenType ptype, std::string id) {
    
  //Add function declaration to tree as id and type   

  func_count++;
  while(t_it->m_type != END_PAREN) { t_it++; }

  t_it++;
  switch(t_it->m_type) {
    case tokenType::OP_CURL:
      return std::make_unique<NodeUnary>(tokenType::FUNCTION_DECL, parse_compound_statement(), id, (t_it - 4)->m_stype);
    case tokenType::SEMICOLON:
      return std::make_unique<NodeUnary>(tokenType::FUNCTION_DECL, nullptr, id, (t_it - 4)->m_stype);
    default:
      lodge::log.error("Expected Semicolon or statement, found {}:{}", t_it->m_stype, t_it->m_type);
      return nullptr;
  }
}

std::unique_ptr<NodeBase> AST::parse_var_declr_statement(std::string type, std::string id) {
  t_it++;
  return std::make_unique<NodeUnary>(tokenType::DECL_STAT, parse_expr_u(), id, type);
}


std::unique_ptr<NodeBase> AST::parse_identifier() {
  
  t_it++;
  std::unique_ptr<NodeBase> e{};
  switch(t_it->m_type) {
    case tokenType::SEMICOLON:
      return nullptr;
    case tokenType::OP_PAREN:
      // if there is a type written before id, it's a declaration
      if((t_it - 2)->m_type == INT) {
        return parse_function_decl((t_it - 2)->m_type, (t_it - 1)->m_stype);
      } else {
        // if its not, it's a function call
        while(t_it->m_type != END_PAREN) {t_it++;}
      }
      return nullptr;
    case tokenType::EQ:
      return parse_var_declr_statement((t_it - 2)->m_stype, (t_it - 1)->m_stype);
    default:
      return nullptr;
  }
  return 0;

}

std::unique_ptr<NodeBase> AST::parse_compound_statement() { 
  auto e = std::make_unique<NodeNary>();
  e->type = tokenType::COMPOUND_STAT;
  while(t_it->m_type != END_CURL) { 
    e->ci.push_back(parse_statement());
  }
  return e; 
}

int AST::parse_var_assignment_statement() {
  return 0;
}

std::unique_ptr<NodeBase> AST::parse_function_or_assignment() {
  t_it++;
  switch(t_it->m_type) {
    case tokenType::OP_PAREN:
      //its a function call
      return nullptr;
    case tokenType::EQ:
      return parse_var_declr_statement((t_it - 2)->m_stype, (t_it - 1)->m_stype);
    default:
      lodge::log.error("Expected paren or equals");
      return nullptr;
  }
  return 0;
}

std::unique_ptr<NodeBase> AST::parse_return_statement() {

  std::vector<Token> vals{};
  t_it++;
  return std::make_unique<NodeUnary>(tokenType::RETURN, parse_expr_u());
}

std::unique_ptr<NodeBase> AST::parse_statement() {

  t_it++;
  switch(t_it->m_type) {
    case tokenType::OP_CURL:
      return parse_compound_statement();
    case tokenType::IF:
      // parse_if_statement();
      return nullptr;
    case tokenType::SEMICOLON:
      t_it++;
      return nullptr;
    case tokenType::IDENTIFIER:
      // Check if it's a function or a variable assignment via '('
      return parse_function_or_assignment();
    case tokenType::INT:
      t_it++;
      if(t_it->m_type == tokenType::IDENTIFIER) {
        return parse_function_or_assignment();
      }
    case tokenType::NUM_LITERAL:
      return nullptr;
    case tokenType::RETURN:
      return parse_return_statement();
    default:
      return nullptr;
  }

  return 0;

}

std::unique_ptr<NodeBase> AST::parse_token() {

  /* When an { is found, this is the start of a compoudnd statement */
  switch(t_it->m_type) {
    case tokenType::INT:
      return parse_int();
    default:
      return nullptr;
  }
  t_it++;
  return nullptr;
}
 
int AST::parse_program() {
  t_it = toks.begin();
  while(true) {
    std::unique_ptr<NodeBase> e{};
    if((e = parse_token()) == nullptr) {
      // print_tree();
      return -1;
    } else {
      head->ci.emplace_back(std::move(e));
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
    t.name = key;
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
  t.name = key;
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
    t.name = key;
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
