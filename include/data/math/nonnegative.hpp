// Copyright (c) 2018-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NONNEGATIVE
#define DATA_MATH_NONNEGATIVE

#include <data/valid.hpp>
#include <data/math/ordered.hpp>

namespace data::math {
    
    template <typename R> struct nonnegative : interface::ordered<R> {
        R Value;
        
        explicit nonnegative(const R& n) : Value{n} {}
        nonnegative() : Value{} {}
        
        bool valid() {
            return data::valid(Value) && Value >= R(0);
        }
        
        operator R() const {
            return Value;
        }
    };
    
    template <typename R> struct nonzero {
        R Value;
        
        explicit nonzero(const R& n) : Value{n} {}
        nonzero() : Value{} {}
        
        bool valid() const {
            return data::valid(Value) && Value != R(0);
        }
        
        operator R() const {
            return Value;
        }
    };

}

template <typename R>
inline data::math::nonnegative<R> operator+(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return data::math::nonnegative<R>{m.Value + n.Value};
}

template <typename R>
inline data::math::nonnegative<R> operator*(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return data::math::nonnegative<R>{m.Value * n.Value};
}

template <typename R>
inline bool operator==(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return m.Value == n.Value;
}

template <typename R>
inline bool operator!=(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return m.Value != n.Value;
}

template <typename R>
inline bool operator>(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return m.Value > n.Value;
}

template <typename R>
inline bool operator>=(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return m.Value >= n.Value;
}

template <typename R>
inline bool operator<(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return m.Value < n.Value;
}

template <typename R>
inline bool operator<=(const data::math::nonnegative<R>& m, const data::math::nonnegative<R>& n) {
    return m.Value <= n.Value;
}

template <typename R>
inline data::math::nonzero<R> operator*(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value * n.Value;
}

template <typename R>
inline bool operator==(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value == n.Value;
}

template <typename R>
inline bool operator!=(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value != n.Value;
}

template <typename R>
inline bool operator>(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value > n.Value;
}

template <typename R>
inline bool operator>=(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value >= n.Value;
}

template <typename R>
inline bool operator<(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value < n.Value;
}

template <typename R>
inline bool operator<=(const data::math::nonzero<R>& m, const data::math::nonzero<R>& n) {
    return m.Value <= n.Value;
}

#endif
