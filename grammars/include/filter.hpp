#ifndef filterHPP
#define filterHPP

#include <functional>
#include "iterable.hpp"

template<class iterator_type, class T>
class FilterIterator: public std::iterator<std::input_iterator_tag, const T, long, const T, const T>{
public:
    FilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (const T&)>& pred);

    const T operator*() const;
    const T operator->() const;

    FilterIterator& operator++();

    bool operator==(const FilterIterator& other) const;
    bool operator!=(const FilterIterator& other) const;
    const FilterIterator end() const;
private:
    void advance();

    iterator_type icurrent;
    iterator_type iend;
    const std::function<bool (const T&)> pred;
};

template<class iterator_type, class T>
FilterIterator<iterator_type,T>::FilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (const T&)>& pred): 
icurrent(begin), iend(end), pred(pred) {
	while((icurrent != iend) && (!pred(*icurrent)))
		++icurrent;
}//cause while in filter.cpp it wouldnt compile

#endif
