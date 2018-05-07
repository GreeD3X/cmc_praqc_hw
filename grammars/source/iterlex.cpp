#include <iostream>
#include "iterlex.hpp"

#include "iterable.hpp"
#include "filter.hpp"
#include "lex.hpp"

LexIterator::LexIterator(lex_type st): lex("", st) {
    ++(*this);
}
bool LexIterator::operator!=(LexIterator const& other) const { return lex.t_lex != other.lex.t_lex;}
bool LexIterator::operator==(LexIterator const& other) const { return !(*this != other);}
const Lex LexIterator::operator*() const {return lex;}

LexIterator& LexIterator::operator++(){
    if(lex.t_lex != LEX_EOF){
        auto lexc = get_lex();
        if(lexc.t_lex == LEX_NULL){
		    throw "Bad lex";
        }
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
						return {lex, LEX_ID};
					} else{
						std::string er = "Unexpected char: '";
						er += c;
						er += "' while triyng to parse ID: " + ' ' + std::to_string(cur_str) + ':' + std::to_string(cur_char); 
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
                    return {lex, LEX_NUM};
				} else{
					std::string er = "Unexpected char: '";
					er += c;
					er += "' while triyng to parse number: " + ' ' + std::to_string(cur_str) + ':' + std::to_string(cur_char); 
					throw er;
				}
                break;
            case SGN:
                if (isSign(c)){
                    lex += c;
                    return {lex, LEX_OP};
                }
                else if (c == EOF)
                    return {"", LEX_EOF};
                else{
					std::string er = "Unknown char: '";
					er += c;
					er += lex + "'" + ' ' + std::to_string(cur_str) + ':' + std::to_string(cur_char); 
					throw er;
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
