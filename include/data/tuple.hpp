// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TUPLE
#define DATA_TUPLE

#include <data/types.hpp>

namespace data {

    // my version of tuple is easier for functional programming. 
    template <typename ...> struct tuple;
    
    template <> struct tuple<> {
        tuple() {}
    };
    
    template <typename first, typename... rest> struct tuple<first, rest...> {
        first First;
        tuple<rest...> Rest;
        
        tuple(first f, rest... r) : First{f}, Rest{tuple<rest...>{r...}} {}
    };
    
    namespace meta {
        
        template <uint n, typename tuple> struct get; 
        
        template <typename first, typename... rest>
        struct get<0, tuple<first, rest...>> {
            using type = first;
            type operator()(tuple<first, rest...> t) {
                return t.First;
            }
        };
        
        template <uint n, typename first, typename... rest>
        struct get<n, tuple<first, tuple<rest...>>> {
            using type = typename get<n - 1, rest...>::type;
            type operator()(tuple<first, rest...> t) {
                return get<n - 1, tuple<rest...>>{}(t.Rest);
            }
        };
    
    }
    
    template <uint n, typename tuple>
    typename meta::get<n, tuple>::type get(tuple t) {
        return meta::get<n, tuple>{}(t);
    }

}

#endif

