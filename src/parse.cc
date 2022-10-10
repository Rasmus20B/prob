#include "parse.h"
#include "src/token.h"

namespace prob {

void AST::set_file(std::string_view path) { m_path = path;}

void AST::print_tokens() {
  
  for(auto i: toks ) {
    lodge::log.info("{}", i.m_stype);
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

  add_node(head, FUNCTION_DECL, ptype, id);
  while(t_it->m_type != END_PAREN) { t_it++; }

  lodge::log.info("Found end parenthesis");
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
      }
      return 0;
    default:
      return -1;
      
  }
  return 0;

}

int AST::parse_token() {

  /* When an { is found, this is the start of a compoudnd statement */
  switch(t_it->m_type) {
    case tokenType::IDENTIFIER:
      lodge::log.info("FOUND AN IDENTIFIER");
      parse_identifier();
      break;
    case tokenType::INT:
      lodge::log.info("FOUND AN INT");
      parse_int();
      break;
    default:
      return -1;
      break;
  }
  return 0;
}
 
int AST::parse_program() {
  t_it = toks.begin();
  while(true) {
    if(parse_token() == -1) {
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
#elif WINDOWSONE
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

int AST::lex_buf_push(std::string &key, tokenType type) {

  Token t{};
  if(key.size() > 0) {
    t.m_type = comp_keywords(key);
    t.m_stype = key;
    toks.push_back(t);
  }
  key.clear();
  /* We dont care about newlines, spaces, etc */
  if(type == NA) {
    return 0;
  }
  t.m_type = type;
  t.m_stype.clear();
  for(auto &i: TOps) {
    if(t.m_type == i.second) {
      t.m_stype = i.first;
    }
  }
  toks.push_back(t);
  return 0;
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
