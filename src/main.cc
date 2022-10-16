#include <iostream>
#include <vector>

#include <lodge.h>

#include "parse.h"
#include "prob.h"

int main(int argc, char **argv) {
  lodge::log.start();
  prob::AST t{};

  t.set_file(argv[1]);
  lodge::log.stop();
  t.lex();
  // t.print_tokens();
  t.parse_program();


  return 0;
}
