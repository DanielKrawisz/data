#ifndef DATA_LIST_INFINITE_HPP
#define DATA_LIST_INFINITE_HPP

#include <data/list.hpp>
#include <data/data.hpp>
#include <data/fold.hpp>
#include <type_traits>
    
namespace data {
        
    template <typename X, typename f>
    struct infinite {
        constexpr static list::definition::list<infinite<X, f>, X> is_list{};
        
        f Function;
        X First;
            
        bool empty() const {
            return false;
        }

        const X& first() const {
            return First;
        }
            
        const infinite rest() const {
            return infinite{Function, Function(First)};
        }
        
    };

}

#endif
