// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH
#define DATA_MATH

#include <data/numbers.hpp>
#include <data/complex.hpp>

#include <data/math/number/modular.hpp>

#include <data/math/permutation.hpp>
#include <data/math/polynomial.hpp>

#include <data/math/fraction.hpp>
#include <data/math/octonion.hpp>

#include <data/math/root.hpp>
#include <data/math/number/bytes/sqrt.hpp>

namespace data::math {

    template <uint64 pow>
    struct root<dec_uint, pow> {
        set<dec_uint> operator () (const dec_uint &n);
    };

    template <uint64 pow>
    struct root<dec_int, pow> {
        set<dec_int> operator () (const dec_int &n);
    };

    template <hex_case zz, uint64 pow>
    struct root<hex::uint<zz>, pow> {
        set<hex::uint<zz>> operator () (const hex::uint<zz> &n);
    };

    template <hex_case zz, uint64 pow>
    struct root<hex::int2<zz>, pow> {
        set<hex::int2<zz>> operator () (const hex::int2<zz> &n);
    };

    template <hex_case zz, uint64 pow>
    struct root<hex::intBC<zz>, pow> {
        set<hex::intBC<zz>> operator () (const hex::intBC<zz> &n);
    };

    template <uint64 pow> struct root<base58_uint, pow> {
        set<base58_uint> operator () (const base58_uint &n);
    };

}

namespace data {
    // modular numbers
    template <auto N> using modular = math::number::modular<N>;
    
    // fractions
    template <typename Z> using fraction = math::fraction<Z>;
    
    // Polynomials 
    template <typename X, typename N, char x = 'x'> using polynomial = math::polynomial<X, N, x>;

    template <Ordered X> using permutation = math::permutation<X>;

    // rational numbers.
    using Q = fraction<Z>;

    using Q_big = fraction<Z_bytes_big>;
    using Q_little = fraction<Z_bytes_little>;

    using Q_BC_big = fraction<Z_bytes_BC_big>;
    using Q_BC_little = fraction<Z_bytes_BC_little>;

    // Gaussian integers.
    using G = math::complex<Z>;
    using GH = math::quaternion<Z>;
    using GO = math::octonion<Z>;

    // complex rationals
    using QC = math::complex<Q>;

    // rational quaternions
    using QH = math::quaternion<Q>;

    // rational octonions
    using QO = math::octonion<Q>;

    // Caylay-Dickson floating points.
    using complex64 = math::complex<float32>;
    using complex128 = math::complex<float64>;
    using quat128 = math::quaternion<float32>;
    using quat256 = math::quaternion<float64>;
    using oct256 = math::octonion<float32>;
    using oct512 = math::octonion<float64>;
}

namespace data::math {

    // Various kinds of real numbers.
    static_assert (Real<float32> && CompositionAlgebra<float32>);
    static_assert (Real<float64> && CompositionAlgebra<float64>);

    static_assert (Real<int32>);
    static_assert (Real<int32_little>);
    static_assert (Real<int32_big>);

    static_assert (Real<int64>);
    static_assert (Real<int64_little>);
    static_assert (Real<int64_big>);

    static_assert (Real<int80>);
    static_assert (Real<int80_little>);
    static_assert (Real<int80_big>);

    static_assert (Real<int128>);
    static_assert (Real<int128_little>);
    static_assert (Real<int128_big>);

    static_assert (Real<int160>);
    static_assert (Real<int160_little>);
    static_assert (Real<int160_big>);

    static_assert (Real<Z_bytes_little>);
    static_assert (Real<Z_bytes_big>);
    static_assert (Real<Z_bytes_BC_little>);
    static_assert (Real<Z_bytes_BC_big>);

    static_assert (Real<Z>);

    // next we look at complexifications of integers and floating points.
    static_assert (Complex<math::complex<float64>> && CompositionAlgebra<math::complex<float64>>);

    static_assert (Complex<math::complex<int80>>);
    static_assert (Complex<math::complex<int80_little>>);
    static_assert (Complex<math::complex<int80_big>>);

    static_assert (Complex<math::complex<Z_bytes_little>>);
    static_assert (Complex<math::complex<Z_bytes_big>>);
    static_assert (Complex<math::complex<Z_bytes_BC_little>>);
    static_assert (Complex<math::complex<Z_bytes_BC_big>>);

    static_assert (Complex<math::complex<Z>>);

    static_assert (Quaternionic<math::quaternion<float64>> && CompositionAlgebra<math::quaternion<float64>>);

    static_assert (Quaternionic<math::quaternion<int80>>);
    static_assert (Quaternionic<math::quaternion<int80_little>>);
    static_assert (Quaternionic<math::quaternion<int80_big>>);

    static_assert (Quaternionic<math::quaternion<Z_bytes_little>>);
    static_assert (Quaternionic<math::quaternion<Z_bytes_big>>);
    static_assert (Quaternionic<math::quaternion<Z_bytes_BC_little>>);
    static_assert (Quaternionic<math::quaternion<Z_bytes_BC_big>>);

    static_assert (Quaternionic<math::quaternion<Z>>);

    static_assert (Octonionic<math::octonion<float64>> &&
        CompositionAlgebra<math::octonion<float64>>);

    static_assert (Octonionic<math::octonion<int80>>);
    static_assert (Octonionic<math::octonion<int80_little>>);
    static_assert (Octonionic<math::octonion<int80_big>>);

    static_assert (Octonionic<math::octonion<Z_bytes_little>>);
    static_assert (Octonionic<math::octonion<Z_bytes_big>>);
    static_assert (Octonionic<math::octonion<Z_bytes_BC_little>>);
    static_assert (Octonionic<math::octonion<Z_bytes_BC_big>>);

    static_assert (Octonionic<math::octonion<Z>>);

    // next we look at rational types constructed from integer types.

    static_assert (Real<fraction<Z_bytes_little>> &&
        CompositionAlgebra<fraction<Z_bytes_little>>);
    static_assert (Real<fraction<Z_bytes_big>> &&
        CompositionAlgebra<fraction<Z_bytes_big>>);
    static_assert (Real<fraction<Z_bytes_BC_little>> &&
        CompositionAlgebra<fraction<Z_bytes_BC_little>>);
    static_assert (Real<fraction<Z_bytes_BC_big>> &&
        CompositionAlgebra<fraction<Z_bytes_BC_big>>);

    static_assert (Real<fraction<Z>> && CompositionAlgebra<fraction<Z>>);

    static_assert (Complex<math::complex<fraction<Z_bytes_little>>> &&
        CompositionAlgebra<math::complex<fraction<Z_bytes_little>>>);
    static_assert (Complex<math::complex<fraction<Z_bytes_big>>> &&
        CompositionAlgebra<math::complex<fraction<Z_bytes_big>>>);
    static_assert (Complex<math::complex<fraction<Z_bytes_BC_little>>> &&
        CompositionAlgebra<math::complex<fraction<Z_bytes_BC_little>>>);
    static_assert (Complex<math::complex<fraction<Z_bytes_BC_big>>> &&
        CompositionAlgebra<math::complex<fraction<Z_bytes_BC_big>>>);

    static_assert (Complex<math::complex<fraction<Z>>> &&
        CompositionAlgebra<math::complex<fraction<Z>>>);

    static_assert (Quaternionic<math::quaternion<fraction<Z_bytes_little>>> &&
        CompositionAlgebra<math::quaternion<fraction<Z_bytes_little>>>);
    static_assert (Quaternionic<math::quaternion<fraction<Z_bytes_big>>> &&
        CompositionAlgebra<math::quaternion<fraction<Z_bytes_big>>>);
    static_assert (Quaternionic<math::quaternion<fraction<Z_bytes_BC_little>>> &&
        CompositionAlgebra<math::quaternion<fraction<Z_bytes_BC_little>>>);
    static_assert (Quaternionic<math::quaternion<fraction<Z_bytes_BC_big>>> &&
        CompositionAlgebra<math::quaternion<fraction<Z_bytes_BC_big>>>);

    static_assert (Quaternionic<math::quaternion<fraction<Z>>> &&
        CompositionAlgebra<math::quaternion<fraction<Z>>>);

    static_assert (Octonionic<math::octonion<fraction<Z_bytes_little>>> &&
        CompositionAlgebra<math::octonion<fraction<Z_bytes_little>>>);
    static_assert (Octonionic<math::octonion<fraction<Z_bytes_big>>> &&
        CompositionAlgebra<math::octonion<fraction<Z_bytes_big>>>);
    static_assert (Octonionic<math::octonion<fraction<Z_bytes_BC_little>>> &&
        CompositionAlgebra<math::octonion<fraction<Z_bytes_BC_little>>>);
    static_assert (Octonionic<math::octonion<fraction<Z_bytes_BC_big>>> &&
        CompositionAlgebra<math::octonion<fraction<Z_bytes_BC_big>>>);

    static_assert (Octonionic<math::octonion<fraction<Z>>> &&
        CompositionAlgebra<math::octonion<fraction<Z>>>);
/*
    static_assert (Complex<fraction<math::complex<Z_bytes_little>>> &&
        CompositionAlgebra<fraction<math::complex<Z_bytes_little>>>);
    static_assert (Complex<fraction<math::complex<Z_bytes_big>>> &&
        CompositionAlgebra<fraction<math::complex<Z_bytes_big>>>);
    static_assert (Complex<fraction<math::complex<Z_bytes_BC_little>>> &&
        CompositionAlgebra<fraction<math::complex<Z_bytes_BC_little>>>);
    static_assert (Complex<fraction<math::complex<Z_bytes_BC_big>>> &&
        CompositionAlgebra<fraction<math::complex<Z_bytes_BC_big>>>);

    static_assert (complex<fraction<math::complex<Z>>> && CompositionAlgebra<fraction<math::complex<Z>>>);

    static_assert (Quaternionic<fraction<math::quaternion<Z_bytes_little>>> &&
        CompositionAlgebra<fraction<math::quaternion<Z_bytes_little>>>);
    static_assert (Quaternionic<fraction<math::quaternion<Z_bytes_big>>> &&
        CompositionAlgebra<fraction<math::quaternion<Z_bytes_big>>>);
    static_assert (Quaternionic<fraction<math::quaternion<Z_bytes_BC_little>>> &&
        CompositionAlgebra<fraction<math::quaternion<Z_bytes_BC_little>>>);
    static_assert (Quaternionic<fraction<math::quaternion<Z_bytes_BC_big>>> &&
        CompositionAlgebra<fraction<math::quaternion<Z_bytes_BC_big>>>);

    static_assert (Quaternionic<fraction<math::quaternion<Z>>> &&
        CompositionAlgebra<fraction<math::quaternion<Z>>>);

    static_assert (Octonionic<fraction<math::octonion<Z_bytes_little>>> &&
        CompositionAlgebra<fraction<math::octonion<Z_bytes_little>>>);
    static_assert (Octonionic<fraction<math::octonion<Z_bytes_big>>> &&
        CompositionAlgebra<fraction<math::octonion<Z_bytes_big>>>);
    static_assert (Octonionic<fraction<math::octonion<Z_bytes_BC_little>>> &&
        CompositionAlgebra<fraction<math::octonion<Z_bytes_BC_little>>>);
    static_assert (Octonionic<fraction<math::octonion<Z_bytes_BC_big>>> &&
        CompositionAlgebra<fraction<math::octonion<Z_bytes_BC_big>>>);

    static_assert (Octonionic<fraction<math::octonion<Z>>> &&
        CompositionAlgebra<fraction<math::octonion<Z>>>);*/

}

namespace data::math {

    template <uint64 pow> 
    set<dec_uint> root<dec_uint, pow>::operator () (const dec_uint &n) {
        return set<dec_uint> (root<N, pow> {} (N {n}));
    }
    
    template <uint64 pow> 
    set<dec_int> root<dec_int, pow>::operator () (const dec_int &n) {
        return set<dec_int> (root<Z, pow> {} (Z {n}));
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::uint<zz>> root<hex::uint<zz>, pow>::operator () (const hex::uint<zz> &n) {
        return set<hex::uint<zz>> (root<N, pow> {} (N {n}));
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::int2<zz>> root<hex::int2<zz>, pow>::operator () (const hex::int2<zz> &n) {
        return set<hex::int2<zz>> (root<Z, pow> {} (Z {n}));
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::intBC<zz>> root<hex::intBC<zz>, pow>::operator () (const hex::intBC<zz> &n) {
        return set<hex::intBC<zz>> (root<Z, pow> {} (Z {n}));
    }

    template <uint64 pow>
    set<base58_uint> root<base58_uint, pow>::operator () (const base58_uint &n) {
        return set<base58_uint> (root<N, pow> {} (N {n}));
    }
}

#endif
