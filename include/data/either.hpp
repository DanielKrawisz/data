// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EITHER
#define DATA_EITHER

#include <variant>
#include <data/types.hpp>

namespace data {
    template <typename... X> struct either;
    
    template <> struct either<> {
        std::ostream &write (std::ostream &o) const {
            return o;
        }
    };

    template <typename... X> struct either : std::variant<X...> {
        using std::variant<X...>::variant;
        either (std::variant<X...> &&x) : std::variant<X...> {x} {}
        
        std::ostream &write (std::ostream &o) const {
            return writer<X...> {} (o, *this);
        }
        
    private:
        
        template <typename Y, typename... A>
        struct writer {
            std::ostream &operator () (std::ostream &o, const either<X...> &x) {
                if (std::holds_alternative<Y> (x)) return o << std::get<Y> (x);
                return writer<A...> {} (o, x);
            }
        };
        
        template <typename Y>
        struct writer<Y> {
            std::ostream &operator () (std::ostream &o, const either<X...> &x) {
                return o << std::get<Y> (x);
            }
        };
    };
    
    template <typename... X> 
    std::ostream inline &operator << (std::ostream &o, const either<X...> &x) {
        return x.write (o);
    }
}

#endif
