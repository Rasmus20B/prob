#include "parse.h"
#include "src/token.h"

namespace prob {

// AST::AST(const std::string path) { auto res = readSourceToBuffer(buf1); }

void AST::set_file(std::string path) { m_path = path;}

void AST::print_tokens() {
  
  int found =0;
  for(auto &i : toks) {
    for(auto &j : TKeywords) {
      if(j.second == i.m_type) {
        lodge::log.info("{}", j.first);
        found =1;
      }
    }
    if(!found) {
      for(auto &k : TOps) {
        if(k.second == i.m_type) {
          lodge::log.info("{}", k.first);
        }
      }
    }
    found = 0;
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

tokenType AST::comp_keywords(const char * key) {
  auto s = TKeywords.find(static_cast<std::string>(key));
  if(s != TKeywords.end()) {
    return s->second;
  } else {
    return IDENTIFIER;
  }
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
  char tokBuf[128];
  int count = 0;

  fp = bp;
  while(*fp) {
    Token t{};
    switch (*fp) {
      case '(':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        toks.push_back(t);
        count = 0;
        t.m_type = OP_PAREN;
        toks.push_back(t);
        break;
      case ')':
        if((t.m_type = comp_keywords(tokBuf)) == NA) {
          /* Load as identifier */
        }
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = END_PAREN;
        toks.push_back(t);
        break;
      case '*':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = MULTIPLY;
        toks.push_back(t);
        break;
      case '+':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = ADD;
        toks.push_back(t);
        break;
      case '/':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = DIVIDE;
        toks.push_back(t);

        break;
      case ',':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = COMMA;
        toks.push_back(t);
        break;
      case '-':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = MINUS;
        toks.push_back(t);
        break;
      case '.':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = DOT;
        toks.push_back(t);
        break;
      case '=':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = EQ;
        toks.push_back(t);

        break;
      case '\n':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        break;
      case ' ':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        break;
      case '{':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = OP_CURL;
        toks.push_back(t);

        break;
      case '}':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = END_CURL;
        toks.push_back(t);

       break;
      case ';':
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        count = 0;
        toks.push_back(t);
        t.m_type = SEMICOLON;
        toks.push_back(t);

      break;
      case EOF:
        t.m_type = comp_keywords(tokBuf);
        memset(tokBuf, 0, sizeof(tokBuf) - 1);
        toks.push_back(t);
        t.m_type = DOT;
        toks.push_back(t);

        break;
      // DEFAULT WILL ACT AS KEYWORDS AND IDENTIFIERS
      default:
        tokBuf[count] = *fp;
        count++;
        break;
      }
    fp++;
    }
}
} // namespace prob
