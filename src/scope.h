#pragma once

#include <unordered_map>
#include <string>

#include "stack.h"

struct Scope {
  Scope* parent;
  Scope* addr = this;
  std::unordered_map<std::string, bool> vars;
};
