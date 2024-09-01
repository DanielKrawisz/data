// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EITHER
#define DATA_EITHER

#include <data/meta.hpp>

namespace data {

    template <typename... X> struct either;
    
    template <> struct either<> {
        either () = delete;
    };

    template <typename... X> struct either : std::variant<meta::contain<X>...> {
        using std::variant<meta::contain<X>...>::variant;
        constexpr either (std::variant<X...> &&x) : std::variant<meta::contain<X>...> {x} {}
        
        using std::variant<meta::contain<X>...>::operator =;

        std::ostream &write (std::ostream &o) const {
            return writer<meta::replace<X, meta::rule<void, std::monostate>>...> {} (o, *this);
        }

        constexpr bool valid () const {
            return !this->valueless_by_exception ();
        }

        template <typename Z>
        constexpr bool is () const {
            return std::holds_alternative<meta::contain<Z>> (*this);
        }

        template <typename Z>
        constexpr meta::retrieve<Z> get () {
            return (meta::retrieve<Z>) std::get<meta::contain<Z>> (*this);
        }

        template <typename Z>
        constexpr const meta::retrieve<Z> get () const {
            return (const meta::retrieve<Z>) std::get<meta::contain<Z>> (*this);
        }

        template <typename Z>
        constexpr Z *get_if () {
            return std::get_if<meta::contain<Z>> (*this);
        }

        template <typename Z>
        constexpr const Z *get_if () const {
            return std::get_if<meta::contain<Z>> (*this);
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
