#ifndef  LEXHPP
#define  LEXHPP
#include <iostream>

enum types {
    TYPE_NULL,
    TYPE_INT,
    TYPE_STRING,
    TYPE_ARR
};
struct Type {
    types type;
    int arr_dim;
    Type (types type = TYPE_NULL, int arr_dim = 0) : type(type), arr_dim(arr_dim) {}
    bool operator == (const Type &otype) const { return this->type == otype.type && this->arr_dim == otype.arr_dim; }
    bool operator != (const Type &otype) const { return !(*this == otype); }

};

enum lex_type { 
    LEX_EOF, 
    LEX_OP,
    LEX_NUM, 
    LEX_ID, 
    LEX_NULL
    //POLIZ_LABEL, 
    //POLIZ_ADDRESS, 
    //POLIZ_GO, 
    //POLIZ_FGO
};
struct Lex{
	Lex(std::string s = "", lex_type lt = LEX_EOF, Type t = Type()): lex(s), t_lex(lt), type(t){};
	std::string lex = "";
    lex_type t_lex = LEX_EOF;
    Type type = Type();
};/**/


enum state { LEX, ID, NUM, SGN};

#endif
