#pragma once

#include <memory>
#include <stdint.h>
#include <vector>

#include <lodge.h>

#include "token.h"

namespace prob {

  const std::unordered_map<tokenType, std::string> ops = {
    { tokenType::ADD,  "+"},
    { tokenType::EQ,  "="},
    { CHAR,  "char"},
    { BREAK,  "continue"},
    { BREAK,  "default"},
    { BREAK,  "do"},
    { BREAK,  "double"},
    { BREAK,  "else"},
    { BREAK,  "enum"},
    { BREAK,  "extern"},
    { BREAK,  "float"},
    { BREAK,  "goto"},
    { BREAK,  "if"},
    { BREAK,  "int"},
    { BREAK,  "long"},
    { BREAK,  "register"},
    { BREAK,  "return"},
    { BREAK,  "short"},
    { BREAK,  "signed"},
    { BREAK,  "sizeof"},
    { BREAK,  "static"},
    { BREAK,  "struct"},
    { BREAK,  "switch"},
    { BREAK,  "typedef"},
    { BREAK,  "union"},
    { BREAK,  "unsigned"},
    { BREAK,  "void"},
    { BREAK,  "volatile"},
    { BREAK,  "while"},
   };
struct NodeBase {
  tokenType type{};
  virtual ~NodeBase() = default;
};

struct NodeUnary : public NodeBase {

  NodeUnary() = default;
  NodeUnary(tokenType t, std::unique_ptr<NodeBase> p) : type(t), c1(std::move(p)) {}
  NodeUnary(tokenType t,  std::unique_ptr<NodeBase> p, std::string id, std::string vt) : type(t), c1(std::move(p)), val(std::move(id)), valType(std::move(vt)) {}

  tokenType type{};
  std::unique_ptr<NodeBase> c1;
  std::variant<int, float, std::string> val{};
  std::string valType{};
};

struct NodeBinary : public NodeBase  {

  NodeBinary() = default;
  NodeBinary(tokenType t, std::unique_ptr<NodeBase> x, std::unique_ptr<NodeBase> y) : type(t), c1(std::move(x)), c2(std::move(y)) { 
    auto res = ops.find(t);
    if(res != ops.end()) {
      this->valType = ops.find(t)->second;
    }
  }

  tokenType type{};
  std::unique_ptr<NodeBase> c1{}, c2{};
  std::variant<int, float, std::string> val{};
  std::string valType{};
  std::string name{};

};

struct NodeTernary : public NodeBase  {

  NodeTernary() = default;

  tokenType type{};
  std::unique_ptr<NodeBase> c1{}, c2{}, c3{};
  std::variant<int, float, std::string> val{};
  std::string valType;
};

struct NodeNary : public NodeBase {

  NodeNary() = default;

  tokenType type{};
  std::vector<std::unique_ptr<NodeBase>> ci{};
  std::variant<int, float, std::string> val{};
  std::string valType;
};
}
