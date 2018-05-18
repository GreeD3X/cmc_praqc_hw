#ifndef SYNTAXHPP
#define SYNTAXHPP

#include <stack>
#include <list>
#include <vector>
#include "iterlex.hpp"

class Parser {
public:
    Parser (Lex_seq& lex_seq): cur_pos(lex_seq.begin()) {}
    void parse ();
    void print_poliz ();
    void print_expr ();
private:
    Lex_seq::iterator cur_pos;
    Lex cur_lex;
    lex_type cur_type;
    std::stack<Type>st_type; 
    std::vector<Lex> poliz;
    void get_lex();
    void S ();
    void Expr ();
    void Index_expr ();
    void Index();
    void check_id ();
    void check_op ();
};

class Syn_exception: public std::exception {
public:
	const char* what() const noexcept {return this->err_msg.c_str();}
    Syn_exception (const std::string &what, const std::pair<int,int>& pos): std::exception(), err_msg("Synt_except: " + std::to_string(pos.first) + ":" + std::to_string(pos.second) + ": " + what) {}
private:
	std::string err_msg;
};

class Sem_exception: public std::exception { 
public:
	const char* what() const noexcept {return this->err_msg.c_str();}
    Sem_exception (const std::string &what, const std::pair<int,int>& pos): std::exception(), err_msg("Sem_except: " + std::to_string(pos.first) + ":" + std::to_string(pos.second) + ": " + what) {}
private:
	std::string err_msg;
};

#endif




