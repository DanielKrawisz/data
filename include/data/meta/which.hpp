#ifndef DATA_META_WHICH_HPP
#define DATA_META_WHICH_HPP

namespace data {
    
    namespace meta {
        
        namespace low {
            
            template <bool, typename ...> struct Which;
            
            template <bool b, typename A>
            struct Which<b, A> {
                using result = A;
                static const result Constructable{};
            };
            
            template <bool b, typename first, typename ... rest>
            struct Which<b, first, rest...> {
                using result = typename Which<b, rest...>::result;
            };
            
            template <typename first, typename ... rest>
            struct Which<true, first, rest...> {
                using result = first;
                static const result Constructable{};
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
