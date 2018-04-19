#ifndef iterableHPP
#define iterableHPP
#include <functional>
#include <iostream>

template<class T1, class T2,
         class T3, class T>
class Iterable{
public:
    /*struct iterator_traits<Iterable∗> {
        using difference_type = ptrdiff_t;
        using value_type = Iterable;
        using pointer = Iterable∗;
        using reference = Iterable& reference;
        using iterator_category = input_iterator_tag;
    };*/
    typedef T1 iterator;
    typedef T2 const_iterator;
    typedef T3 fiterator;
    typedef T type;
    virtual iterator begin() = 0;
    virtual const_iterator begin() const = 0;
    virtual iterator end() = 0;
    virtual const_iterator end() const = 0;
    virtual fiterator filter(const std::function<bool (const type&)>& f) = 0;
};

template<class T1, class T2, class T3, class T4>
void printIterable(const Iterable<T1,T2,T3, T4>& Iter){
    std::cout << "-------\nPrinting " << typeid(Iter).name()<< "\n-------" << std::endl;
    for(auto x: Iter){
        std::cout << x << std::endl;
    }
    std::cout << "-------\nFinished printing " << typeid(Iter).name()<< "\n-------" << std::endl;
}
#endif
