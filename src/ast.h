#pragma once

#include <memory>
#include <stdint.h>
#include <vector>

#include <lodge.h>

#include "token.h"

namespace prob {

struct NodeBase {
  tokenType type{};
  virtual ~NodeBase() = default;
};

struct NodeUnary : public NodeBase {

  NodeUnary() = default;
  NodeUnary(tokenType t, std::unique_ptr<NodeBase> p) : type(t), c1(std::move(p)) {}
  tokenType type{};
  std::unique_ptr<NodeBase> c1;
};

struct NodeBinary : public NodeBase  {

  NodeBinary() = default;
  NodeBinary(tokenType t, std::unique_ptr<NodeBase> x, std::unique_ptr<NodeBase> y) : type(t), c1(std::move(x)), c2(std::move(y)) {}

  tokenType type{};
  std::unique_ptr<NodeBase> c1{}, c2{};
};

struct NodeTernary : public NodeBase  {
  tokenType type{};
  std::unique_ptr<NodeBinary> c1{}, c2{}, c3{};
};
}
