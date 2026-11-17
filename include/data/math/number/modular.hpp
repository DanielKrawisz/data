// Copyright (c) 2019-202 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/types.hpp>
#include <data/arithmetic.hpp>
#include <data/math/algebra.hpp>
#include <data/math/number/extended_euclidian.hpp>

namespace data::math::number {
    template <typename X> concept mod_base = RingNumber<X>;

    // TODO modular must satisfy group_integral
    template <auto mod, mod_base X = decltype (mod)> struct modular;
    
    template <auto mod, mod_base X>
    constexpr bool operator == (const modular<mod, X> &, const modular<mod, X> &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr bool operator == (const modular<mod, X> &, const Y &);
/*
    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr bool operator == (const Y &, const modular<mod, X> &);
*/
    template <auto mod, mod_base X>
    modular<mod, X> &operator ++ (modular<mod, X> &);

    template <auto mod, mod_base X>
    modular<mod, X> &operator -- (modular<mod, X> &);

    template <auto mod, mod_base X>
    modular<mod, X> operator ++ (modular<mod, X> &, int);

    template <auto mod, mod_base X>
    modular<mod, X> operator -- (modular<mod, X> &, int);
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> operator + (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> operator - (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> operator - (const modular<mod, X> &);
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> operator * (const modular<mod, X> &, const modular<mod, X> &);

    template <auto mod, mod_base X>
    constexpr modular<mod, X> &operator += (modular<mod, X> &, const modular<mod, X> &);

    template <auto mod, mod_base X>
    constexpr modular<mod, X> &operator -= (modular<mod, X> &, const modular<mod, X> &);

    template <auto mod, mod_base X>
    constexpr modular<mod, X> &operator *= (modular<mod, X> &, const modular<mod, X> &);

    template <auto mod, mod_base X>
    constexpr modular<mod, X> operator ^ (const modular<mod, X> &, const modular<mod, X> &);

    template <auto mod, mod_base X>
    constexpr maybe<modular<mod, X>> invert (const modular<mod, X> &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> operator + (const modular<mod, X> &, const Y &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> operator - (const modular<mod, X> &, const Y &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> operator * (const modular<mod, X> &, const Y &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> operator + (const Y &, const modular<mod, X> &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> operator - (const Y &, const modular<mod, X> &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> operator * (const Y &, const modular<mod, X> &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> &operator += (modular<mod, X> &, const Y &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> &operator -= (modular<mod, X> &, const Y &);

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> &operator *= (modular<mod, X> &, const Y &);

    template <auto mod, mod_base X>
    constexpr modular<mod, X> operator ^ (const modular<mod, X> &, const X &);

    template <auto mod, mod_base X> requires requires (X x) {
        { x >> 1 } -> Same<X>;
    } constexpr modular<mod, X> operator >> (const modular<mod, X> &, int);

    template <auto mod, mod_base X> requires requires (X &x) {
        { x >>= 1 } -> Same<X &>;
    } constexpr modular<mod, X> &operator >>= (modular<mod, X> &, int);

    template <auto mod, mod_base X> requires requires (X x) {
        { x << 1 } -> Same<X>;
    } constexpr modular<mod, X> operator << (const modular<mod, X> &, int);

    template <auto mod, mod_base X> requires requires (X &x) {
        { x <<= 1 } -> Same<X &>;
    } constexpr modular<mod, X> &operator <<= (modular<mod, X> &, int);

    template <auto mod, mod_base X> requires requires (std::ostream &o, X x) {
        { o << x } -> Same<std::ostream &>;
    } std::ostream &operator << (std::ostream &o, const modular<mod, X> &m);
    
    template <auto mod, mod_base X> struct modular {
        constexpr static const decltype (mod) Modulus = mod;
        static const X &modulus () {
            static X m = convert<X> (Modulus);
            return m;
        }
        X Value;

        template <typename... P>
        requires requires (P... p) {
            { X (p...) };
        } constexpr modular (P... p): Value (p...) {
            if consteval {
                Value %= convert<X> (mod);
            } else {
                Value %= modulus ();
            }
        }

        constexpr modular (const X &v): Value (v) {
            if consteval {
                Value %= convert<X> (mod);
            } else {
                Value %= modulus ();
            }
        }
        
        constexpr bool valid () const {
            if consteval {
                return data::valid (Value) && Value >= 0 && Value < convert<X> (mod);
            } else {
                return data::valid (Value) && Value >= 0 && Value < modulus ();
            }
        }

        constexpr operator X () const {
            return Value;
        }
        
    };

    template <auto mod, mod_base X> struct increment<modular<mod, X>> {
        constexpr modular<mod, X> operator () (const modular<mod, X> &);
    };

    template <auto mod, mod_base X> struct decrement<modular<mod, X>> {
        constexpr modular<mod, X> operator () (const modular<mod, X> &);
    };
    
}

namespace data::math {

    template <auto mod, typename X>
    struct numeric_limits<number::modular<mod, X>> {
        constexpr static const number::modular<mod, X> Max {convert<X> (mod) - 1};
        constexpr static const number::modular<mod, X> Min {0};

        constexpr static const number::modular<mod, X> &max () {
            return Max;
        }

        constexpr static const number::modular<mod, X> &min () {
            return Min;
        }
    };
}

namespace data::math::def {
    
    template <auto mod, typename X>
    struct identity<plus<number::modular<mod, X>>, number::modular<mod, X>>
        : identity<plus<X>, X> {
        constexpr number::modular<mod, X> operator () () {
            return {identity<plus<X>, X>::value ()};
        }
    };
    
    template <auto mod, typename X>
    struct inverse<plus<number::modular<mod, X>>, number::modular<mod, X>> {
        constexpr number::modular<mod, X> operator () (const number::modular<mod, X> &a, const number::modular<mod, X> &b) {
            return minus_mod (b.Value, a.Value, nonzero {mod});
        }
    };

    template <auto mod, typename X>
    struct mul_2_pow<number::modular<mod, X>> : mul_2_pow<X> {
        constexpr number::modular<mod, X> operator () (const number::modular<mod, X> &a, uint32 pow) {
            return a << int (pow);
        }
    };

    template <auto mod, typename X>
    struct div_2_pow<number::modular<mod, X>> : div_2_pow<X> {
        constexpr number::modular<mod, X> operator () (const number::modular<mod, X> &a, uint32 pow) {
            return a >> int (pow);
        }
    };

    template <auto mod, typename X, Exponent Exp>
    struct pow<number::modular<mod, X>, Exp> {
        constexpr number::modular<mod, X> operator () (const number::modular<mod, X> &base, const Exp &exp) {
            if consteval {
                return data::pow_mod (base.Value, exp, nonzero {math::convert<X> (mod)});
            } else {
                return data::pow_mod (base.Value, exp, nonzero {number::modular<mod, X>::modulus ()});
            }
        }
    };
    
}

namespace data::math::number {

    template <auto mod, mod_base X> requires requires (std::ostream &o, X x) {
        { o << x } -> Same<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const modular<mod, X> &m) {
        return o << m.Value;
    }
    
    template <auto mod, mod_base X>
    constexpr bool inline operator == (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value == b.Value;
    }

    template <auto mod, mod_base X>
    constexpr auto inline operator <=> (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value <=> b.Value;
    }
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline operator + (const modular<mod, X> &a, const modular<mod, X> &b) {
        if consteval {
            return data::plus_mod (a.Value, b.Value, nonzero {convert<X> (mod)});
        } else {
            return data::plus_mod (a.Value, b.Value, nonzero {modular<mod, X>::modulus ()});
        }
    }
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline operator * (const modular<mod, X> &a, const modular<mod, X> &b) {
        if consteval {
            return data::times_mod (a.Value, b.Value, nonzero {convert<X> (mod)});
        } else {
            return data::times_mod (a.Value, b.Value, nonzero {modular<mod, X>::modulus ()});
        }
    }
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline operator - (const modular<mod, X> &a, const modular<mod, X> &b) {
        if consteval {
            if (a.Value < b.Value) return convert<X> (mod) - (b.Value - a.Value);
            return data::minus_mod (a.Value, b.Value, nonzero {convert<X> (mod)});
        } else {
            if (a.Value < b.Value) return modular<mod, X>::modulus () - (b.Value - a.Value);
            return data::minus_mod (a.Value, b.Value, nonzero {modular<mod, X>::modulus ()});
        }
    }
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline operator - (const modular<mod, X> &a) {
        if consteval {
            return data::negate_mod (a.Value, nonzero {convert<X> (mod)});
        } else {
            return data::negate_mod (a.Value, nonzero {modular<mod, X>::modulus ()});
        }
    }
    
    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline operator ^ (const modular<mod, X> &a, const X &b) {
        if consteval {
            return data::pow_mod (a.Value, b, nonzero {convert<X> (mod)});
        } else {
            return data::pow_mod (a.Value, b, nonzero {modular<mod, X>::modulus ()});
        }
    }

    template <auto mod, mod_base X>
    constexpr maybe<modular<mod, X>> invert (const modular<mod, X> &x) {
        //auto proof = euclidian::extended<X, decltype (-x.Value)>::algorithm (mod, x.Value);
        auto proof = euclidian::extended<X, modular<mod, X>>::algorithm (mod, x.Value);
        if (proof.GCD != 1) return {};
        return proof.BezoutT;
    }

    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline increment<modular<mod, X>>::operator () (const modular<mod, X> &x) {
        return x + modular<mod, X> {1};
    }

    template <auto mod, mod_base X>
    constexpr modular<mod, X> inline decrement<modular<mod, X>>::operator () (const modular<mod, X> &x) {
        return x - modular<mod, X> {1};
    }

    template <auto mod, mod_base X, ImplicitlyConvertible<X> Y>
    constexpr modular<mod, X> inline operator * (const modular<mod, X> &a, const Y &b) {
        return a * modular<mod, X> {b};
    }

    template <auto mod, mod_base X = decltype (mod), ImplicitlyConvertible<X> Y>
    constexpr bool operator == (const modular<mod, X> &a, const Y &b) {
        return a == modular<mod, X> {X (b)};
    }

    template <auto mod, mod_base X> requires requires (X x) {
        { x >> 1 } -> Same<X>;
    } constexpr modular<mod, X> inline operator >> (const modular<mod, X> &x, int i) {
        if consteval {
            return data::mod (X (x.Value >> i), nonzero {convert<X> (mod)});
        } else {
            return data::mod (X (x.Value >> i), nonzero {modular<mod, X>::modulus ()});
        }
    }

    template <auto mod, mod_base X> requires requires (X &x) {
        { x >>= 1 } -> Same<X &>;
    } constexpr modular<mod, X> &operator >>= (modular<mod, X> &x, int i) {
        return x = x >> i;
    }

    template <auto mod, mod_base X> requires requires (X x) {
        { x << 1 } -> Same<X>;
    } constexpr modular<mod, X> operator << (const modular<mod, X> &x, int i) {
        if consteval {
            return data::mod (X (x.Value << i), nonzero {convert<X> (mod)});
        } else {
            return data::mod (X (x.Value << i), nonzero {modular<mod, X>::modulus ()});
        }
    }

    template <auto mod, mod_base X> requires requires (X &x) {
        { x <<= 1 } -> Same<X &>;
    } constexpr modular<mod, X> &operator <<= (modular<mod, X> &x, int i) {
        return x = x << i;
    }

}

