#ifndef DATA_TOOLS_LIST_QUEUE_HPP
#define DATA_TOOLS_LIST_QUEUE_HPP

#include <data/queue.hpp>
    
namespace data {

    template <typename l, typename e>
    struct list_queue {
        l List;
            
        bool empty() const {
            return list::empty(List);
        }
            
        e& first() const {
            return list::first(List);
        }

        list_queue rest() const {
            return list_queue{list::rest(List)};
        }
        
        list_queue prepend(e elem) const {
            return list_queue{list::prepend(List, elem)};
        }
        
        list_queue operator+(e elem) const {
            return prepend(elem);
        }
        
        list_queue append(e elem) {
            return list_queue{list::append(List, elem)};
        }

        constexpr static const list::definition::extendable<l, e> requirement_1{};
        constexpr static const queue::definition::queue<list_queue<l, e>, e> requirement_2{};
    };
    

}

#endif
