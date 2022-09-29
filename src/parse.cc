#include "parse.h"

namespace prob {

/* 1. Read file into "source" buffer.
   2. Turn buffer into AST.
    2.1 Get rid of white space
    2.2 Read each char, fill up second buffer after each character to check for
   keywords
*/

AST::AST(const std::string path) { auto res = readSource(path); }

int AST::readSource(const std::string path) {

  constexpr uint32_t BUF_SIZE = 16 * 1024;
  int fd = open(path.c_str(), O_RDONLY);
  if (!fd) {
    lodge::log.error("Error opening file");
    return -1;
  }

  posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
  char buf[BUF_SIZE + 1];

  while (size_t bytes_read = read(fd, buf, BUF_SIZE)) {

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

std::string AST::getSource() { return this->source; }

void AST::parse() {

  char cur{};

  switch (cur) {
  case '+':
    // it's a plus, add it to the AST and ready 2 leaves
    break;
  }

  return;
}
} // namespace prob
