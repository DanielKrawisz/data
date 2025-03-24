// Copyright (c) 2020-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC
#define DATA_ARITHMETIC

#include <data/types.hpp>

namespace data {

    // basic arithmetic
    template <typename A, typename B> auto plus (const A &, const B &);
    template <typename A, typename B> auto minus (const A &, const B &);
    template <typename A, typename B> auto times (const A &, const B &);
    template <typename A, typename Exp> A pow (const A &, const Exp &);

    // modular arithmetic
    template <typename A, typename Mod> A negate_mod (const A &, const Mod &);
    template <typename A, typename B, typename Mod> A plus_mod (const A &, const B &, const Mod &);
    template <typename A, typename B, typename Mod> A minus_mod (const A &, const B &, const Mod &);
    template <typename A, typename B, typename Mod> A times_mod (const A &, const B &, const Mod &);
    template <typename A, typename Exp, typename Mod> A pow_mod (const A &, const Exp &, const Mod &);

    // bit arithmetic
    template <typename A> auto bit_not (const A &);

    template <typename A, typename B> auto bit_add (const A &, const B &);
    template <typename A, typename B> auto bit_or (const A &, const B &);
    template <typename A, typename B> auto bit_xor (const A &, const B &);
    template <typename A, typename B> auto bit_nand (const A &, const A &);
    template <typename A, typename B> auto bit_nor (const A &, const A &);

    template <typename A> A bit_shift_left (const A &, uint32);
    template <typename A> A bit_shift_right (const A &, uint32);

    template <typename A> A inline bit_shift (const A &x, int32 i) {
        return i < 0 ? bit_shift_right (x, static_cast<uint32> (-i)) : bit_shift_left (x, static_cast<uint32> (i));
    }

    // helper functions needed to implement some of these.

    // count digits in a number.
    template <typename A> size_t digits_base_2 (const A &);

    template <typename A> A mul_2 (const A &);
    template <typename A> A div_2 (const A &);

    template <typename A> A square (const A &);

    template <typename A, typename B> A mul_2_mod (const A &, const B &);

    template <typename A, typename B> A square_mod (const A &, const B &);

}

namespace data::math {
    template <typename A> struct digits_base_2;

    template <typename A> struct mul_2;
    template <typename A> struct div_2;

    template <typename A> struct square;

    template <typename A, typename Mod = A> struct mul_2_mod;
    template <typename A, typename Mod = A> struct square_mod;

    template <typename A, typename B = A> struct plus;
    template <typename A, typename B = A> struct minus;
    template <typename A> struct negate_mod;
    template <typename A, typename B = A, typename Mod = B> struct plus_mod;
    template <typename A, typename B = A, typename Mod = B> struct minus_mod;

    template <typename A> struct bit_not;
    template <typename A, typename B = A> struct bit_and;
    template <typename A, typename B = A> struct bit_or;
    template <typename A, typename B = A> struct bit_xor;
    template <typename A, typename B = A> struct bit_nand;
    template <typename A, typename B = A> struct bit_nor;
    template <typename A> struct bit_shift_left;
    template <typename A> struct bit_shift_right;

    template <typename A, typename B = A> struct times;
    template <typename A, typename B = A, typename Mod = B> struct times_mod;

    template <typename A, typename Exp = A> struct pow;
    template <typename A, typename Exp = A, typename Mod = Exp> struct pow_mod;

    template <typename A, typename B> struct times {
        auto operator () (const A &x, const B &y) const {
            return x * y;
        }
    };
}

namespace data {

    template <typename A> size_t inline digits_base_2 (const A &x) {
        return math::digits_base_2<A> {} (x);
    }

    template <typename A> A inline square (const A &x) {
        return math::square<A> {} (x);
    }

    template <typename A> A inline mul_2 (const A &x) {
        return math::mul_2<A> {} (x);
    }

    template <typename A> A inline div_2 (const A &x) {
        return math::mul_2<A> {} (x);
    }

    template <typename A, typename Mod = A> A inline mul_2_mod (const A &x, const Mod &z) {
        return math::mul_2_mod<A, Mod> {} (x, z);
    }

    template <typename A, typename Mod = A> A inline square_mod (const A &x, const Mod &z) {
        return math::square_mod<A, Mod> {} (x, z);
    }

    template <typename A, typename B> auto inline plus (const A &x, const B &y) {
        return math::plus<A, B> {} (x, y);
    }

    template <typename A, typename B> auto inline minus (const A &x, const B &y) {
        return math::minus<A, B> {} (x, y);
    }

    template <typename A, typename B, typename Mod> A inline plus_mod (const A &x, const B &y, const Mod &n) {
        return math::plus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename B, typename Mod> A inline minus_mod (const A &x, const B &y, const Mod &n) {
        return math::minus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A> auto bit_not (const A &x) {
        return math::bit_not<A> {} (x);
    }

    template <typename A, typename B> A inline bit_and (const A &x, const A &y) {
        return math::bit_and<A, B> {} (x, y);
    }

    template <typename A, typename B> A inline bit_or (const A &x, const A &y) {
        return math::bit_or<A, B> {} (x, y);
    }

    template <typename A, typename B> auto bit_xor (const A &x, const B &y) {
        return math::bit_xor<A, B> {} (x, y);
    }

    template <typename A, typename B> auto bit_nand (const A &x, const A &y) {
        return math::bit_nand<A, B> {} (x, y);
    }

    template <typename A, typename B> auto bit_nor (const A &x, const A &y) {
        return math::bit_nor<A, B> {} (x, y);
    }

    template <typename A> A inline bit_shift_right (const A &x, uint32 i) {
        return math::bit_shift_right<A> {} (x, i);
    }

    template <typename A> A inline bit_shift_left (const A &x, uint32 i) {
        return math::bit_shift_left<A> {} (x, i);
    }

    template <typename A, typename B> auto inline times (const A &x, const B &y) {
        return math::times<A, B> {} (x, y);
    }

    template <typename A, typename Exp, typename Mod> A inline times_mod (const A &x, const Exp &y, const Mod &n) {
        return math::times_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Mod> A inline pow (const A &x, const Mod &y) {
        return math::pow<A, Mod> {} (x, y);
    }

    template <typename A, typename Exp, typename Mod> A inline pow_mod (const A &x, const Exp &y, const Mod &n) {
        return math::pow_mod<A, Exp, Mod> {} (x, y, n);
    }
}


// default behaviors for these functions is to use the operators.
namespace data::math {

    template <typename A, typename B> struct bit_and {
        auto operator () (const A &x, const B &y) const {
            return x & y;
        }
    };

    template <typename A, typename B> struct bit_or {
        auto operator () (const A &x, const B &y) const {
            return x | y;
        }
    };

    template <typename A, typename Mod> struct mul_2_mod {
        A operator () (const A &x, const Mod &mod) const {
            return data::mul_2 (x) % mod;
        }
    };

    template <typename A, typename B, typename N> struct plus_mod {
        auto operator () (const A &x, const B &y, const N &n) const {
            return data::plus (x, y) % n;
        }
    };

    template <typename A, typename B, typename N> struct minus_mod {
        auto operator () (const A &x, const B &y, const N &n) const {
            return data::minus (x, y) % n;
        }
    };

    template <typename A, typename N> struct square_mod {
        auto operator () (const A &x, const N &n) const {
            return data::square (x) % n;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x + y };
    } struct plus<A, B> {
        auto operator () (const A &x, const B &y) const {
            return x + y;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x * y };
    } struct times<A, B> {
        auto operator () (const A &x, const B &y) const {
            return x * y;
        }
    };

    template <typename A> requires requires (const A &x) {
        { x * x };
    } struct square<A> {
        A operator () (const A &x) const {
            return x * x;
        }
    };
}

// default behaviors for these functions is to use the operators.
namespace data::math {
    template <std::unsigned_integral X> struct digits_base_2<X> {
        size_t operator () (X);
    };
}

#endif
