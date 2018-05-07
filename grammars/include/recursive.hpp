#ifndef SYNTAXHPP
#define SYNTAXHPP

#include <stack>
#include <vector>
#include "iterlex.hpp"

class Parser {
    Lex_seq::iterator cur_pos;
    Lex cur_lex;
    type_of_lex cur_type;
    std::stack<Type> st_type;
    std::vector<Lex> poliz;
    void S ();
    void Expr ();
    void Add_expr ();
    void Add_expr1 ();
    void Mult_expr ();
    void Mult_expr1 ();
    void Id ();
    void check ();
    void check_op ();
  public :
    Parser (Lex_seq &lex_seq) : cur_pos(lex_seq.begin()) {}
    void parse ();
    void print_poliz ();
    void print_expr ();

};

class Syntax_exception : public Exception {
  public :
    Syntax_exception (const std::string &what, const std::pair<int,int>& pos) : Exception(what, pos.first, pos.second) { this->what = "SYNTAX:" + what; }
};

class Semantic_exception : public Exception { 
  public :
    Semantic_exception (const std::string &what, const std::pair<int,int>& pos) : Exception(what, pos.first, pos.second) { this->what = "SENANTICS:" + what; }
};
#endif




