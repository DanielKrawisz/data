// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIGURATE
#define DATA_MATH_FIGURATE

#include <data/arithmetic.hpp>

namespace data::math {

    struct negative_factorial : exception {
        negative_factorial () : exception {"factorial of zero is undefined"} {}
    };

    // TODO tighten these constraints
    template <WholeNumber N> constexpr nonzero<N> factorial (const N &n) {
        if (n < 0) throw negative_factorial {};
        if (n < 2) return nonzero<N> {1};
        N z = n;
        N m = n;
        while (m > 2) z *= --m;
        return nonzero<N> {z};
    }

    template <WholeNumber N> constexpr N binomial (const N &n, const N &k) {
        if (n < 0 || k < 0 || k > n) throw negative_factorial {};
        if (n < 2 || k == 0) return 1;
        N m = n - k;
        if (m < k) return binomial (n, m);
        N z = 1;
        while (m < n) z *= ++m;
        return divide (z, factorial<N> (k));
    }

    template <WholeNumber N> constexpr N inline multichoose (const N &n, const N &r) {
        return binomial (n + r - 1, r);
    }

    template <WholeNumber N> constexpr N inline polytopic_number (const N &r, const N &n) {
        if (n == 0) return 0;
        return multichoose (n, r);
    }

    template <WholeNumber N> constexpr N inline triangular_number (const N &n) {
        return polytopic_number<N> (2, n);
    }

    template <WholeNumber N> constexpr N inline tetrahedral_number (const N &n) {
        return polytopic_number<N> (3, n);
    }

    template <WholeNumber N> constexpr N inline pentatope_number (const N &n) {
        return polytopic_number<N> (4, n);
    }
}

#endif

