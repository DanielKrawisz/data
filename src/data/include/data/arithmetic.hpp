// Copyright (c) 2020-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC
#define DATA_ARITHMETIC

#include <data/types.hpp>
#include <data/maybe.hpp>
#include <data/math/nonnegative.hpp>

namespace data {

    // basic arithmetic
    template <typename A> constexpr bool even (const A &);
    template <typename A> constexpr bool odd (const A &);

    template <typename A, typename B> constexpr auto plus (const A &, const B &);
    template <typename A, typename B> constexpr auto minus (const A &, const B &);
    template <typename A, typename B> constexpr auto times (const A &, const B &);
    template <typename A, typename Exp> constexpr A pow (const A &, const Exp &);

    template <typename A> auto constexpr square (const A &);

    template <typename dividend, typename divisor>
    constexpr auto divide (const dividend &a, const math::nonzero<divisor> &b);

    template <typename dividend, typename divisor>
    constexpr bool divides (const dividend &a, const math::nonzero<divisor> &b);

    // modular arithmetic
    template <typename A, typename Mod = A> constexpr auto mod (const A &, const math::nonzero<Mod> &);

    template <typename A, typename Mod = A> constexpr auto negate_mod (const A &, const math::nonzero<Mod> &);

    template <typename A, typename B, typename Mod> constexpr auto plus_mod (const A &, const B &, const math::nonzero<Mod> &);
    template <typename A, typename B, typename Mod> constexpr auto minus_mod (const A &, const B &, const math::nonzero<Mod> &);

    template <typename A, typename B, typename Mod> constexpr auto times_mod (const A &, const B &, const math::nonzero<Mod> &);
    template <typename A, typename Exp = A, typename Mod = Exp> constexpr auto pow_mod (const A &, const Exp &, const math::nonzero<Mod> &);

    template <typename A, typename Mod> constexpr auto invert_mod (const A &x, const math::nonzero<Mod> &n);

    // helper functions for computing pow mod and times mod.
    template <typename A, typename Mod = A> constexpr auto mul_2_mod (const A &, const math::nonzero<Mod> &);
    template <typename A, typename Mod = A> constexpr auto square_mod (const A &, const math::nonzero<Mod> &);

    template <typename A> constexpr auto mod_2 (const A &);
    template <typename A> constexpr auto mul_2 (const A &);
    template <typename A> constexpr auto div_2 (const A &);

    template <typename A> constexpr bool inline even (const A &x) {
        return mod_2 (x) == 0;
    }

    template <typename A> constexpr bool inline odd (const A &x) {
        return mod_2 (x) == 1;
    }

    template <typename dividend, typename divisor>
    constexpr bool inline divides (const dividend &a, const math::nonzero<divisor> &b) {
        return divide (a, b) == 0;
    }

    // bit arithmetic
    template <typename A> constexpr auto bit_not (const A &);

    template <typename A, typename B> constexpr auto bit_add (const A &, const B &);
    template <typename A, typename B> constexpr auto bit_or (const A &, const B &);
    template <typename A, typename B> constexpr auto bit_xor (const A &, const B &);
    template <typename A, typename B> constexpr auto bit_nand (const A &, const A &);
    template <typename A, typename B> constexpr auto bit_nor (const A &, const A &);

    template <typename A> constexpr A bit_shift_left (const A &, uint32);
    template <typename A> constexpr A bit_shift_right (const A &, uint32);

    template <typename A> constexpr A inline bit_shift (const A &x, int32 i) {
        return i < 0 ? bit_shift_right (x, static_cast<uint32> (-i)) : bit_shift_left (x, static_cast<uint32> (i));
    }

    // may not exist
    template <typename A, typename Mod = A> constexpr auto invert_mod (const A &, const math::nonzero<Mod> &);

    // count digits in a number.
    template <typename A> size_t constexpr digits_base_2 (const A &);

}

namespace data::math::def {
    template <typename A> struct digits_base_2;

    template <typename A> struct mul_2;
    template <typename A> struct div_2;
    template <typename A> struct mod_2;

    template <typename A> struct square;

    template <typename A, typename B = A> struct plus;
    template <typename A, typename B = A> struct minus;

    template <typename A> struct bit_not;
    template <typename A, typename B = A> struct bit_and;
    template <typename A, typename B = A> struct bit_or;
    template <typename A, typename B = A> struct bit_xor;
    template <typename A, typename B = A> struct bit_nand;
    template <typename A, typename B = A> struct bit_nor;
    template <typename A> struct bit_shift_left;
    template <typename A> struct bit_shift_right;

    template <typename A, typename B = A> struct times;
    template <typename A, typename Exp = A> struct pow;

    template <typename A, typename B> struct times {
        constexpr auto operator () (const A &x, const B &y) const {
            return x * y;
        }
    };

    template <typename A, typename B = A> struct divide;

    template <typename A, typename Mod = A> struct mod;
    template <typename A, typename Mod = A> struct negate_mod;

    template <typename A, typename Mod = A> struct invert_mod;
    template <typename A, typename B = A, typename Mod = B> struct plus_mod;
    template <typename A, typename B = A, typename Mod = B> struct minus_mod;

    template <typename A, typename B = A, typename Mod = B> struct times_mod;
    template <typename A, typename Exp = A, typename Mod = Exp> struct pow_mod;

    template <typename A, typename Mod = A> struct mul_2_mod;
    template <typename A, typename Mod = A> struct square_mod;
}

namespace data {

    template <typename A> constexpr size_t inline digits_base_2 (const A &x) {
        return math::def::digits_base_2<A> {} (x);
    }

    template <typename A> constexpr auto inline square (const A &x) {
        return math::def::square<A> {} (x);
    }

    template <typename A> constexpr auto inline mul_2 (const A &x) {
        return math::def::mul_2<A> {} (x);
    }

    template <typename A> constexpr auto inline div_2 (const A &x) {
        return math::def::div_2<A> {} (x);
    }

    template <typename A> constexpr auto inline mod_2 (const A &x) {
        return math::def::mod_2<A> {} (x);
    }

    template <typename A, typename B> constexpr auto inline plus (const A &x, const B &y) {
        return math::def::plus<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto inline minus (const A &x, const B &y) {
        return math::def::minus<A, B> {} (x, y);
    }

    template <typename A> constexpr auto bit_not (const A &x) {
        return math::def::bit_not<A> {} (x);
    }

    template <typename A, typename B> constexpr auto inline bit_and (const A &x, const B &y) {
        return math::def::bit_and<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto inline bit_or (const A &x, const B &y) {
        return math::def::bit_or<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto bit_xor (const A &x, const B &y) {
        return math::def::bit_xor<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto bit_nand (const A &x, const A &y) {
        return math::def::bit_nand<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto bit_nor (const A &x, const A &y) {
        return math::def::bit_nor<A, B> {} (x, y);
    }

    template <typename A> constexpr A inline bit_shift_right (const A &x, uint32 i) {
        return math::def::bit_shift_right<A> {} (x, i);
    }

    template <typename A> constexpr A inline bit_shift_left (const A &x, uint32 i) {
        return math::def::bit_shift_left<A> {} (x, i);
    }

    template <typename A, typename B> constexpr auto inline times (const A &x, const B &y) {
        return math::def::times<A, B> {} (x, y);
    }

    template <typename A, typename Mod> constexpr A inline pow (const A &x, const Mod &y) {
        return math::def::pow<A, Mod> {} (x, y);
    }

    template <typename A, typename Mod> constexpr auto mod (const A &x, const math::nonzero<Mod> &n) {
        return math::def::mod<A, Mod> {} (x, n);
    }

    template <typename A, typename Mod> constexpr auto negate_mod (const A &x, const math::nonzero<Mod> &n) {
        return math::def::negate_mod<A, Mod> {} (x, n);
    }

    template <typename A, typename B, typename Mod> constexpr auto inline plus_mod (const A &x, const B &y, const math::nonzero<Mod> &n) {
        return math::def::plus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename B, typename Mod> constexpr auto inline minus_mod (const A &x, const B &y, const math::nonzero<Mod> &n) {
        return math::def::minus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename Exp, typename Mod> constexpr auto inline times_mod (const A &x, const Exp &y, const math::nonzero<Mod> &n) {
        return math::def::times_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Exp, typename Mod> constexpr auto inline pow_mod (const A &x, const Exp &y, const math::nonzero<Mod> &n) {
        return math::def::pow_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Mod> constexpr auto invert_mod (const A &x, const math::nonzero<Mod> &n) {
        return math::def::invert_mod<A, Mod> {} (x, n);
    }

    template <typename A, typename Mod> constexpr auto inline mul_2_mod (const A &x, const math::nonzero<Mod> &z) {
        return math::def::mul_2_mod<A, Mod> {} (x, z);
    }

    template <typename A, typename Mod> constexpr auto inline square_mod (const A &x, const math::nonzero<Mod> &z) {
        return math::def::square_mod<A, Mod> {} (x, z);
    }
}


// default behaviors for these functions is to use the operators.
namespace data::math::def {

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x + y };
    } struct plus<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x + y;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x * y };
    } struct times<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x * y;
        }
    };

    template <typename A> requires requires (const A &x) {
        { x * x };
    } struct square<A> {
        constexpr A operator () (const A &x) const {
            return x * x;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x & y };
    } struct bit_and<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x & y;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x | y };
    } struct bit_or<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x | y;
        }
    };

    // we don't have a similar definition for bit xor because sometimes
    // people use ^ for power instead.

    template <std::unsigned_integral X> struct digits_base_2<X> {
        constexpr size_t operator () (X);
    };

    template <std::integral X> struct times<X, X> {
        constexpr X operator () (X x, X y) {
            return x * y;
        }

        constexpr math::nonzero<X> operator () (math::nonzero<X> x, math::nonzero<X> y) {
            return x.Value * y.Value;
        }
    };

    template <std::integral X> struct bit_not<X> {
        constexpr auto operator () (X n) {
            return static_cast<X> (~n);
        }
    };

    template <std::integral X> struct bit_xor<X> {
        constexpr auto operator () (X a, X b) {
            return static_cast<X> (a ^ b);
        }
    };
}

#endif
