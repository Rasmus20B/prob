#include <iostream>
#include <vector>

#include <lodge.h>

#include "parse.h"
#include "prob.h"

int main(int argc, char **argv) {

  lodge::log.start();
  prob::AST t{};

  t.set_file(argv[1]);
  t.lex();
  t.parse_program();

  t.print_tree();

  lodge::log.stop();

  return 0;
}
