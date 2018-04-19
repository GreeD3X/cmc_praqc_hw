#ifndef iterlexHPP
#define iterlexHPP
#include<functional>
#include "iterable.hpp"
#include "filter.hpp"
#include "lex.hpp"

class LexIterator: public std::iterator<std::input_iterator_tag, const std::string, long, const std::string, const std::string>{
public:
    LexIterator(int state = 0);
    bool operator!=(LexIterator const& other) const;
    bool operator==(LexIterator const& other) const;
    const std::string operator*() const;
    LexIterator& operator++();  
private:
    int state;
    std::string lex;
};

class Lex_seq: public Iterable<
    LexIterator,
    LexIterator,
    FilterIterator<LexIterator, std::string>, 
    std::string>{
public:
    iterator begin()      ;
    const_iterator begin() const;
    iterator end()        ;
    const_iterator end()   const;
    fiterator filter(const std::function<bool (const std::string&)>& f);

};



#endif
