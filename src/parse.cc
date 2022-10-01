#include "parse.h"

namespace prob {

/* 1. Read file into "source" buffer.
   2. Turn buffer into AST.
    2.1 Get rid of white space
    2.2 Read each char, fill up second buffer after each character to check for
   keywords
*/

AST::AST(const std::string path) { auto res = readSourceToBuffer(); }

void AST::set_file(std::string path) { m_path = path;}

int AST::readSourceToBuffer() {

  int fd = open(m_path.c_str(), O_RDONLY);
  if (!fd) {
    lodge::log.error("Error opening file");
    return -1;
  }

  posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
  std::array<char, BUF_SIZE + 1> buf;

  while (size_t bytes_read = read(fd, buf.begin(), BUF_SIZE)) {

    if (bytes_read == (size_t)-1) {
      lodge::log.error("error reading file");
      exit(255);
    }
    if (!bytes_read) {
      break;
    }
  }

  source = std::move(buf);
  return 0;
}

// std::string AST::getSource() { return this->source; }

void AST::lex() {

  auto bp = source.begin();
  auto fp = source.begin();

  while(*bp) {
    switch (*bp) {
      case '(':
        lodge::log.info("Found an LPAREN");
        break;
      case ')':
        lodge::log.info("Found an RPAREN");
        break;
      case '*':
        break;
      case '+':
        break;
      case ',':
        break;
      case '-':
        break;
      case '.':
        break;
      case '\n':
        break;
      case EOF:
        lodge::log.info("Found EOF");
        break;
      default:
        break;
      }
    bp++;
  }
}
} // namespace prob
