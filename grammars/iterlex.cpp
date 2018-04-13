#include "iterable.hpp"
#include <iostream>
#include "lex.hpp"

template<class iterator_type, class T>
class FilterIterator: public std::iterator<std::input_iterator_tag, const T, long, const T, const T>{
public:
    FilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (const T&)>& pred): 
    icurrent(begin), iend(end), pred(pred)
    {
    while((icurrent != iend) && (!pred(*icurrent)))
        ++icurrent;
    }

    const T operator*() const { return *icurrent; }
    const T operator->() const { return *icurrent; }

    FilterIterator& operator++() { advance(); return *this; }

    bool operator==(const FilterIterator& other) const { return icurrent == other.icurrent; }
    bool operator!=(const FilterIterator& other) const { return !(operator==(other)); }
    const FilterIterator end() const {return {this->iend, this->iend, this->pred};};
private:
    void advance()
    {
        do
        {
            ++icurrent;
        }
        while((icurrent != iend) && (!pred(*icurrent)));
    } // advance

    iterator_type icurrent;
    iterator_type iend;
    const std::function<bool (const T&)> pred;
};

class LexIterator: public std::iterator<std::input_iterator_tag, const int, long, const int, const int>{
public:
    LexIterator(bool state = true)
	    : state(state), lex("") {
		    ++(*this);
	    }
    bool operator!=(LexIterator const& other) const { return state != other.state;};
    bool operator==(LexIterator const& other) const { return state == other.state;};
    const std::string operator*() const {return lex;};
    LexIterator& operator++(){
        if(state){
                auto [lexc, statec] = get_lex();
                lex = lexc;
                state = statec;
        }
        return *this;
    };  
private:
    bool state;
    std::string lex;
};

template<class T1, class T2, class T3, class T4>
void printIterable(const Iterable<T1,T2,T3, T4>& Iter){
    std::cout << "-------\nPrinting " << typeid(Iter).name()<< "\n-------" << std::endl;
    for(auto x: Iter){
        std::cout << x << std::endl;
    }
    std::cout << "-------\nFinished printing " << typeid(Iter).name()<< "\n-------" << std::endl;
}

class lex_seq: public Iterable<
    LexIterator,
    LexIterator,
    FilterIterator<LexIterator, std::string>, 
    std::string>{
public:
    LexIterator begin()       { return {}; }
    LexIterator begin() const { return {}; }
    LexIterator end()         { return {false}; }
    LexIterator end()   const { return {false}; }
    FilterIterator<LexIterator, std::string> filter(const std::function<bool (const std::string&)>& f) { return { begin(), end(), f }; }
private:
};
