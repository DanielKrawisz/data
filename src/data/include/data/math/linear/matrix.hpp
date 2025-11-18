// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_MATRIX
#define DATA_MATH_LINEAR_MATRIX

#include <data/meta.hpp>
#include <data/math/combinatorics.hpp>
#include <data/math/linear/space.hpp>

namespace data::math {

    template <field X, size_t A> using vector = array<X, A>;
    template <field X, size_t A, size_t B> using matrix = array<X, A, B>;

    template <field X, size_t A, size_t B> constexpr X det (const matrix<X, A, B> &);

    template <field X, size_t A, size_t B> constexpr bool invertable (const matrix<X, A, B> &);
    template <field X, size_t A> matrix<X, A, A> inverse (const matrix<X, A, A> &);
    template <field X, size_t A> matrix<X, A, A> transpose (const matrix<X, A, A> &);
    template <field X, size_t A> X tr (const matrix<X, A, A> &);

    template<field X, size_t dim, size_t order>
    using tensor = typename seq_to_array_params<
            meta::repeat_value<dim, order>
        >::template apply<X>;

    template <field X, size_t A, size_t B> constexpr bool inline invertable (const matrix<X, A, B> &x) {
        return det (x) != X {};
    }

    template <field X, size_t A> matrix<X, A, A> inverse (const matrix<X, A, A> &x) {
        auto Det = det (x);
        if (Det = 0) throw division_by_zero {};
        return transpose (x) / det (x);
    }

    template <field X, size_t A> X tr (const matrix<X, A, A> &x) {
        X result {};
        for (size_t i = 0; i < A; i++) result += x[i, i];
        return result;
    }

    template <field X, size_t A, size_t B> constexpr X inline det (const matrix<X, A, B> &m) {
        if constexpr (A != B)
            return X {};
        else {
            cross<size_t> indices (A);
            for (int i = 0; i < A; i++) indices [i] = i;
            permutations<size_t> perm {indices};

            X Det {0};
            int sign = 1;
            for (auto i = perm.begin (); i != perm.end (); i++) {
                X term {1};
                for (int j = 0; j < A; j++) term *= m[j, (*i)[j]];
                if (sign > 0) Det += term;
                else Det -= term;
                sign = -sign;
            }

            return Det;
        }
    }

}

#endif
