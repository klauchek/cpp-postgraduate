%option c++

%{

#include "grammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
VAR     [a-zA-Z][a-zA-Z_0-9]*
%%

{WS}              /* skip blanks and tabs */
"+"               return yy::parser::token_type::PLUS;
"-"               return yy::parser::token_type::MINUS;
"="               return yy::parser::token_type::ASSIGN;
";"               return yy::parser::token_type::SCOLON;
{DIGIT1}{DIGIT}*  return yy::parser::token_type::NUMBER;
"print"           return yy::parser::token_type::PRINT;
"input"           return yy::parser::token_type::INPUT;
{VAR}             return yy::parser::token_type::ID;
"["               return yy::parser::token_type::LBR;
"]"               return yy::parser::token_type::RBR;
.			      return yy::parser::token_type::ERR;

%%

// nothing