#include <data/data.hpp>
#include <data/list.hpp>
#include <data/queue.hpp>
#include <data/meta/which.hpp>

namespace data {
    
    namespace meta {
        
        template <typename L>
        struct if_is_list {
            constexpr static list::is_list<L> is_list{};
            
            typename list::is_list<L>::element& first(L l) const {
                list::first(l);
            }
            
            bool empty(L l) const {
                list::empty(l);
            }
            
            L rest(L l) const {
                list::rest(l);
            }
            
            L reverse(L l) const {
                list::reverse(l);
            }
        };
        
        template <typename L>
        struct if_is_queue {
            constexpr static queue::is_queue<L> is_queue{};
            
            typename queue::is_queue<L>::element& first(L l) const {
                queue::first(l);
            }
            
            bool empty(L l) const {
                queue::empty(l);
            }
            
            L rest(L l) const {
                queue::rest(l);
            }
        };
        
        template <typename L>
        struct has_reverse {
            L reverse(L l) const {
                return l.reverse();
            }
        };
        
    }
    
    template <typename L, typename X>
    X& first(L l) {
        return meta::Which<meta::if_is_list<L>, meta::if_is_queue<L>>{}.first(l);
    }
    
    template <typename L>
    L rest(L l) {
        return meta::Which<meta::if_is_list<L>, meta::if_is_queue<L>>{}.rest(l);
    }
    
    template <typename L>
    bool empty(L l) {
        return meta::Which<meta::if_is_list<L>, meta::if_is_queue<L>>{}.empty(l);
    }
    
    template <typename L>
    L reverse(L l) {
        return meta::Which<meta::has_reverse<L>, meta::if_is_list<L>>{}.reverse(l);
    }

}
