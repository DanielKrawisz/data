#ifndef DATA_APPEND_HPP
#define DATA_APPEND_HPP

#include <data/queue.hpp>
#include <data/meta/which.hpp>

namespace data
{

    template <typename l, typename e>
    inline l prepend(l ls, e elem) {
        list::prepend(ls, elem);
    }
    
    namespace meta {
        
        template <typename l, typename e>
        struct append_queue {
            static l append(l ls, e elem) {
                return queue::definition::queue<l, e>{}.append(ls, elem);
            }
        };
        
        template <typename l, typename e>
        struct append_list {
            static l append(l ls, e elem) {
                return list::append(ls, elem);
            }
        };
    }
    
    template <typename l, typename e>
    inline l append(l ls, e elem) {
        return meta::Which<meta::append_queue<l, e>, meta::append_list<l, e>>::Result::append(ls, elem);
    }

}

#endif

