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

    // either works just like std::variant except that it is less wordy
    // and can take references and void as subtypes.
    template <typename... X> struct either : std::variant<wrapped<X>...> {
        using std::variant<wrapped<X>...>::variant;
        constexpr either (std::variant<X...> &&x) : std::variant<wrapped<X>...> {x} {}
        
        using std::variant<wrapped<X>...>::operator =;

        std::ostream &write (std::ostream &o) const {
            return writer<meta::replace<X, meta::rule<void, std::monostate>>...> {} (o, *this);
        }

        constexpr bool valid () const {
            return !this->valueless_by_exception ();
        }

        template <typename Z>
        constexpr bool is () const {
            return std::holds_alternative<wrapped<Z>> (*this);
        }

        template <typename Z>
        constexpr retrieved<Z> get () {
            return (retrieved<Z>) std::get<wrapped<Z>> (*this);
        }

        template <typename Z>
        constexpr const retrieved<Z> get () const {
            return (const retrieved<Z>) std::get<wrapped<Z>> (*this);
        }

        template <typename Z>
        constexpr Z *get_if () {
            return std::get_if<wrapped<Z>> (*this);
        }

        template <typename Z>
        constexpr const Z *get_if () const {
            return std::get_if<wrapped<Z>> (*this);
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
