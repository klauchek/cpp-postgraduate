//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Driver program for simplest grammar of comparsions
//
//----------------------------------------------------------------------------

#include <iostream>

#include "driver.hpp"
#include <memory>

ast::GlobalNode *GlobalNode = nullptr;
// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() { return 1; }

int main() {
  FlexLexer *lexer = new yyFlexLexer;
  yy::Driver driver(lexer);

  std::unique_ptr<ast::GlobalNode> CurrentScopeOwner{ast::createGlobalNode()};
  GlobalNode = CurrentScopeOwner.get();

  driver.parse();
  delete lexer;
}