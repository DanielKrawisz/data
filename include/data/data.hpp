#ifndef DATA_DATA_HPP
#define DATA_DATA_HPP

#include <data/types.hpp>

namespace data {
    
    template <typename X, typename Y>
    inline X plus(X a, Y b) {
        return a + b;
    }
    
    template <typename Any>
    bool valid(Any);
    
    template <typename List>
    List reverse(List);

}

#endif
