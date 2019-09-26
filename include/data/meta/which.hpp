// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META_WHICH
#define DATA_META_WHICH

namespace data {
    
    namespace meta {
        
        namespace low {
            
            template <bool, typename ...> struct Which;
            
            template <bool b, typename A>
            struct Which<b, A> {
                using result = A;
                constexpr static result Constructable{};
            };
            
            template <bool b, typename first, typename ... rest>
            struct Which<b, first, rest...> {
                using result = typename Which<b, rest...>::result;
                constexpr static result Constructable{};
            };
            
            template <typename first, typename ... rest>
            struct Which<true, first, rest...> {
                using result = first;
                constexpr static result Constructable{};
            };
        }
        
        // contains the first constructable type in a list. 
        template <typename first, typename ... rest> 
        struct Which {
            using result = typename low::Which<true, first, rest...>::result;
        };
        
    }
    
}

#endif
