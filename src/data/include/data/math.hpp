// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH
#define DATA_MATH

#include <data/numbers.hpp>
#include <data/complex.hpp>

#include <data/math/permutation.hpp>
#include <data/math/polynomial.hpp>

#include <data/math/fraction.hpp>
#include <data/math/octonion.hpp>

namespace data {
    
    // fractions
    template <typename Z> using fraction = math::fraction<Z>;
    
    // Polynomials 
    template <typename X, typename N, char x = 'x'> using polynomial = math::polynomial<X, N, x>;

    template <ordered X> using permutation = math::permutation<X>;

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

    // Various kinds of real numbers.
    static_assert (real<float32> && math::normed_field<float32>);
    static_assert (real<float64> && math::normed_field<float64>);

    static_assert (real<int32> && math::normed_ring<int32>);
    static_assert (real<int32_little> && math::normed_ring<int32_little>);
    static_assert (real<int32_big> && math::normed_ring<int32_big>);

    static_assert (real<int64> && math::normed_ring<int64>);
    static_assert (real<int64_little> && math::normed_ring<int64_little>);
    static_assert (real<int64_big> && math::normed_ring<int64_big>);

    static_assert (real<int80> && math::normed_ring<int80>);
    static_assert (real<int80_little> && math::normed_ring<int80_little>);
    static_assert (real<int80_big> && math::normed_ring<int80_big>);

    static_assert (real<int128> && math::normed_ring<int128>);
    static_assert (real<int128_little> && math::normed_ring<int128_little>);
    static_assert (real<int128_big> && math::normed_ring<int128_big>);

    static_assert (real<int160> && math::normed_ring<int160>);
    static_assert (real<int160_little> && math::normed_ring<int160_little>);
    static_assert (real<int160_big> && math::normed_ring<int160_big>);

    static_assert (real<Z_bytes_little> && math::normed_ring<Z_bytes_little>);
    static_assert (real<Z_bytes_big> && math::normed_ring<Z_bytes_big>);
    static_assert (real<Z_bytes_BC_little> && math::normed_ring<Z_bytes_BC_little>);
    static_assert (real<Z_bytes_BC_big> && math::normed_ring<Z_bytes_BC_big>);

    static_assert (real<Z> && math::normed_ring<Z>);

    // next we look at complexifications of integers and floating points.
    static_assert (complex<math::complex<float64>> && math::normed_field<math::complex<float64>>);

    static_assert (complex<math::complex<int64>> && math::normed_ring<math::complex<int64>>);
    static_assert (complex<math::complex<int64_little>> && math::normed_ring<math::complex<int64_little>>);
    static_assert (complex<math::complex<int64_big>> && math::normed_ring<math::complex<int64_big>>);

    static_assert (complex<math::complex<int80>> && math::normed_ring<math::complex<int80>>);
    static_assert (complex<math::complex<int80_little>> && math::normed_ring<math::complex<int80_little>>);
    static_assert (complex<math::complex<int80_big>> && math::normed_ring<math::complex<int80_big>>);

    static_assert (complex<math::complex<Z_bytes_little>> && math::normed_ring<math::complex<Z_bytes_little>>);
    static_assert (complex<math::complex<Z_bytes_big>> && math::normed_ring<math::complex<Z_bytes_big>>);
    static_assert (complex<math::complex<Z_bytes_BC_little>> && math::normed_ring<math::complex<Z_bytes_BC_little>>);
    static_assert (complex<math::complex<Z_bytes_BC_big>> && math::normed_ring<math::complex<Z_bytes_BC_big>>);

    static_assert (complex<math::complex<Z>> && math::normed_ring<math::complex<Z>>);

    static_assert (quaternionic<math::quaternion<float64>> && math::normed_field<math::quaternion<float64>>);

    static_assert (quaternionic<math::quaternion<int64>> && math::normed_ring<math::quaternion<int64>>);
    static_assert (quaternionic<math::quaternion<int64_little>> && math::normed_ring<math::quaternion<int64_little>>);
    static_assert (quaternionic<math::quaternion<int64_big>> && math::normed_ring<math::quaternion<int64_big>>);

    static_assert (quaternionic<math::quaternion<int80>> && math::normed_ring<math::quaternion<int80>>);
    static_assert (quaternionic<math::quaternion<int80_little>> &&
        math::normed_ring<math::quaternion<int80_little>>);
    static_assert (quaternionic<math::quaternion<int80_big>> &&
        math::normed_ring<math::quaternion<int80_little>>);

    static_assert (quaternionic<math::quaternion<Z_bytes_little>> &&
        math::normed_ring<math::quaternion<Z_bytes_little>>);
    static_assert (quaternionic<math::quaternion<Z_bytes_big>> &&
        math::normed_ring<math::quaternion<Z_bytes_big>>);
    static_assert (quaternionic<math::quaternion<Z_bytes_BC_little>> &&
        math::normed_ring<math::quaternion<Z_bytes_BC_little>>);
    static_assert (quaternionic<math::quaternion<Z_bytes_BC_big>> &&
        math::normed_ring<math::quaternion<Z_bytes_BC_big>>);

    static_assert (quaternionic<math::quaternion<Z>> && math::normed_ring<math::quaternion<Z>>);

    static_assert (octonionic<math::octonion<float64>> &&
        math::normed_field<math::octonion<float64>>);

    static_assert (octonionic<math::octonion<int64>> &&
        math::normed_ring<math::octonion<int64>>);
    static_assert (octonionic<math::octonion<int64_little>> &&
        math::normed_ring<math::octonion<int64_little>>);
    static_assert (octonionic<math::octonion<int64_big>> &&
        math::normed_ring<math::octonion<int64_big>>);

    static_assert (octonionic<math::octonion<int80>> &&
        math::normed_ring<math::octonion<int80>>);
    static_assert (octonionic<math::octonion<int80_little>> &&
        math::normed_ring<math::octonion<int80_little>>);
    static_assert (octonionic<math::octonion<int80_big>> &&
        math::normed_ring<math::octonion<int80_big>>);

    static_assert (octonionic<math::octonion<Z_bytes_little>> &&
        math::normed_ring<math::octonion<Z_bytes_little>>);
    static_assert (octonionic<math::octonion<Z_bytes_big>> &&
        math::normed_ring<math::octonion<Z_bytes_big>>);
    static_assert (octonionic<math::octonion<Z_bytes_BC_little>> &&
        math::normed_ring<math::octonion<Z_bytes_BC_little>>);
    static_assert (octonionic<math::octonion<Z_bytes_BC_big>> &&
        math::normed_ring<math::octonion<Z_bytes_BC_big>>);

    static_assert (octonionic<math::octonion<Z>> && math::normed_ring<math::octonion<Z>>);

    // next we look at rational types constructed from integer types.
    static_assert (real<fraction<int64>> && math::normed_field<fraction<int64>>);
    static_assert (real<fraction<int64_little>> && math::normed_field<fraction<int64_little>>);
    static_assert (real<fraction<int64_big>> && math::normed_field<fraction<int64_big>>);

    static_assert (real<fraction<int80>> && math::normed_field<fraction<int80>>);
    static_assert (real<fraction<int80_little>> && math::normed_field<fraction<int80_little>>);
    static_assert (real<fraction<int80_big>> && math::normed_field<fraction<int80_big>>);

    static_assert (real<fraction<Z_bytes_little>> && math::normed_field<fraction<Z_bytes_little>>);
    static_assert (real<fraction<Z_bytes_big>> && math::normed_field<fraction<Z_bytes_big>>);
    static_assert (real<fraction<Z_bytes_BC_little>> && math::normed_field<fraction<Z_bytes_BC_little>>);
    static_assert (real<fraction<Z_bytes_BC_big>> && math::normed_field<fraction<Z_bytes_BC_big>>);

    static_assert (real<fraction<Z>> && math::normed_field<fraction<Z>>);

    static_assert (complex<math::complex<fraction<int64>>> && math::normed_field<math::complex<fraction<int64>>>);
    static_assert (complex<math::complex<fraction<int64_little>>> &&
        math::normed_field<math::complex<fraction<int64_little>>>);
    static_assert (complex<math::complex<fraction<int64_big>>> &&
        math::normed_field<math::complex<fraction<int64_big>>>);

    static_assert (complex<math::complex<fraction<int80>>> &&
        math::normed_field<math::complex<fraction<int80>>>);
    static_assert (complex<math::complex<fraction<int80_little>>> &&
        math::normed_field<math::complex<fraction<int80_little>>>);
    static_assert (complex<math::complex<fraction<int80_big>>> &&
        math::normed_field<math::complex<fraction<int80_big>>>);

    static_assert (complex<math::complex<fraction<Z_bytes_little>>> &&
        math::normed_field<math::complex<fraction<Z_bytes_little>>>);
    static_assert (complex<math::complex<fraction<Z_bytes_big>>> &&
        math::normed_field<math::complex<fraction<Z_bytes_big>>>);
    static_assert (complex<math::complex<fraction<Z_bytes_BC_little>>> &&
        math::normed_field<math::complex<fraction<Z_bytes_BC_little>>>);
    static_assert (complex<math::complex<fraction<Z_bytes_BC_big>>> &&
        math::normed_field<math::complex<fraction<Z_bytes_BC_big>>>);

    static_assert (complex<math::complex<fraction<Z>>> && math::normed_field<math::complex<fraction<Z>>>);

    static_assert (quaternionic<math::quaternion<fraction<int64>>> &&
        math::normed_field<math::quaternion<fraction<int64>>>);
    static_assert (quaternionic<math::quaternion<fraction<int64_little>>> &&
        math::normed_field<math::quaternion<fraction<int64_little>>>);
    static_assert (quaternionic<math::quaternion<fraction<int64_big>>> &&
        math::normed_field<math::quaternion<fraction<int64_big>>>);

    static_assert (quaternionic<math::quaternion<fraction<int80>>> &&
        math::normed_field<math::quaternion<fraction<int80>>>);
    static_assert (quaternionic<math::quaternion<fraction<int80_little>>> &&
        math::normed_field<math::quaternion<fraction<int80_little>>>);
    static_assert (quaternionic<math::quaternion<fraction<int80_big>>> &&
        math::normed_field<math::quaternion<fraction<int80_big>>>);

    static_assert (quaternionic<math::quaternion<fraction<Z_bytes_little>>> &&
        math::normed_field<math::quaternion<fraction<Z_bytes_little>>>);
    static_assert (quaternionic<math::quaternion<fraction<Z_bytes_big>>> &&
        math::normed_field<math::quaternion<fraction<Z_bytes_big>>>);
    static_assert (quaternionic<math::quaternion<fraction<Z_bytes_BC_little>>> &&
        math::normed_field<math::quaternion<fraction<Z_bytes_BC_little>>>);
    static_assert (quaternionic<math::quaternion<fraction<Z_bytes_BC_big>>> &&
        math::normed_field<math::quaternion<fraction<Z_bytes_BC_big>>>);

    static_assert (quaternionic<math::quaternion<fraction<Z>>> &&
        math::normed_field<math::quaternion<fraction<Z>>>);

    static_assert (octonionic<math::octonion<fraction<int64>>> &&
        math::normed_field<math::octonion<fraction<int64>>>);
    static_assert (octonionic<math::octonion<fraction<int64_little>>> &&
        math::normed_field<math::octonion<fraction<int64_little>>>);
    static_assert (octonionic<math::octonion<fraction<int64_big>>> &&
        math::normed_field<math::octonion<fraction<int64_big>>>);

    static_assert (octonionic<math::octonion<fraction<int80>>> &&
        math::normed_field<math::octonion<fraction<int80>>>);
    static_assert (octonionic<math::octonion<fraction<int80_little>>> &&
        math::normed_field<math::octonion<fraction<int80_little>>>);
    static_assert (octonionic<math::octonion<fraction<int80_big>>> &&
        math::normed_field<math::octonion<fraction<int80_big>>>);

    static_assert (octonionic<math::octonion<fraction<Z_bytes_little>>> &&
        math::normed_field<math::octonion<fraction<Z_bytes_little>>>);
    static_assert (octonionic<math::octonion<fraction<Z_bytes_big>>> &&
        math::normed_field<math::octonion<fraction<Z_bytes_big>>>);
    static_assert (octonionic<math::octonion<fraction<Z_bytes_BC_little>>> &&
        math::normed_field<math::octonion<fraction<Z_bytes_BC_little>>>);
    static_assert (octonionic<math::octonion<fraction<Z_bytes_BC_big>>> &&
        math::normed_field<math::octonion<fraction<Z_bytes_BC_big>>>);

    static_assert (octonionic<math::octonion<fraction<Z>>> &&
        math::normed_field<math::octonion<fraction<Z>>>);
/*
    static_assert (complex<fraction<math::complex<int64>>> && math::normed_field<fraction<math::complex<int64>>>);
    static_assert (complex<fraction<math::complex<int64_little>>> && math::normed_field<fraction<math::complex<int64_little>>>);
    static_assert (complex<fraction<math::complex<int64_big>>> && math::normed_field<fraction<math::complex<int64_big>>>);

    static_assert (complex<fraction<math::complex<int80>>> && math::normed_field<fraction<math::complex<int80>>>);
    static_assert (complex<fraction<math::complex<int80_little>>> &&
        math::normed_field<fraction<math::complex<int80_little>>>);
    static_assert (complex<fraction<math::complex<int80_big>>> &&
        math::normed_field<fraction<math::complex<int80_big>>>);

    static_assert (complex<fraction<math::complex<Z_bytes_little>>> &&
        math::normed_field<fraction<math::complex<Z_bytes_little>>>);
    static_assert (complex<fraction<math::complex<Z_bytes_big>>> &&
        math::normed_field<fraction<math::complex<Z_bytes_big>>>);
    static_assert (complex<fraction<math::complex<Z_bytes_BC_little>>> &&
        math::normed_field<fraction<math::complex<Z_bytes_BC_little>>>);
    static_assert (complex<fraction<math::complex<Z_bytes_BC_big>>> &&
        math::normed_field<fraction<math::complex<Z_bytes_BC_big>>>);

    static_assert (complex<fraction<math::complex<Z>>> && math::normed_field<fraction<math::complex<Z>>>);

    static_assert (quaternionic<fraction<math::quaternion<int64>>> &&
        math::normed_field<fraction<math::quaternion<int64>>>);
    static_assert (quaternionic<fraction<math::quaternion<int64_little>>> &&
        math::normed_field<fraction<math::quaternion<int64_little>>>);
    static_assert (quaternionic<fraction<math::quaternion<int64_big>>> &&
        math::normed_field<fraction<math::quaternion<int64_big>>>);

    static_assert (quaternionic<fraction<math::quaternion<int80>>> &&
        math::normed_field<fraction<math::quaternion<int80>>>);
    static_assert (quaternionic<fraction<math::quaternion<int80_little>>> &&
        math::normed_field<fraction<math::quaternion<int80_little>>>);
    static_assert (quaternionic<fraction<math::quaternion<int80_big>>> &&
        math::normed_field<fraction<math::quaternion<int80>>>);

    static_assert (quaternionic<fraction<math::quaternion<Z_bytes_little>>> &&
        math::normed_field<fraction<math::quaternion<Z_bytes_little>>>);
    static_assert (quaternionic<fraction<math::quaternion<Z_bytes_big>>> &&
        math::normed_field<fraction<math::quaternion<Z_bytes_big>>>);
    static_assert (quaternionic<fraction<math::quaternion<Z_bytes_BC_little>>> &&
        math::normed_field<fraction<math::quaternion<Z_bytes_BC_little>>>);
    static_assert (quaternionic<fraction<math::quaternion<Z_bytes_BC_big>>> &&
        math::normed_field<fraction<math::quaternion<Z_bytes_BC_big>>>);

    static_assert (quaternionic<fraction<math::quaternion<Z>>> &&
        math::normed_field<fraction<math::quaternion<Z>>>);

    static_assert (octonionic<fraction<math::octonion<int64>>> &&
        math::normed_field<fraction<math::octonion<int64>>>);
    static_assert (octonionic<fraction<math::octonion<int64_little>>> &&
        math::normed_field<fraction<math::octonion<int64_little>>>);
    static_assert (octonionic<fraction<math::octonion<int64_big>>> &&
        math::normed_field<fraction<math::octonion<int64_big>>>);

    static_assert (octonionic<fraction<math::octonion<int80>>> &&
        math::normed_field<fraction<math::octonion<int80>>>);
    static_assert (octonionic<fraction<math::octonion<int80_little>>> &&
        math::normed_field<fraction<math::octonion<int80_little>>>);
    static_assert (octonionic<fraction<math::octonion<int80_big>>> &&
        math::normed_field<fraction<math::octonion<int80_big>>>);

    static_assert (octonionic<fraction<math::octonion<Z_bytes_little>>> &&
        math::normed_field<fraction<math::octonion<Z_bytes_little>>>);
    static_assert (octonionic<fraction<math::octonion<Z_bytes_big>>> &&
        math::normed_field<fraction<math::octonion<Z_bytes_big>>>);
    static_assert (octonionic<fraction<math::octonion<Z_bytes_BC_little>>> &&
        math::normed_field<fraction<math::octonion<Z_bytes_BC_little>>>);
    static_assert (octonionic<fraction<math::octonion<Z_bytes_BC_big>>> &&
        math::normed_field<fraction<math::octonion<Z_bytes_BC_big>>>);

    static_assert (octonionic<fraction<math::octonion<Z>>> &&
        math::normed_field<fraction<math::octonion<Z>>>);*/
    
}

#endif
