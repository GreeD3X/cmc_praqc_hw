#include "filter.hpp"
#include <functional>

template<class iterator_type, class T>
const T FilterIterator<iterator_type,T>::operator*() const {
	return *icurrent;
}

template<class iterator_type, class T>
const T FilterIterator<iterator_type,T>::operator->() const {
	return *icurrent; 
}

template<class iterator_type, class T>
FilterIterator<iterator_type,T>& FilterIterator<iterator_type,T>::operator++() { 
	advance(); 
	return *this; 
}

template<class iterator_type, class T>
bool FilterIterator<iterator_type,T>::operator==(const FilterIterator& other) const { 
	return icurrent == other.icurrent; 
}

template<class iterator_type, class T>
bool FilterIterator<iterator_type,T>::operator!=(const FilterIterator& other) const { 
	return !(operator==(other)); 
}

template<class iterator_type, class T>
const FilterIterator<iterator_type,T> FilterIterator<iterator_type,T>::end() const {
	return {this->iend, this->iend, this->pred};
}

template<class iterator_type, class T>
void FilterIterator<iterator_type,T>::advance(){
    do
    {
        ++icurrent;
    }
    while((icurrent != iend) && (!pred(*icurrent)));
} // advance
