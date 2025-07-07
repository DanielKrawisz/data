// Copyright (c) 2020-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INFINITE
#define DATA_MATH_INFINITE

#include <data/maybe.hpp>
#include <data/either.hpp>
#include <data/ordered.hpp>
#include <data/sign.hpp>
#include <iostream>

namespace data::math {
    // attach infinite values to a type.

    // will have constants called Value or Positive and Negative if defined for the given type. 
    template <typename X> struct infinite;

    // unsigned limit is suitable for adding an infinite value to unsigned numbers
    // as well as to unordered types to represent a point at infinity, such as for
    // the Riemann sphere.
    template <typename X> struct unsigned_limit;

    // signed limit is suitable for types along the real line, such as the integers or rationals.
    // don't use signed_limit with built-in floating points since they already have infinite values. 
    template <Ordered X> struct signed_limit;

    // detect infinite values. 
    template <typename X> constexpr bool is_infinite (const X &);
    constexpr bool inline is_infinite (const float &x);
    constexpr bool inline is_infinite (const double &x);
    constexpr bool inline is_infinite (const long double &x);
    template <typename X> constexpr bool is_infinite (const unsigned_limit<X> &);
    template <typename X> constexpr bool is_infinite (const signed_limit<X> &);

    // equality operators provided for infinite extensions.
    // Infinite values are equal to each other. 
    template <typename X, typename Y>
    requires ImplicitlyConvertible<Y, X>
    constexpr bool operator == (const unsigned_limit<X> &, const unsigned_limit<Y> &);

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool operator == (const signed_limit<X> &, const signed_limit<Y> &);

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool operator == (const unsigned_limit<X> &, const Y &);

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool operator == (const signed_limit<X> &, const Y &);

    // if X is ordered, then an ordering operation exists
    // that sets infinity higher than any finite value.
    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto operator <=> (const unsigned_limit<X> &, const unsigned_limit<Y> &);

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto operator <=> (const signed_limit<X> &, const signed_limit<Y> &);

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto operator <=> (const unsigned_limit<X> &, const Y &);

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto operator <=> (const signed_limit<X> &, const Y &);

    // extend sign to infinite types if the underlying type has a sign value defined for it. 
    // for unsigned_limit, infinite value is assumed positive. 
    template <typename X> requires requires {
        typename sign<X>;
    } struct sign<unsigned_limit<X>> {
        constexpr signature operator () (const unsigned_limit<X> &x) const;
    };

    template <Ordered X> requires requires {
        typename sign<X>;
    } struct sign<signed_limit<X>> {
        constexpr signature operator () (const signed_limit<X> &x) const;
    };

    // Extend << to infinite extensions if the underlying type has << defined on it. 
    template <typename X> std::ostream &operator << (std::ostream &, const unsigned_limit<X> &x);
    template <Ordered X> std::ostream &operator << (std::ostream &, const signed_limit<X> &x);
    
    template <typename X> struct unsigned_limit {

        constexpr unsigned_limit ();
        constexpr unsigned_limit (const X &x);
        constexpr unsigned_limit (X &&x);
        
        constexpr bool infinite () const;
        constexpr bool finite () const;
        
        constexpr static const unsigned_limit &infinity ();
        
        constexpr unsigned_limit operator + (const X &x) const;
        constexpr unsigned_limit operator * (const X &x) const;

        using comparison = decltype (std::declval<X> () <=> std::declval<X> ());

        maybe<X> Value;

    private:
        // constructor for producing the infinite value.
        constexpr unsigned_limit (maybe<X> &&x, int) : Value {x} {}
        friend struct infinite<unsigned_limit<X>>;
    };
    
    template <Ordered X> struct signed_limit {

        constexpr signed_limit ();
        constexpr signed_limit (const X &x);
        constexpr signed_limit (X &&x);

        constexpr static const signed_limit &infinity ();
        constexpr static const signed_limit &negative_infinity ();
        
        constexpr bool infinite () const;
        constexpr bool finite () const;
        constexpr bool positive_infinite () const;
        constexpr bool negative_infinite () const;

        using comparison = decltype (std::declval<X> () <=> std::declval<X> ());

        signed_limit operator - () const;

        either<X, bool> Value;
    private:
        // constructor for producing the infinite value.
        constexpr signed_limit (either<X, bool> &&v, int) : Value {v} {}
        friend struct infinite<signed_limit<X>>;
    };

    // NOTE: these values ought to be declared constexpr. This may be possible in c++23
    template <typename X> struct infinite<unsigned_limit<X>> {
        static const inline unsigned_limit<X> Value {maybe<X> {}, 0};
    };

    template <typename X> struct infinite<signed_limit<X>> {
        static const inline signed_limit<X> Positive {either<X, bool> {true}, 0};
        static const inline signed_limit<X> Negative {either<X, bool> {false}, 0};
    };

    // for types that do not have a max and a min value, we use the infinite types.
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

    constexpr bool inline is_infinite (const float &x) {
        return x == std::numeric_limits<float>::infinity () || x == -std::numeric_limits<float>::infinity ();
    }

    constexpr bool inline is_infinite (const double &x) {
        return x == std::numeric_limits<float>::infinity () || x == -std::numeric_limits<float>::infinity ();
    }

    constexpr bool inline is_infinite (const long double &x) {
        return x == std::numeric_limits<float>::infinity () || x == -std::numeric_limits<float>::infinity ();
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

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool inline operator == (const unsigned_limit<X> &a, const unsigned_limit<Y> &b) {
        return a.Value == static_cast<maybe<X>> (b.Value);
    }

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool inline operator == (const signed_limit<X> &a, const signed_limit<Y> &b) {
        return a.Value == static_cast<either<X, bool>> (b.Value);
    }

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool inline operator == (const unsigned_limit<X> &a, const Y &b) {
        return a.infinite () ? false : *a.Value == static_cast<X> (b);
    }

    template <typename X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr bool inline operator == (const signed_limit<X> &a, const Y &b) {
        return a.finite () ? a.Value.template get<X> () == static_cast<X> (b) : false;
    }

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto inline operator <=> (const unsigned_limit<X> &a, const unsigned_limit<Y> &b) {
        return b.finite () ? a <=> static_cast<X> (*b.Value) : a.infinite () ? X {0} <=> X {0} : unsigned_limit<X>::comparison::less;
    }

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto inline operator <=> (const signed_limit<X> &a, const signed_limit<Y> &b) {
        return b.finite () ? a <=> static_cast<X> (b.Value.template get<Y> ()) :
            a.infinite () ? ((a.Value.template get<bool> () ? X {1} : X {0}) <=> (b.Value.template get<bool> () ? X {1} : X {0})) :
                    b.Value.template get<bool> () ? X {0} <=> X {1} : X {1} <=> X {0};
    }

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto inline operator <=> (const unsigned_limit<X> &a, const Y &b) {
        return a.infinite () ? unsigned_limit<X>::comparison::greater : *a.Value <=> static_cast<X> (b);
    }

    template <Ordered X, typename Y> requires ImplicitlyConvertible<Y, X>
    constexpr auto inline operator <=> (const signed_limit<X> &a, const Y &b) {
        return a.finite () ? a.Value.template get<X> () <=> static_cast<X> (b) :
            a.Value.template get<bool> () ? signed_limit<X>::comparison::greater : signed_limit<X>::comparison::less;
    }

    template <typename X> requires requires {
        typename sign<X>;
    } constexpr signature inline sign<unsigned_limit<X>>::operator () (const unsigned_limit<X> &x) const {
        return x.infinite () ? positive : data::sign (*x.Value);
    }

    template <Ordered X> requires requires {
        typename sign<X>;
    } constexpr signature inline sign<signed_limit<X>>::operator () (const signed_limit<X> &x) const {
        return x.finite () ? data::sign (x.Value) : x.positive_infinite () ? positive : negative;
    }

    template <typename X> std::ostream inline &operator << (std::ostream &o, const unsigned_limit<X> &x) {
        if (x.infinite ()) return o << "infinity";
        return o << *x.Value;
    }

    template <Ordered X> std::ostream inline &operator << (std::ostream &o, const signed_limit<X> &x) {
        if (x.finite ()) return o << x.Value.template get<X> ();
        if (!x.Value.template get<bool> ()) o << "-";
        return o << "infinity";
    }
    
    template <typename X> constexpr unsigned_limit<X>::unsigned_limit (): Value {X {}} {}
    template <typename X> constexpr unsigned_limit<X>::unsigned_limit (const X &x) : Value {x} {}
    template <typename X> constexpr unsigned_limit<X>::unsigned_limit (X &&x) : Value {x} {}
    
    template <typename X> constexpr bool inline unsigned_limit<X>::infinite () const {
        return !bool (Value);
    }

    template <typename X> constexpr bool inline unsigned_limit<X>::finite () const {
        return bool (Value);
    }
    
    template <typename X> constexpr const unsigned_limit<X> inline &unsigned_limit<X>::infinity () {
        return math::infinite<unsigned_limit<X>>::Value;
    }
    
    template <typename X> constexpr unsigned_limit<X> inline unsigned_limit<X>::operator + (const X &x) const {
        if (infinite ()) return infinity ();
        return {*Value + x};
    }
    
    template <typename X> constexpr unsigned_limit<X> inline unsigned_limit<X>::operator * (const X &x) const {
        if (infinite ()) return infinity ();
        return {*Value * x};
    }
    
    template <Ordered X> constexpr signed_limit<X>::signed_limit () : Value {X {}} {}
    template <Ordered X> constexpr signed_limit<X>::signed_limit (const X &x) : Value {x} {}
    template <Ordered X> constexpr signed_limit<X>::signed_limit (X &&x) : Value {x} {}

    template <Ordered X> constexpr const signed_limit<X> &signed_limit<X>::infinity () {
        return math::infinite<signed_limit<X>>::Positive;
    }

    template <Ordered X> constexpr const signed_limit<X> &signed_limit<X>::negative_infinity () {
        return math::infinite<signed_limit<X>>::Negative;
    }
    
    template <Ordered X> constexpr bool signed_limit<X>::infinite () const {
        return Value.template is<bool> ();
    }

    template <Ordered X> constexpr bool signed_limit<X>::finite () const {
        return !Value.template is<bool> ();
    }
    
    template <Ordered X> constexpr bool signed_limit<X>::positive_infinite () const {
        return Value.template is<bool> () && Value.template get<bool> ();
    }
    
    template <Ordered X> constexpr bool signed_limit<X>::negative_infinite () const {
        return Value.template is<bool> () && !Value.template get<bool> ();
    }

    template <Ordered X> signed_limit<X> signed_limit<X>::operator - () const {
        return finite () ? signed_limit {-Value.template get<X> ()} : Value.template get<bool> ? negative_infinity () : infinity ();
    }
}

#endif 

