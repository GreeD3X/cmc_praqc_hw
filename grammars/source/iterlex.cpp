#include <iostream>
#include "iterlex.hpp"

LexIterator::LexIterator(bool state): state(state), lex("") {
    ++(*this);
}
bool LexIterator::operator!=(LexIterator const& other) const { return state != other.state;};
bool LexIterator::operator==(LexIterator const& other) const { return state == other.state;};
const std::string LexIterator::operator*() const {return lex;};
LexIterator& LexIterator::operator++(){
    if(state){
        auto [lexc, statec] = get_lex();
        lex = lexc;
        state = statec;
        if(state == -5){
		    throw "Bad lex";
        }
    }
    return *this;
}  

Lex_seq::iterator Lex_seq::begin()       { return {true}; }
Lex_seq::const_iterator Lex_seq::begin() const { return {true}; }
Lex_seq::iterator Lex_seq::end()         { return {false}; }
Lex_seq::const_iterator Lex_seq::end()   const { return {false}; }
Lex_seq::fiterator Lex_seq::filter(const std::function<bool (const std::string&)>& f) {	
	return {begin(), end(), f};
}
