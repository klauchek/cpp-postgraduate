#pragma once

#include <string>
#include <unordered_map>

#include "Node.hpp"

class Symtab_local final {
  std::unordered_map<std::string, ast::INode *> Symbols_;

public:
  void add(std::string Symbol, ast::INode *Node) { Symbols_[Symbol] = Node; }

  ast::INode *find(std::string Symbol) const {
    auto It = Symbols_.find(Symbol);
    if (It == Symbols_.end())
      return nullptr;
    return It->second;
  }

  void free() {
    for (auto &Elt : Symbols_)
      delete Elt.second;
    Symbols_.clear();
  }
};

class Symtab final {
  std::unordered_map<const ast::INode *, Symtab_local> Tables_;

public:
  void add(const ast::INode *Scope, std::string Symbol, ast::INode *N) {
    Tables_[Scope].add(Symbol, N);
  }

  ast::INode *exists(const ast::INode *Scope, std::string Symbol) const {
    auto It = Tables_.find(Scope);
    if (It == Tables_.end())
      return nullptr;
    return It->second.find(Symbol);
  }

  void free(const ast::INode *Scope) {
    auto It = Tables_.find(Scope);
    if (It != Tables_.end())
      It->second.free();
  }
};

extern Symtab GlobalTable;
