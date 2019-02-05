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
        
    }
    
    template <typename L>
    L reverse(L l) {
        return meta::Which<meta::has_reverse<L>, meta::if_is_list<L>>{}.reverse(l);
    }

}
