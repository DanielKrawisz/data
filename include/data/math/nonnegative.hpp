// Copyright (c) 2018-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NONNEGATIVE
#define DATA_MATH_NONNEGATIVE

#include <data/valid.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/ordered.hpp>
#include <data/math/sign.hpp>
#include <data/math/lowest.hpp>

namespace data::math {
    
    template <ordered R> struct nonnegative {
        R Value;
        
        nonnegative(const R& n) : Value{n} {}
        nonnegative(R&& n) : Value{n} {}
        nonnegative() : Value{-1} {}
        
        bool valid() {
            return data::valid(Value) && Value >= R(0);
        }
        
        operator R() const {
            return Value;
        }
        
        nonnegative& operator++();
        nonnegative operator++(int);
    };
    
    template <ordered R> struct lowest<nonnegative<R>> {
        nonnegative<R> operator()() {
            return nonnegative{R{0}};
        }
    };
    
    template <typename R> struct nonzero {
        R Value;
        
        nonzero(const R &n) : Value{n} {}
        nonzero(R &&n) : Value{n} {}
        nonzero() : Value{0} {}
        
        bool valid() const {
            return data::valid(Value) && Value != R(0);
        }
        
        operator R() const {
            return Value;
        }
        
        nonzero& operator=(const R &n) {
            Value = n;
            return *this;
        }
        
        nonzero& operator=(R &&n) {
            Value = n;
            return *this;
        }
    };
    
    template <typename X> struct identity<times<X>, nonzero<X>> {
        nonzero<X> operator()() {
            return 1;
        }
    };
    
    template <typename X> struct inverse<times<X>, nonzero<X>> {
        nonzero<X> operator()(const nonzero<X> &a, const nonzero<X> &b) {
            return b / a;
        }
    };
    
}

namespace data {
    template <math::has_sign_function R>
    math::sign inline sign(const math::nonzero<R> &x) {
        return sign(x.Value);
    }
    
    template <math::has_sign_function R>
    math::sign inline sign(const math::nonnegative<R> &x) {
        return sign(x.Value);
    }
}

namespace data::math {

    template <ordered R>
    nonnegative<R> inline operator+(const nonnegative<R>& m, const nonnegative<R>& n) {
        return nonnegative<R>{m.Value + n.Value};
    }

    template <ordered R>
    nonnegative<R> inline operator*(const nonnegative<R>& m, const nonnegative<R>& n) {
        return nonnegative<R>{m.Value * n.Value};
    }

    template <ordered R>
    bool inline operator==(const nonnegative<R>& m, const nonnegative<R>& n) {
        return m.Value == n.Value;
    }

    template <ordered R>
    bool inline operator!=(const nonnegative<R>& m, const nonnegative<R>& n) {
        return m.Value != n.Value;
    }

    template <ordered R>
    bool inline operator>(const nonnegative<R>& m, const nonnegative<R>& n) {
        return m.Value > n.Value;
    }

    template <ordered R>
    bool inline operator>=(const nonnegative<R>& m, const nonnegative<R>& n) {
        return m.Value >= n.Value;
    }

    template <ordered R>
    bool inline operator<(const nonnegative<R>& m, const nonnegative<R>& n) {
        return m.Value < n.Value;
    }

    template <ordered R>
    bool inline operator<=(const nonnegative<R>& m, const nonnegative<R>& n) {
        return m.Value <= n.Value;
    }
    
    template <ordered R> nonnegative<R> inline &nonnegative<R>::operator++() {
        ++Value;
        return *this;
    }
    
    template <ordered R> nonnegative<R> inline nonnegative<R>::operator++(int) {
        nonnegative<R> n = *this;
        ++Value;
        return n;
    }

    template <typename R>
    nonzero<R> inline operator*(const nonzero<R>& m, const nonzero<R>& n) {
        return nonzero<R>{m.Value * n.Value};
    }

    template <typename R>
    bool inline operator==(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value == n.Value;
    }

    template <typename R>
    bool inline operator!=(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value != n.Value;
    }

    template <typename R>
    bool inline operator>(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value > n.Value;
    }

    template <typename R>
    bool inline operator>=(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value >= n.Value;
    }

    template <typename R>
    bool inline operator<(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value < n.Value;
    }

    template <typename R>
    bool inline operator<=(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value <= n.Value;
    }

}

#endif
