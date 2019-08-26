// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/list/linked.hpp>
    
namespace data::list {
    namespace low {
        template <typename list, typename ... x> struct build_list;
        
        template <typename list> struct build_list<list> {
            list operator()(list l) {
                return l;
            }
        };
        
        template <typename list, typename x> struct build_list<list, x> {
            list operator()(list l, x arg) {
                return l + arg;
            }
        };
        
        template <typename list, typename x, typename ... y> struct build_list<list, x, y...> {
            list operator()(list l, x first, y... rest) {
                build_list<list>{}(l + first, rest...);
            }
        };
    }
    
    template <typename elem>
    template <typename ... x>
    linked<elem> linked<elem>::make(x... arg) {
        return low::build_list<linked>{}(linked{}, arg...);
    }
    
    template <typename elem>
    template <typename ... x>
    linked<elem&> linked<elem&>::make(x... arg) {
        return low::build_list<linked>{}(linked{}, arg...);
    }
}
