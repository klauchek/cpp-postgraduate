
#include <map>
#include <stdexcept>
#include <string>

#include "Node.hpp"
#include "Symtab.hpp"

Symtab GlobalTable;

namespace ast {

INode *makeNumNode(int Val){
    return new NumNode(Val);
};
INode *makeBinOpNode(INode *Lhs, Operation Op, INode *Rhs) {
    return new BinOpNode(Lhs, Op, Rhs);
}
INode *makeUnOpNode(INode *Var, Operation Op){
    return new UnOpNode(Var, Op);
}

INode *makeMemAccessNode(std::optional<INode *> Base, INode *Addr) {
    if(Base.has_value()) {
        return new MemAccessNode(Base.value(), Addr);
    }
    return new MemAccessNode(std::nullopt, Addr);
}

GlobalNode *createGlobalNode() {
    return new GlobalNode{};
}

INode *GlobalNode::access(std::string VarName) {
  INode *Existing = visible(VarName);
  if (Existing)
    return Existing;

  INode *NewDecl = new VarNode{VarName};

  GlobalTable.add(this, VarName, NewDecl);
  return NewDecl;
}

INode *GlobalNode::visible(std::string const &VarName) const {
  INode *F = GlobalTable.exists(this, VarName);
  return F;
}

INode *GlobalNode::visible(INode* Var) const {
  std::string VarName = static_cast<VarNode*>(Var)->getName();
  INode *F = GlobalTable.exists(this, VarName);
  return F;
}

int BinOpNode::calc() {
  int Val = 0;
  switch (Op_) {
  case Operation::Plus:
    if (Left_ == nullptr)
      return Right_->calc();
    return Left_->calc() + Right_->calc();
  case Operation::Minus:
    if (Left_ == nullptr)
      return -Right_->calc();
    return Left_->calc() - Right_->calc();
  case Operation::Assign:
    Val = Right_->calc();
    static_cast<VarNode *>(Left_)->setVal(Val);
    return Val;

  default:
    throw std::runtime_error("Operation not supported");
  }
  return Val;
}

int UnOpNode::calc() {
  int Val = 0;
  switch (Op_) {
  case Operation::Print:
    std::cout << Child_->calc() << std::endl;
    break;
  case Operation::Input:
    std::cin >> Val;
    return Val;

  default:
    throw std::runtime_error("Operation not supported");
  }
  return Val;
}

} // namespace ast
