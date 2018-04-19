#include "lex.hpp"
#include <cstdio>

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

char gc(std::istream& is = std::cin){
	return is.get();
}

void uc(std::istream& is = std::cin){
	is.unget();
}

bool isDelim(char c){
	return /*c==' ' || c == '\n' || c== '\r' || c == '\t' || */isspace(c);
}

bool isSign(char c){
	return c == '[' || c == ']' || c == '=';
}

std::pair<std::string, int> get_lex () {
    char c = gc();
    std::string lex("");
    state CS = LEX;
    do{
        switch(CS){
            case LEX:
                if ( isDelim(c) ) 
                    c = gc();
                else if ( isalpha(c) ){
  	  	            lex += c;
                    c = gc();
                    CS = ID;
                }
                else if ( isdigit(c) ){
                    lex += c;
                    c = gc();
                    CS = NUM;
                }
                else 
                  CS = SGN;
                break;
            case ID:
                if ( isalpha(c) || isdigit(c) ) {
                    lex += c; 
                    c = gc();
                } else{ 
					if ( isDelim(c) || isSign(c) || c == EOF){
						uc();//return c to stream
						return {lex, 0};
					} else{
						std::string er = "Unexpected char: '";
						er += c;
						er += "' while triyng to parse ID: " + lex; 
						throw er;
					}
				}
                break;
            case NUM:
                if (isdigit(c)) {
                    lex += c;
                    c = gc();
                }
                else if (isDelim(c) || isSign(c) || c == EOF){
					uc();
                    return {lex, 0};
				} else{
					std::string er = "Unexpected char: '";
					er += c;
					er += "' while triyng to parse number: " + lex; 
					throw er;
				}
                break;
            case SGN:
                if (isSign(c)){
                    lex += c;
                    return {lex, 0};
                }
                else if (c == EOF)
                    return {"", EOF};
                else{
					std::string er = "Unknown char: '";
					er += c;
					er += "'" + lex; 
					throw er;
				}
                break;
        }//end switch
    } while (true);
}

