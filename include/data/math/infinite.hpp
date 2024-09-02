// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INFINITE
#define DATA_MATH_INFINITE

#include <data/maybe.hpp>
#include <data/either.hpp>
#include <data/math/ordered.hpp>
#include <data/math/sign.hpp>
#include <iostream>

namespace data::math {
    // attach infinite values to a type.
    template <ordered X> struct unsigned_limit;
    template <ordered X> struct signed_limit;

    template <ordered X> std::ostream &operator << (std::ostream &, const unsigned_limit<X> &x);
    template <ordered X> std::ostream &operator << (std::ostream &, const signed_limit<X> &x);
    
    template <ordered X> struct unsigned_limit {
        using comparison = decltype (std::declval<X> () <=> std::declval<X> ());

        constexpr unsigned_limit (): Value {X {}} {}
        constexpr unsigned_limit (const X &x) : Value {x} {}
        constexpr unsigned_limit (X &&x) : Value {x} {}
        
        constexpr bool infinite () const {
            return !bool (Value);
        }

        constexpr bool finite () const {
            return bool (Value);
        }
        
        static const unsigned_limit &infinity ();

        comparison operator <=> (const unsigned_limit &x) const {
            return x.finite () ? *this <=> *x.Value :
                (infinite () ? X {1} : X {0}) <=> X {1};
        }

        bool operator == (const unsigned_limit &x) const {
            return Value == x.Value;
        }

        comparison operator <=> (const X &x) const {
            return infinite () ? X {1} <=> X {0} : *Value <=> x;
        }

        bool operator == (const X &x) const {
            return infinite () ? false : *Value == x;
        }

        signature sign () const {
            return infinite () ? positive : data::sign (*Value);
        }
        
        constexpr unsigned_limit operator + (const X &x) const {
            if (infinite ()) return infinity ();
            return {*Value + x};
        }
        
        constexpr unsigned_limit operator * (const X &x) const {
            if (infinite ()) return infinity ();
            return {*Value * x};
        }

        maybe<X> Value;
    private:
        constexpr unsigned_limit (maybe<X> &&x, int) : Value {x} {}
    };
    
    template <ordered X> struct signed_limit {
        using comparison = decltype (std::declval<X> () <=> std::declval<X> ());

        constexpr signed_limit () : Value {X {}} {}
        constexpr signed_limit (const X &x) : Value {x} {}
        constexpr signed_limit (X &&x, int) : Value {x} {}

        static const signed_limit &infinity ();

        static const signed_limit &negative_infinity ();
        
        constexpr bool infinite () const {
            return Value.template is<bool> ();
        }

        constexpr bool finite () const {
            return !Value.template is<bool> ();
        }
        
        constexpr bool positive_infinite () const {
            return Value.template is<bool> () && Value.template get<bool> ();
        }
        
        constexpr bool negative_infinite () const {
            return Value.template is<bool> () && !Value.template get<bool> ();
        }

        comparison operator <=> (const signed_limit &x) const {
            return x.finite () ? *this <=> x.Value.template get<X> () :
                infinite () ? ((this->Value.template get<bool> () ? X {1} : X {0}) <=> (x.Value.template get<bool> () ? X {1} : X {0})) :
                    x.Value.template get<bool> () ? X {0} <=> X {1} : X {1} <=> X {0};
        }

        bool operator == (const signed_limit &x) const {
            return Value == x.Value;
        }

        comparison operator <=> (const X &x) const {
            return finite () ? Value.template get<X> () <=> x :
                Value.template get<bool> () ? X {1} <=> X {0} : X {0} <=> X {1};
        }

        bool operator == (const X &x) const {
            return finite () ? Value.template get<X> () == x : false;
        }

        signature sign () const {
            return finite () ? data::sign (*static_cast<either<X, bool>> (*this)) : positive_infinite () ? positive : negative;
        }

        signed_limit operator - () const {
            return finite () ? signed_limit {-Value.template get<X> ()} : Value.template get<bool> ? negative_infinity () : infinity ();
        }

        either<X, bool> Value;
    private:
        constexpr signed_limit (either<X, bool> &&v, int) : Value {v} {}
    };
}

namespace data {
    // we don't use all the cardinal numbers because
    // infinite is as big as we can get for now. 
    using cardinality = math::unsigned_limit<size_t>;
}

namespace data::math {

    template <ordered X> std::ostream inline &operator << (std::ostream &o, const unsigned_limit<X> &x) {
        if (x.infinite ()) return o << "infinity";
        return o << *x.Value;
    }

    template <ordered X> std::ostream inline &operator << (std::ostream &o, const signed_limit<X> &x) {
        if (x.finite ()) return o << x.Value.template get<X> ();
        if (!x.Value.template get<bool> ()) o << "-";
        return o << "infinity";
    }

    template <ordered X> const unsigned_limit<X> &unsigned_limit<X>::infinity () {
        static unsigned_limit<X> Infinity {maybe<X> {}, 0};
        return Infinity;
    }

    template <ordered X> const signed_limit<X> &signed_limit<X>::infinity () {
        static signed_limit<X> Infinity {either<X, bool> {true}, 0};
        return Infinity;
    }

    template <ordered X> const signed_limit<X> &signed_limit<X>::negative_infinity () {
        static signed_limit<X> Infinity {either<X, bool> {false}, 0};
        return Infinity;
    }
}

#endif 

