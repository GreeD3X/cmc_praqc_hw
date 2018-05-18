#include "recursive.hpp"
#include <stack>
#include <vector>


void Parser::get_lex(){
    cur_lex = *cur_pos;
    cur_type = cur_lex.get_lt();
    ++cur_pos;
    //std::cout << cur_type << std::endl;
}   

void Parser::parse(){
	poliz = std::vector<Lex>(0);
    get_lex();
    S();
    std::cout << "SUCESS" << std::endl;
}

void Parser::S(){
    while (cur_type != LEX_EOF) {
        Expr();
        poliz.push_back(Lex(LEX_EXP,"next expression"));
    }
}

void Parser::Expr(){
    Index_expr();
    while (cur_lex.get_lt() == LEX_ASS) {
        get_lex();
        Index_expr();
        check_op();
        poliz.push_back(Lex(LEX_ASS, "="));
    }
}

void Parser::Index_expr (){
    switch (cur_type) {
        case LEX_ID: 
            check_id();
            st_type.push(cur_lex.get_type());
            poliz.push_back(cur_lex);
            get_lex();
            Index();
            break;
        case LEX_NUM: 
            cur_lex.set_type(Type(TYPE_INT, -2));
            st_type.push(cur_lex.get_type());
            poliz.push_back(cur_lex);
            get_lex();
            break;
        default:
			throw Syn_exception("Expected const or Id", cur_pos.get_pos());
			break;
    }
}

void Parser::Index(){
    if (cur_type == LEX_LBR) {
        poliz.push_back(cur_lex);
        Type tmp = st_type.top();
        st_type.pop();
        --tmp.arr_dim;
        if (tmp.arr_dim < 0) {
            throw Sem_exception("Attempt to index non-array structure", cur_pos.get_pos());
        }
        st_type.push(tmp);
        get_lex();
        Expr();
        if (st_type.top().type != TYPE_INT) {
            throw Sem_exception("Bad index type", cur_pos.get_pos());
        }
        st_type.pop();
        if (cur_type != LEX_RBR) {
            throw Syn_exception("Right bracket is missing", cur_pos.get_pos());
        }
        poliz.push_back(cur_lex);
        get_lex();
        Index();
    }
}

void Parser::check_id ()
{
    std::string name = cur_lex.get_name();
    int cnt = 0;
    if(name.length() != 1)
		for (size_t i = 0; i < name.length() - 1; i++) {
			auto c = name[i];
			if (c != 'a') {
				throw Sem_exception("Unknown variable", cur_pos.get_pos());
			} else {
				++cnt;
			}
		}
	char letter = name[name.length() - 1];
    if (letter == 'i' || letter == 'j' || letter == 'k') {
        cur_lex.set_type(Type(TYPE_INT, cnt));
    } else {
        if (letter == 's' || letter == 't') {
                cur_lex.set_type(Type(TYPE_STRING, cnt));
        } else { 
                throw Sem_exception("Unknown variable", cur_pos.get_pos());
        }
    }
}

void Parser::check_op ()
{
    Type type1, type2;
    type2 = st_type.top();
    st_type.pop();
    type1 = st_type.top();
    st_type.pop();
    if (type1 == type2 || (type2.arr_dim == -2 && type1.arr_dim == 0 && type1.type == TYPE_INT)) {
        st_type.push(type1);
    } else {
		if(type1.arr_dim == -2){
				throw Sem_exception("Left operand cant be const", cur_pos.get_pos());
		}
        throw Sem_exception("Types mismatch in operation", cur_pos.get_pos());
    }
}

void Parser::print_poliz ()
{
    for (const Lex lex : poliz) {
		if(lex.get_lt() == LEX_EXP)
			std::cout << std::endl;
        else
			std::cout << lex << ' ';
    }
    std::cout << std::endl;
}

void Parser::print_expr ()
{
    std::stack<Lex> st_poliz;
    Lex lex0;
    int i = 1;
    for (Lex lex : poliz) {
		lex_type lex_t = lex.get_lt();
        if (lex_t != LEX_ASS && lex_t != LEX_RBR) {
            if (lex_t == LEX_LBR) {
                Lex lex1 = st_poliz.top();
                st_poliz.pop();
                Type type = lex1.get_type();
                --type.arr_dim;
                lex1.set_type(type);
                st_poliz.push(lex1);
            }
            st_poliz.push(lex);
        } else {
            if (lex_t == LEX_RBR) {
                do {
                    lex0 = st_poliz.top();
                    st_poliz.pop();
                } while (lex0.get_lt() != LEX_LBR);
            } else {
                Lex lex1 = st_poliz.top();
                st_poliz.pop();
                Lex lex2 = st_poliz.top();
                st_poliz.pop();
                std::cout << i << ") " << lex2 << ' ' << lex << ' ' << lex1 << " -----> " << lex2.get_type() << std::endl;
                st_poliz.push(Lex(lex2.get_lt(), "Expr" + std::to_string(i), lex2.get_type()));
                ++i;
            }
        }
    }
    std::cout << "Standalone expresions : " << std::endl;
    while (!st_poliz.empty()) {
        Lex lex = st_poliz.top();
        if(lex.get_lt() != LEX_EXP)
			std::cout << i << ") " << st_poliz.top() << " -----> " << lex.get_type() << std::endl;
		++i;
        st_poliz.pop();
    }
}

