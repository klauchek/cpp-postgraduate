#ifndef __NODE__HPP__
#define __NODE__HPP__

#include <string>
#include <iostream>
#include <vector>
#include <optional>

namespace ast {
struct INode {

    virtual void dump() const = 0;
    virtual int calc() = 0;
    virtual ~INode() = default;
};


enum class Operation {
  Plus,
  Minus,
  Assign,
  Print,
  Input
};

struct VarNode final: public INode {

    std::string Name_;
    int Value_;

    VarNode(const std::string &Name) : Name_(Name) {}
    void setVal(const int Value) {
        Value_ = Value;
    }

    int getVal() const {
        return Value_;
    }

    std::string getName() {
        return Name_;
    }

    void dump() const override {
        std::cout << "name_: " << Name_ << std::endl;
        std::cout << "value_: " << Value_ << std::endl;
    }
    int calc() override { 
        std::cout << "VarNode: calc " << std::endl; 
        return 1;
    }
};

struct NumNode final: public INode {
    int Value_;

    NumNode(const int Value) : Value_(Value) {}

    int getVal() const {
        return Value_;
    }

    void dump() const override {
        std::cout << "value_: " << Value_ << std::endl;
    }
    int calc() override { 
        std::cout << "NumNode: calc " << std::endl; 
        return 1;
    }
};

struct BinOpNode : public INode {
    INode* Left_;
    Operation Op_;
    INode* Right_;

    BinOpNode(INode* Left, Operation Op, INode* Right) : Left_(Left), Op_(Op), Right_(Right) {}
    void dump() const override final {
        std::cout << "BinOpNode: " << std::endl; 
        Left_->dump();
        //std::cout << op_;
        Right_->dump();
    }
    int calc() override; 
};

//print, input
struct UnOpNode : public INode {
    INode* Child_;
    Operation Op_;

    UnOpNode(INode* Child, Operation Op) : Child_(Child), Op_(Op) {}
    void dump() const override {
        std::cout << "UnOpNode: " << std::endl;
        Child_->dump();
        //std::cout << op_;
    }
    int calc() override;

};

struct MemAccessNode : public INode {
    std::optional<INode *> Base_;
    INode* Addr_;

    MemAccessNode(std::optional<INode *> Base = std::nullopt, INode* Addr = nullptr) : Base_(Base), Addr_(Addr) {}
    void dump() const override final {
        std::cout << "MemAccessNode: " << std::endl; 
        if(Base_.has_value())
            Base_.value()->dump();
        if(Addr_ != nullptr)
            Addr_->dump();
    }
    int calc() override { 
        std::cout << "MemAccessNode: calc " << std::endl; 
        return 1;
    }
};

struct GlobalNode : public INode {

    std::vector<INode*> statements_;

    GlobalNode(std::vector<INode*> statements) : statements_(statements) {}
    GlobalNode() {}

    GlobalNode *push() { 
       return new GlobalNode(); 
    }
    void addStatement(INode *statement) { 
        statements_.push_back(statement);
    }

    void dump() const override {
        std::cout << "GlobalNode: " << std::endl;
        for(const auto* node : statements_) {
            node->dump();
        }
    }
    int calc() override {
        for (auto *s : statements_)
            s->calc();
        return 0;
    }
    INode *access(std::string VarName);
    INode *visible(std::string const &VarName) const;
    INode *visible(INode* Var) const;

};

// ctor functions

INode *makeNumNode(int Val);
INode *makeBinOpNode(INode *Lhs, Operation Op, INode *Rhs);
INode *makeUnOpNode(INode *Var, Operation Op);
INode *makeMemAccessNode(std::optional<INode *> Base, INode *Addr);
GlobalNode *createGlobalNode();

}
extern ast::GlobalNode *GlobalNode;

#endif //__NODE__HPP__