#include "parse.h"
#include "src/token.h"

namespace prob {

// AST::AST(const std::string path) { auto res = readSourceToBuffer(buf1); }

void AST::set_file(std::string path) { m_path = path;}

void AST::print_tokens() {
  
  for(auto &i: toks) {
    lodge::log.info("{}", i.m_stype);
  }
}

int AST::readSourceToBuffer(std::array<char, BUF_SIZE + 1>& buf) {

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
  bytes_read = read(fd, buf.begin(), BUF_SIZE);
  return 0;
}

// std::string AST::getSource() { return this->source; }

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
    if(t.m_type == i.second)
      t.m_stype = i.first;
  }
  toks.push_back(t);
  return 0;
}

void AST::lex() {

  if(readSourceToBuffer(buf1) == -1) {
    lodge::log.error("Failed to Read Source File");
  }
  if(readSourceToBuffer(buf2) == -1) {
    lodge::log.error("Failed to Read Source File");
  }
  buf1.at(BUF_SIZE) = EOF;
  buf2.at(BUF_SIZE) = EOF;

  // FIRST NEED TO GET TO THE FIRST CHARACTER (PAST WHITESPACE)
  //
  std::string tokBuf;
  int count = 0;

  fp = bp;
  while(*fp) {
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
        lex_buf_push(tokBuf, NA);
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
