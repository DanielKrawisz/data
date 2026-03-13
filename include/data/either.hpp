// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EITHER
#define DATA_EITHER

#include <data/meta.hpp>
#include <data/valid.hpp>

namespace data {

    template <typename... X> struct either;
    
    template <> struct either<> {
        either () = delete;
    };

    // either works just like std::variant except that it is less wordy
    // and can take references and void as subtypes.
    template <typename... X> struct either : public std::variant<wrapped<X>...> {
        using parent = std::variant<wrapped<X>...>;
        using parent::variant;
        constexpr either (parent &&x) : parent {x} {}
        
        using std::variant<wrapped<X>...>::operator =;

        std::ostream &write (std::ostream &o) const {
            return writer<meta::replace<X, meta::rule<void, std::monostate>>...> {} (o, *this);
        }

        template <typename F>
        constexpr auto visit (F&& f) const {
            return std::visit (std::forward<F> (f), static_cast<const either<wrapped<X>...>> (*this));
        }

        constexpr bool valid () const {
            if (this->valueless_by_exception ()) return false;
            return this->visit ([] (auto &&val) {
                return data::valid (val);
            });
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
            return std::get_if<wrapped<Z>> (static_cast<parent *> (this));
        }

        template <typename Z>
        constexpr const Z *get_if () const {
            return std::get_if<wrapped<Z>> (static_cast<const parent *> (this));
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
