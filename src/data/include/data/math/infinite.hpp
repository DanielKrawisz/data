// Copyright (c) 2020-2024 Daniel Krawisz
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

    // unsigned limit is suitable for adding an infinite value to unsigned numbers
    // as well as to unordered types to represent a point at infinity, such as for
    // the Riemann sphere.
    template <typename X> struct unsigned_limit;

    // signed limit is suitable for types along the real line, such as the integers or rationals.
    template <ordered X> struct signed_limit;

    template <typename X, typename Y>
    requires implicitly_convertible_to<Y, X>
    constexpr bool operator == (const unsigned_limit<X> &, const unsigned_limit<Y> &);

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool operator == (const signed_limit<X> &, const signed_limit<Y> &);

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool operator == (const unsigned_limit<X> &, const Y &);

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool operator == (const signed_limit<X> &, const Y &);

    // if X is ordered, then an ordering operation exists
    // that sets infinity higher than any finite value.
    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto operator <=> (const unsigned_limit<X> &, const unsigned_limit<Y> &);

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto operator <=> (const signed_limit<X> &, const signed_limit<Y> &);

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto operator <=> (const unsigned_limit<X> &, const Y &);

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto operator <=> (const signed_limit<X> &, const Y &);

    template <typename X> constexpr bool is_infinite (const X &);
    bool inline is_infinite (const float &x);
    bool inline is_infinite (const double &x);
    bool inline is_infinite (const long double &x);
    template <typename X> constexpr bool is_infinite (const unsigned_limit<X> &);
    template <typename X> constexpr bool is_infinite (const signed_limit<X> &);

    template <typename X> requires requires {
        typename sign<X>;
    } struct sign<unsigned_limit<X>> {
        constexpr signature operator () (const unsigned_limit<X> &x) const;
    };

    template <ordered X> requires requires {
        typename sign<X>;
    } struct sign<signed_limit<X>> {
        constexpr signature operator () (const signed_limit<X> &x) const;
    };

    template <typename X> std::ostream &operator << (std::ostream &, const unsigned_limit<X> &x);
    template <ordered X> std::ostream &operator << (std::ostream &, const signed_limit<X> &x);
    
    template <typename X> struct unsigned_limit {
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

        constexpr static const unsigned_limit Infinity {maybe<X> {}, 0};
        
        constexpr static const unsigned_limit &infinity () {
            return Infinity;
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
        constexpr signed_limit (X &&x) : Value {x} {}

        constexpr static const signed_limit Infinity {either<X, bool> {true}, 0};
        constexpr static const signed_limit NegativeInfinity {either<X, bool> {false}, 0};

        constexpr static const signed_limit &infinity () {
            return Infinity;
        }

        constexpr static const signed_limit &negative_infinity () {
            return NegativeInfinity;
        }
        
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

        signed_limit operator - () const {
            return finite () ? signed_limit {-Value.template get<X> ()} : Value.template get<bool> ? negative_infinity () : infinity ();
        }

        either<X, bool> Value;
    private:
        constexpr signed_limit (either<X, bool> &&v, int) : Value {v} {}
    };

    template <typename X> struct numeric_limits;

    template <std::integral X> struct numeric_limits<X> {
        constexpr static const X Max = std::numeric_limits<X>::max ();
        constexpr static const X Min = std::numeric_limits<X>::min ();

        constexpr static const X &max () {
            return Max;
        }

        constexpr static const X &min () {
            return Min;
        }
    };

    template <typename X> constexpr bool inline is_infinite (const X &) {
        return false;
    }

    bool inline is_infinite (const float &x) {
        return std::isinf (x);
    }

    bool inline is_infinite (const double &x) {
        return std::isinf (x);
    }

    bool inline is_infinite (const long double &x) {
        return std::isinf (x);
    }

    template <typename X> constexpr bool inline is_infinite (const unsigned_limit<X> &x) {
        return x.is_infinite ();
    }

    template <typename X> constexpr bool inline is_infinite (const signed_limit<X> &x) {
        return x.is_infinite ();
    }
}

namespace data {
    // we don't use all the cardinal numbers because
    // infinite is as big as we can get for now. 
    using cardinality = math::unsigned_limit<size_t>;
}

namespace data::math {

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool inline operator == (const unsigned_limit<X> &a, const unsigned_limit<Y> &b) {
        return a.Value == static_cast<maybe<X>> (b.Value);
    }

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool inline operator == (const signed_limit<X> &a, const signed_limit<Y> &b) {
        return a.Value == static_cast<either<X, bool>> (b.Value);
    }

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool inline operator == (const unsigned_limit<X> &a, const Y &b) {
        return a.infinite () ? false : *a.Value == static_cast<X> (b);
    }

    template <typename X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr bool inline operator == (const signed_limit<X> &a, const Y &b) {
        return a.finite () ? a.Value.template get<X> () == static_cast<X> (b) : false;
    }

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto inline operator <=> (const unsigned_limit<X> &a, const unsigned_limit<Y> &b) {
        return b.finite () ? a <=> static_cast<X> (*b.Value) : a.infinite () ? X {0} <=> X {0} : unsigned_limit<X>::comparison::less;
    }

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto inline operator <=> (const signed_limit<X> &a, const signed_limit<Y> &b) {
        return b.finite () ? a <=> static_cast<X> (b.Value.template get<Y> ()) :
            a.infinite () ? ((a.Value.template get<bool> () ? X {1} : X {0}) <=> (b.Value.template get<bool> () ? X {1} : X {0})) :
                    b.Value.template get<bool> () ? X {0} <=> X {1} : X {1} <=> X {0};
    }

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto inline operator <=> (const unsigned_limit<X> &a, const Y &b) {
        return a.infinite () ? unsigned_limit<X>::comparison::greater : *a.Value <=> static_cast<X> (b);
    }

    template <ordered X, typename Y> requires implicitly_convertible_to<Y, X>
    constexpr auto inline operator <=> (const signed_limit<X> &a, const Y &b) {
        return a.finite () ? a.Value.template get<X> () <=> static_cast<X> (b) :
            a.Value.template get<bool> () ? signed_limit<X>::comparison::greater : signed_limit<X>::comparison::less;
    }

    template <typename X> requires requires {
        typename sign<X>;
    } constexpr signature inline sign<unsigned_limit<X>>::operator () (const unsigned_limit<X> &x) const {
        return x.infinite () ? positive : data::sign (*x.Value);
    }

    template <ordered X> requires requires {
        typename sign<X>;
    } constexpr signature inline sign<signed_limit<X>>::operator () (const signed_limit<X> &x) const {
        return x.finite () ? data::sign (x.Value) : x.positive_infinite () ? positive : negative;
    }

    template <typename X> std::ostream inline &operator << (std::ostream &o, const unsigned_limit<X> &x) {
        if (x.infinite ()) return o << "infinity";
        return o << *x.Value;
    }

    template <ordered X> std::ostream inline &operator << (std::ostream &o, const signed_limit<X> &x) {
        if (x.finite ()) return o << x.Value.template get<X> ();
        if (!x.Value.template get<bool> ()) o << "-";
        return o << "infinity";
    }
}

#endif 

