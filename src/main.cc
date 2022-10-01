#include <iostream>
#include <vector>

#include <lodge.h>

#include "parse.h"
#include "prob.h"

int main(int argc, char **argv) {
  lodge::log.start();
  prob::AST t;

  t.set_file(argv[1]);
  if (t.readSourceToBuffer()) {
    lodge::log.error("Error: Unable to read source");
  }
  prob::probability p;
  p.start();

  t.lex();

  lodge::log.stop();

  return 0;
}
