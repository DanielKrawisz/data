// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_MATRIX
#define DATA_MATH_LINEAR_MATRIX

#include <data/math/linear/space.hpp>

namespace data::math::linear {

    template <field X, size_t A, size_t B> using matrix = array<X, A, B>;

    template <field X, size_t A, size_t B> X det (const matrix<X, A, B> &);
    template <field X, size_t A> X det (const matrix<X, A, A> &);

    template <field X, size_t A, size_t B> bool invertable (const matrix<X, A, B> &);
    template <field X, size_t A> bool invertable (const matrix<X, A, A> &);
    template <field X, size_t A> matrix<X, A, A> inverse (const matrix<X, A, A> &);

}

#endif
