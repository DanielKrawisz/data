#ifndef DATA_DATA
#define DATA_DATA

#include <data/types.hpp>
#include <data/math/number/bounded.hpp>

namespace data {
    
    template <typename X, typename Y>
    inline X plus(X a, Y b) {
        return a + b;
    }
    
    template <typename Any>
    bool valid(Any);
    
    template <typename List>
    List reverse(List);
    
    using uint256 = math::number::bounded<4>;

}

#endif
