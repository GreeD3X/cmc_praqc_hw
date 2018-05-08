#include "recursive.hpp"

void Parser::get_lex(){
    ++cur_pos;
    cur_lex = *cur_pos;
    cur_type = cur_lex.get_lt();
}   

void Parser::parse(){
    get_lex();
    S();
    std::cout << "SUCESS" << std::endl;
}

void Parser::S(){
    while (cur_type != LEX_EOF) {
        Expr();
    }
}

void Parser::Expr(){
    Index_expr();
    while (cur_lex.get_lt() == LEX_ASS) {
        get_lex();
        Index_expr();
        check_op();
        poliz.push_back(Lex(LEX_ASS));
    }
}

void Parser::Ass_expr (){
    Index_expr();
    while (cur_lex.get_lt() == LEX_ASS) {
        get_lex();
        Index_expr();
        check_op();
        poliz.push_back(Lex(LEX_ASS));
    }
}

void Parser::Index_expr (){
    switch (cur_type) {
        case LEX_ID: 
            check();
            st_type.push(cur_lex.get_type());
            poliz.push_back(cur_lex);
            get_lex();
            Index();
            break;
        case LEX_NUM: 
            cur_lex.set_type(Type(TYPE_INT, 0));
            st_type.push(Type(TYPE_INT, 0));
            poliz.push_back(cur_lex);
            get_lex();
            break;
        default:
			break;
    }
}

void Parser::Index()
{
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
        if (st_type.top() != Type(TYPE_INT, 0)) {
            throw Sem_exception("Bad index", cur_pos.get_pos());
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

void Parser::check ()
{
    std::string name = cur_lex.get_name();
    char letter = 0;
    int cnt = -1;
    for (auto c : name) {
        if (c != 'i' && c != 'j' && c != 'k' && c != 's' && c != 't' && c != 'a') {
            throw Sem_exception("Unknown variable", cur_pos.get_pos());
        } else {
            ++cnt;
            letter = c;
        }
    }
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
    if (type1 == Type(TYPE_INT, 0) && type2 == Type(TYPE_INT, 0)) {
        st_type.push(Type(TYPE_INT, 0));
    } else {
        throw Sem_exception("Types mismatch", cur_pos.get_pos());
    }
}

void Parser::print_poliz ()
{
    for (const Lex &lex : poliz) {
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
        if (lex_t != LEX_PLUS && lex_t != LEX_TIMES && lex_t != LEX_RBR) {
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
                std::cout << i << ") " << lex1 << ' ' << lex << ' ' << lex2 << " -----> " << Type(TYPE_INT, 0) << std::endl;
                st_poliz.push(Lex(lex1.get_lex_type(), "Expr" + std::to_string(i), lex1.get_type()));
                ++i;
            }
        }
    }
    std::cout << "Standalone expresions : " << std::endl;
    while (!st_poliz.empty()) {
        Lex lex = st_poliz.top();
        std::cout << i << ") " << st_poliz.top() << " -----> " << lex.get_type() << std::endl;
        ++i;
        st_poliz.pop();
    }
}

