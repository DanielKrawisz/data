// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

// basic types
#include <data/float.hpp>
#include <data/math.hpp>

#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/bounded/bounded.hpp>

#include <data/encoding/integer.hpp>

#include <data/math/number/bytes/N.hpp>
#include <data/math/number/gmp/gmp.hpp>

#include <data/encoding/base58.hpp>

#include <data/math/number/rational.hpp>
#include <data/math/octonion.hpp>

#include <data/math.hpp>

namespace data {
    // we use a wrapper around gmp for natural numbers and integers. 
    
    // Natural numbers
    using N = math::number::N;
    
    using N_bytes_little = math::N_bytes<endian::little>;
    
    using N_bytes_big = math::N_bytes<endian::big>;
    
    // Integers
    using Z = math::number::Z;
    
    using Z_bytes_little = math::Z_bytes<endian::little>;
    
    using Z_bytes_big = math::Z_bytes<endian::big>;
    
    // bounted integers and uints. 
    template <size_t size> 
    using int_little = math::sint_little<size>;
    
    template <size_t size> 
    using int_big = math::sint_big<size>;
    
    template <size_t size> 
    using uint_little = math::uint_little<size>;
    
    template <size_t size> 
    using uint_big = math::uint_big<size>;
    
    using uint128 = uint_little<16>;
    using uint160 = uint_little<20>;
    using uint224 = uint_little<28>;
    using uint256 = uint_little<32>;
    using uint320 = uint_little<40>;
    using uint448 = uint_little<56>;
    using uint512 = uint_little<64>;
    
    using int128 = int_little<16>;
    using int160 = int_little<20>;
    using int224 = int_little<28>;
    using int256 = int_little<32>;
    using int320 = int_little<40>;
    using int448 = int_little<56>;
    using int512 = int_little<64>;
    
    // rational numbers. 
    using Q = math::fraction<Z, N>;
    
    using Q_big = math::fraction<Z_bytes_big, N_bytes_little>;
    using Q_little = math::fraction<Z_bytes_little, N_bytes_big>;
    
    // Gaussian integers. 
    using G = math::complex<Z>;
    
    // complex rationals
    using CQ = math::complex<Q>;
        
    // rational quaternions
    using HQ = math::quaternion<Q>;
        
    // rational octonions
    using OQ = math::octonion<Q>;
    
    using complex64 = math::complex<float32>;
    using complex128 = math::complex<float64>;
    using quat128 = math::quaternion<float32>;
    using quat256 = math::quaternion<float64>;
    using oct256 = math::octonion<float32>;
    using oct512 = math::octonion<float64>;
    
}

#endif
