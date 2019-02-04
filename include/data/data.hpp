#ifndef DATA_DATA_HPP
#define DATA_DATA_HPP

#include <data/types.hpp>
#include <data/list.hpp>

namespace data {
    
    template <typename Any>
    inline Any plus(Any a, Any b) {
        return a + b;
    }
    
    template <typename Any>
    bool valid(Any);
    
    template <typename List, typename Element>
    typename list::is_list<List>::element& first(List);
    
    template <typename List>
    List rest();
    
    template <typename Container>
    bool empty();
    
    template <typename List>
    List reverse(List);

}

#endif
