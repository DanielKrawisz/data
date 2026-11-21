// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/arithmetic.hpp>

namespace data::math {

    // TODO tighten these constraints
    template <Number N> constexpr nonzero<N> factorial (const N &n);
    template <Number N> constexpr nonzero<N> rising_power (const N &n, const N &);
    template <Number N> constexpr nonzero<N> falling_power (const N &n, const N &);

    template <Number N> constexpr N binomial (const N &n, const N &k);
    template <Number N> constexpr N inline multichoose (const N &n, const N &r);

    template <Number N> constexpr N inline polytopic_number (const N &r, const N &n);
    template <Number N> constexpr N inline triangular_number (const N &n);
    template <Number N> constexpr N inline tetrahedral_number (const N &n);
    template <Number N> constexpr N inline pentatope_number (const N &n);

    template <Number N> constexpr N Sterling (const N &n, const N &k);

    struct negative_factorial : exception {
        negative_factorial () : exception {"factorial of zero is undefined"} {}
    };

    template <Number N> constexpr nonzero<N> factorial (const N &n) {
        if (n < 0) throw negative_factorial {};
        if (n < 2) return nonzero<N> {1};
        N z = n;
        N m = n;
        while (m > 2) z *= --m;
        return nonzero<N> {z};
    }

    template <Number N> constexpr N binomial (const N &n, const N &k) {
        if (n < 0 || k < 0 || k > n) throw negative_factorial {};
        if (n < 2 || k == 0) return 1;
        N m = n - k;
        if (m < k) return binomial (n, m);
        N z = 1;
        while (m < n) z *= ++m;
        return divide (z, factorial<N> (k));
    }

    template <Number N> constexpr N inline multichoose (const N &n, const N &r) {
        return binomial (n + r - 1, r);
    }

    template <Number N> constexpr N inline polytopic_number (const N &r, const N &n) {
        if (n == 0) return 0;
        return multichoose (n, r);
    }

    template <Number N> constexpr N inline triangular_number (const N &n) {
        return polytopic_number<N> (2, n);
    }

    template <Number N> constexpr N inline tetrahedral_number (const N &n) {
        return polytopic_number<N> (3, n);
    }

    template <Number N> constexpr N inline pentatope_number (const N &n) {
        return polytopic_number<N> (4, n);
    }
}

