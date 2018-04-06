#ifndef iterableHPP
#define iterableHPP
#include <functional>

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
    virtual T1 begin() = 0;
    virtual T2 begin() const = 0;
    virtual T1 end() = 0;
    virtual T2 end() const = 0;
    virtual T3 filter(const std::function<bool (T)>& f) = 0;
};
#endif
