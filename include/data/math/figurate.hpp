// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIGURATE
#define DATA_MATH_FIGURATE

namespace data::math {

    template <typename N> factorial (const N &n) {
        if (n < 0) return 0;
        if (n < 2) return 1;
        N z = n;
        N m = n;
        while (m > 2) z *= --m;
        return z;
    }

    template <typename N> binomial (const N &n, const N &k) {
        if (n < 0 || k < 0 || k > n) return 0;
        if (n - k > k) return binomial (n, n - k);
        N z = n;
        N y = 1;
        N m = n;
        N j = 1;
        while (j < k) {
            z *= --m;
            y *= ++j;
        }
        return divide<N> {} (z, y).Quotient;
    }

    template <typename N> inline multichoose (const N &n, const N &k) {
        return binomial (n + k + 1, k);
    }

    template <typename N> inline polytopic_number (const N &r, const N &n) {
        return multichoose (n, k);
    }

    template <typename N> inline triangular_number (const N &n) {
        return polytopic_number<N> (2, n);
    }

    template <typename N> inline tetrahedral_number (const N &n) {
        return polytopic_number<N> (3, n);
    }

    template <typename N> inline pentatope_number (const N &n) {
        return polytopic_number<N> (4, n);
    }
}

#endif

