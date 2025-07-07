// Copyright (c) 2018-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NONNEGATIVE
#define DATA_MATH_NONNEGATIVE

#include <data/valid.hpp>
#include <data/ordered.hpp>
#include <data/sign.hpp>

namespace data::math {
    
    template <Ordered R> struct nonnegative;
    template <typename R> struct nonzero;

    template <typename R> constexpr bool operator == (const nonnegative<R> &, const nonnegative<R> &);
    template <typename R> constexpr bool operator == (const nonzero<R> &, const nonzero<R> &);

    template <typename R> constexpr nonnegative<R> operator + (const nonnegative<R> &m, const nonnegative<R> &n);
    template <typename R> constexpr nonnegative<R> operator * (const nonnegative<R> &m, const nonnegative<R> &n);

    template <typename R> constexpr nonzero<R> operator * (const nonzero<R> &m, const nonzero<R> &n);

    template <Ordered R> struct nonnegative {
        R Value;
        
        constexpr explicit nonnegative (const R &n) : Value {n} {}
        constexpr explicit nonnegative (R &&n) : Value {n} {}
        constexpr nonnegative () : Value {-1} {}
        
        constexpr bool valid () {
            return data::valid (Value) && Value >= R (0);
        }
        
        constexpr operator R () const {
            return Value;
        }

        constexpr nonnegative &operator = (const R &n) {
            Value = n;
            return *this;
        }

        constexpr nonnegative &operator = (R &&n) {
            Value = n;
            return *this;
        }
        
        constexpr nonnegative &operator ++ ();
        constexpr nonnegative operator ++ (int);
    };
    
    template <typename R> struct nonzero {
        R Value;
        
        constexpr explicit nonzero (const R &n) : Value {n} {}
        constexpr explicit nonzero (R &&n) : Value {n} {}
        constexpr nonzero () : Value {0} {}
        
        constexpr bool valid () const {
            return data::valid (Value) && Value != R (0);
        }
        
        constexpr operator R () {
            return Value;
        }
        
        constexpr nonzero &operator = (const R &n) {
            Value = n;
            return *this;
        }
        
        constexpr nonzero &operator = (R &&n) {
            Value = n;
            return *this;
        }

        constexpr bool operator == (const R &x) const {
            return Value == x;
        }
    };
    
    template <typename R>
    nonzero (const R &) -> nonzero<R>;

    template <typename R> std::ostream &operator << (std::ostream &o, const nonnegative<R> &n);

    template <typename R> std::ostream &operator << (std::ostream &o, const nonzero<R> &n);
    
}

namespace data {
    template <math::has_sign_function R>
    constexpr math::signature inline sign (const math::nonzero<R> &x) {
        return sign (x.Value);
    }
    
    template <math::has_sign_function R>
    constexpr math::signature inline sign (const math::nonnegative<R> &x) {
        return sign (x.Value);
    }
}

namespace data::math {

    template <typename R>
    constexpr bool inline operator == (const nonnegative<R> &m, const nonnegative<R> &n) {
        return m.Value == n.Value;
    }

    template <typename R>
    constexpr bool inline operator == (const nonzero<R> &m, const nonzero<R> &n) {
        return m.Value == n.Value;
    }

    template <typename R>
    constexpr auto inline operator <=> (const nonnegative<R> &m, const nonnegative<R> &n) {
        return m.Value <=> n.Value;
    }

    template <typename R>
    constexpr auto inline operator <=> (const nonzero<R> &m, const nonzero<R> &n) {
        return m.Value <=> n.Value;
    }

    template <typename R>
    constexpr nonnegative<R> inline operator + (const nonnegative<R> &m, const nonnegative<R> &n) {
        return nonnegative<R> {m.Value + n.Value};
    }

    template <typename R>
    constexpr nonnegative<R> inline operator * (const nonnegative<R> &m, const nonnegative<R> &n) {
        return nonnegative<R> {m.Value * n.Value};
    }

    template <typename R>
    constexpr nonzero<R> inline operator * (const nonzero<R> &m, const nonzero<R> &n) {
        return nonzero<R> {m.Value * n.Value};
    }
    
    template <Ordered R> constexpr nonnegative<R> inline &nonnegative<R>::operator ++ () {
        ++Value;
        return *this;
    }
    
    template <Ordered R> constexpr nonnegative<R> inline nonnegative<R>::operator ++ (int) {
        nonnegative<R> n = *this;
        ++Value;
        return n;
    }

    template <typename R>
    std::ostream inline &operator << (std::ostream &o, const nonnegative<R> &n) {
        return o << n.Value;
    }

    template <typename R>
    std::ostream inline &operator << (std::ostream &o, const nonzero<R> &n) {
        return o << n.Value;
    }

}

#endif
