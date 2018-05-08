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

std::ostream& operator << (std::ostream& str, Type type)
{
    if (type.arr_dim != 0) {
        str << type.arr_dim << " dimensional array of ";
    }
    switch (type.type) {
        case TYPE_NULL :
            str << "TYPE_NULL";
            return str;
        case TYPE_INT :
            str << "integer";
            return str;
        case TYPE_STRING :
            str << "string";
            return str;
		default :
			str << type.type;
			return str;
    }
}

enum lex_type { 
    LEX_EOF, 
    LEX_ASS,
    LEX_LBR,
    LEX_RBR,
    LEX_NUM, 
    LEX_ID, 
    LEX_NULL
    //POLIZ_LABEL, 
    //POLIZ_ADDRESS, 
    //POLIZ_GO, 
    //POLIZ_FGO
};

class Lex{
public:
	Lex(lex_type lt = LEX_NULL, std::string s = "", const Type& t = Type()): lex_t(lt), lex(s), type(t){};
	lex_type get_lt() const { return lex_t; }
    std::string get_name() const { return lex; }
    Type get_type() const { return type; }
    void set_type(Type new_type) { type = new_type; }
    friend std::ostream& operator << (std::ostream& out, Lex lex){ out << lex; return out;};
private:
    lex_type lex_t;
	std::string lex;
    Type type;
};/**/

class Lex_exception: public std::exception { 
public:
	const char* what() const noexcept{return this->err_msg.c_str();}
    Lex_exception (const std::string &what, const std::pair<int,int>& pos): std::exception(), err_msg("Lex_except: " + std::to_string(pos.first) + ":" + std::to_string(pos.second) + ": " + what) {}
private:
	std::string err_msg;
};

#endif
