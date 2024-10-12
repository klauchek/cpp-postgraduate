/* ------------------------------------------------------------------------- **
 *
 * Source code for MIPT ILab
 * Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
 * Licensed after GNU GPL v3
 *
 * ------------------------------------------------------------------------- **
 *
 *  Grammar with shift/reduce, but correct
 *  EQL -> EQ; EQL | empty
 *  EQ -> E = E
 *  E -> number | E + number | E - number
 *
 * ------------------------------------------------------------------------- */

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "Node.hpp" 

// forward decl of argument to parser
namespace yy { class Driver; }
}

%code
{
#include "driver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         Driver* driver);
void wholeProgramAction();
}
}


%token
  ASSIGN  "="
  MINUS   "-"
  PLUS    "+"
  SCOLON  ";"
  LBR     "["    
  RBR     "]"     
  PRINT    "print"
  INPUT    "input"
  ERR
;

%token <int>             NUMBER
%token <std::string>     ID

%type <std::vector<ast::INode*>> statement_list
%type <ast::INode*> statement
%type <ast::INode*> assign
%type <ast::INode*> lvalue
%type <ast::INode*> prvalue
%type <ast::INode*> value
%type <ast::INode*> mem_access
%type <ast::INode*> lexpr
%type <ast::INode*> expr
%type <ast::INode*> term 

%left '+' '-'
%right '='

%start program

%%

program: tree                    { yy::wholeProgramAction();
                                    std::cout << "tree\n";}
;

tree: statement_list             { GlobalNode = GlobalNode->push();
                                    std::cout << "statement list\n"; }
;

statement_list : statement SCOLON statement_list          { GlobalNode->addStatement($1);
                                                            std::cout << "statement SCOLON statement list\n"; }
               | %empty { std::cout << "empty \n"; } 

statement: assign                 { $$ = $1; std::cout << "assign\n";}
        | expr                    { $$ = $1; std::cout << "expr from statement\n";}
        | PRINT assign            { $$ = ast::makeUnOpNode($2, ast::Operation::Print); std::cout << "print from statement\n";}
        | PRINT expr              { $$ = ast::makeUnOpNode($2, ast::Operation::Print); std::cout << "print from statement\n";}
        | INPUT lvalue            { $$ = ast::makeUnOpNode($2, ast::Operation::Input); std::cout << "input from statement\n";}
;

assign: lvalue ASSIGN expr        { $$ = ast::makeBinOpNode($1, ast::Operation::Assign, $3); std::cout << "ID EQ expr\n";}
;

lvalue: lexpr                     {$$ = $1;}
        | mem_access              {$$ = $1;}
;

mem_access:  LBR expr RBR         { $$ = ast::makeMemAccessNode(std::nullopt, $2); std::cout << "mem access \n"; } //mem access
            | term LBR expr RBR   { $$ = ast::makeMemAccessNode($1, $3); std::cout << "mem access with base \n"; } //mem acces
;

prvalue : NUMBER                  { $$ = ast::makeNumNode($1); std::cout << "NUMBER\n"; }
;

lexpr:   ID                       { $$ = GlobalNode->access($1); std::cout << "ID\n"; }
        | LBR                     {std::string Varname = "c"; $$ = GlobalNode->access(Varname); std::cout << "C\n";}
        | RBR LBR                 {std::string Varname = "x"; $$ = GlobalNode->access(Varname); std::cout << "X\n";}
;

value : lexpr                     { $$ = GlobalNode->visible($1); std::cout << "lval from value\n"; }
        | prvalue                 { $$ = $1; std::cout << "rval from value\n"; }
        | mem_access              { $$ = $1;}
;

expr: value                       { $$ = $1; std::cout << "value from expr\n";}
    | value PLUS expr             { $$ = ast::makeBinOpNode($1, ast::Operation::Plus, $3); std::cout << "lval PLUS expr\n"; }
    | value MINUS expr            { $$ = ast::makeBinOpNode($1, ast::Operation::Minus, $3); std::cout << "lval MINUS expr\n"; }
;

    term: ID                      { $$ = GlobalNode->visible($1); std::cout << "ID\n";
                                          if (!$$) { std::cout << "Unvisible ID\n";}}
    | LBR                         {std::string Varname = "c"; $$ = GlobalNode->visible(Varname); std::cout << "C\n";
                                          if (!$$) { std::cout << "Unvisible ID\n";}}
    | RBR LBR                     {std::string Varname = "x"; $$ = GlobalNode->visible(Varname); std::cout << "X\n";
                                          if (!$$) { std::cout << "Unvisible ID\n";}}
; 

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}

void wholeProgramAction() {
  GlobalNode->calc();
}
}