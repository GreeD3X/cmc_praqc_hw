#include "lex.hpp"

/*type_of_lex Lex::get_type(){
    return t_lex;
}

int Lex::get_value(){
    return v_lex;
}

ostream& operator << (ostream &out, const Lex& l){
    out << '(' << l.get_type() << ',' << l.get_value() << ");";
    return out;
}*/

std::ostream& operator << (std::ostream& out, Type type)
{
	if(type.arr_dim == -2){
		out << "const of ";
	}
	else{
		if (type.arr_dim != 0) {
			out << type.arr_dim << " dimensional array of ";
		}
	}
    switch (type.type) {
        case TYPE_NULL :
            out << "TYPE_NULL";
			break;
        case TYPE_INT :
            out << "integer";
            break;
        case TYPE_STRING :
            out << "string";
            break;
		default :
			out << type.type;
			break;
    }
    return out;
}
