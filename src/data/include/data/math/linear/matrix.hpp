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
    template <field X, size_t A> matrix<X, A, A> invert (const matrix<X, A, A> &);
    template <field X, size_t A, size_t B> matrix<X, B, A> transpose (const matrix<X, A, B> &);
    template <field X, size_t A> X tr (const matrix<X, A, A> &);

    template <field X, size_t A> matrix<X, A, A> identity ();

    template<field X, size_t dim, size_t order>
    using tensor = typename seq_to_array_params<
            meta::repeat_value<dim, order>
        >::template apply<X>;

    template <field X, size_t A, size_t B> constexpr bool inline invertable (const matrix<X, A, B> &x) {
        return det (x) != X {};
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

    template <field X, size_t A> matrix<X, A, A> identity () {
        matrix<X, A, A> I {};
        for (int i = 0; i < A; i++) I[i, i] = X {1};
        return I;
    }

    template <field X, size_t A, size_t B> matrix<X, B, A> inline transpose (const matrix<X, A, B> &x) {
        matrix<X, B, A> result;
        for (int i = 0; i < A; i++) for (int j = 0; j < B; j++) result[j, i] = x[i, j];
        return result;
    }

    template <field X, size_t N> matrix<X, N, N> invert (const matrix<X, N, N> &A) {
        matrix<X, N, N> I = identity<X, N> ();

        matrix<X, N, N> M = I;
        matrix<X, N, N> AM = A;
        // we calculate successive coefficients of the characteristic polynomial here.
        // the last one will be the det.
        X c = -tr (A);

        for (size_t k = 2; k <= N; k++) {
            M = AM + I * c;
            AM = A * M;
            c = -tr (AM) / k;
        }

        if (c == 0) throw division_by_zero {};

        return -M / c;
    }

}

#endif
