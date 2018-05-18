#include <iostream>
#include "iterlex.hpp"

#include "iterable.hpp"
#include "filter.hpp"
#include "lex.hpp"

LexIterator::LexIterator(lex_type st): lex(st, "") {
    ++(*this);
}

bool LexIterator::operator!=(LexIterator const& other) const { return lex.get_lt() != other.lex.get_lt();}
bool LexIterator::operator==(LexIterator const& other) const { return !(*this != other);}
const Lex LexIterator::operator*() const {return lex;}

LexIterator& LexIterator::operator++(){
    if(lex.get_lt() != LEX_EOF){
        auto lexc = get_lex();
        /*if(lexc.get_lt() == LEX_NULL){
		    throw Lex_exception("Bad lex: ", {cur_str, cur_char});
        }*/
        lex = lexc;
    }
    return *this;
}

Lex LexIterator::get_lex() {
	enum state { LEX, ID, NUM, SGN};
    char c = gc();
    ++cur_char;
    std::string lex("");
    state CS = LEX;
    do{
        switch(CS){
            case LEX:
                if ( isDelim(c) ){ 
					if(c == '\n'){
						++cur_str;
						cur_char = 0;
					}
                    c = gc();
                    ++cur_char;
				}
                else if ( isalpha(c) ){
  	  	            lex += c;
                    c = gc();
                    ++cur_char;
                    CS = ID;
                }
                else if ( isdigit(c) ){
                    lex += c;
                    c = gc();
                    ++cur_char;
                    CS = NUM;
                }
                else 
                  CS = SGN;
                break;
            case ID:
                if ( isalpha(c) || isdigit(c) ) {
                    lex += c; 
                    c = gc();
                    ++cur_char;
                } else{ 
					if ( isDelim(c) || isSign(c) || c == EOF){
						uc();//return c to stream
						--cur_char;
						return {LEX_ID, lex};
					} else{
						std::string er = "Unexpected char: '";
						er += c;
						er += "' while triyng to parse ID: "; 
						throw Lex_exception(er, {cur_str, cur_char});
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
                    return {LEX_NUM, lex};
				} else{
					std::string er = "Unexpected char: '";
					er += c;
					er += "' while triyng to parse number: "; 
					throw Lex_exception(er, {cur_str, cur_char});
				}
                break;
            case SGN:
                if (isSign(c)){
                    lex += c;
                    switch(c){
							case '[':
								return {LEX_LBR, lex};
							case ']':
								return {LEX_RBR, lex};
							case '=':
								return {LEX_ASS, lex};
					}
                    
                }
                else if (c == EOF)
                    return {LEX_EOF, ""};
                else{
					std::string er = "Unknown char: '";
					er += c;
					er += lex + "'"; 
					throw Lex_exception(er, {cur_str, cur_char});
				}
                break;
        }//end switch
    } while (true);
}


  

Lex_seq::iterator Lex_seq::begin()       { return {LEX_NULL}; }
Lex_seq::const_iterator Lex_seq::begin() const { return {LEX_NULL}; }
Lex_seq::iterator Lex_seq::end()         { return {LEX_EOF}; }
Lex_seq::const_iterator Lex_seq::end()   const { return {LEX_EOF}; }
Lex_seq::fiterator Lex_seq::filter(const std::function<bool (const Lex&)>& f) {	
	return {begin(), end(), f};
}
