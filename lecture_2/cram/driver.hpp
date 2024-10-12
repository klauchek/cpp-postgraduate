//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Driver for simplest grammar of comparsions. Note plex_->YYText() usage.
//
//----------------------------------------------------------------------------

#pragma once

#include "Node.hpp"
#include "grammar.tab.hh"
#include <FlexLexer.h>

// struct yyRet {
//   std::string name;
//   int value;
//   ast::INode *treeNode;
//   ast::Operation op;
// };

// #define YYSTYPE yyRet

namespace yy {

class Driver {
  FlexLexer *plex_;

public:
  Driver(FlexLexer *plex) : plex_(plex) {}

  parser::token_type yylex(parser::semantic_type *yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
    if (tt == yy::parser::token_type::NUMBER)
      yylval->as<int>() = std::stoi(plex_->YYText());
    return tt;
  }

  bool parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
  }
};

} // namespace yy