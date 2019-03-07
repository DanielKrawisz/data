#include <data/data.hpp>
#include <data/list.hpp>
#include <data/queue.hpp>
#include <data/meta/which.hpp>

namespace data {
    
    namespace meta {
        
        template <typename L>
        struct if_is_list {
            constexpr static list::is_list<L> is_list{};
            
            bool empty(L l) const {
                return list::empty(l);
            }
            
            L rest(L l) const {
                return list::rest(l);
            }
            
            L reverse(L l) const {
                return list::reverse(l);
            }
        };
        
        template <typename L>
        struct has_reverse {
            L reverse(L l) const {
                return l.reverse();
            }
        };
        
        template <typename Any>
        struct no_valid_method {
            bool operator()(const Any a) const {
                return true;
            }
        };
        
        template <typename Any>
        struct no_valid_method<Any*> {
            bool operator()(const Any a) const {
                return true;
            }
        };
        
        template <typename Any>
        struct has_valid_method {
            bool operator()(const Any a) const {
                return a.valid();
            }
        };
        
        template <typename Any>
        struct has_valid_method<Any*> {
            bool operator()(const Any* a) const {
                return a != nullptr ? a->valid() : false;
            }
        };
        
        template <typename Any>
        struct valid {
            bool operator()(const Any a) const {
                return meta::Which<has_valid_method<Any>, no_valid_method<Any>>::result{}(a);
            }
        };
        
    }
    
    template <typename L>
    L reverse(L l) {
        return meta::Which<meta::has_reverse<L>, meta::if_is_list<L>>::result{}.reverse(l);
    }
    
    template <typename Any>
    bool valid(Any a) {
        return meta::valid<Any>{}(a);
    }

}
