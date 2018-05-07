#ifndef iterlexHPP
#define iterlexHPP
#include<functional>
#include "iterable.hpp"
#include "filter.hpp"
#include "lex.hpp"

class LexIterator: public std::iterator<std::input_iterator_tag, const std::string, long, const std::string, const std::string>{
public:
	LexIterator(lex_type st);
    bool operator!=(LexIterator const& other) const;
    bool operator==(LexIterator const& other) const;
    const Lex operator*() const;
    LexIterator& operator++();
    std::pair<int,int> get_pos(){return {cur_str, cur_char};}  
private: 
	char gc(std::istream& is = std::cin) const {return is.get();}
	void uc(std::istream& is = std::cin) const {is.unget();}
	bool isDelim(const char c)const{return /*c==' ' || c == '\n' || c== '\r' || c == '\t' || */isspace(c);}
	bool isSign(const char c) const{return c == '[' || c == ']' || c == '=';}
	Lex get_lex();
    Lex lex;
    int cur_char = 0;
    int cur_str = 1;
};

class Lex_seq: public Iterable<
    LexIterator,
    LexIterator,
    FilterIterator<LexIterator, Lex>, 
    Lex>{
public:
    iterator begin()      ;
    const_iterator begin() const;
    iterator end()        ;
    const_iterator end()   const;
    fiterator filter(const std::function<bool (const Lex&)>& f);
};



#endif
