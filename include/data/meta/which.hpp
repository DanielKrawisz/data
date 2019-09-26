// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META_WHICH
#define DATA_META_WHICH

#include <type_traits>
#include "require.hpp"

namespace data {
    
    namespace meta {
        
        namespace low {
            
            template <bool, typename ...> struct which;
            
            template <bool b, typename first>
            struct which<b, first> {
                using result = first;
                constexpr static first Constructable{};
            };
            
            template <bool b, typename first, typename ... rest>
            struct which<b, first, rest...> : which<b, rest...> {
                using result = typename which<b, rest...>::result;
            };
            
            template <typename first, typename ... rest>
            struct which<true, first, rest...> {
                using result = first;
                constexpr static first Constructable{};
            };
        }
        
        // contains the first constructable type in a list. 
        template <typename first, typename ... rest> 
        struct Which {
            using result = typename low::which<true, first, rest...>::result;
        };
        
    }
    
}

#endif
