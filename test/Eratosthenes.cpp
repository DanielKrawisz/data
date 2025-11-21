// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/number/eratosthenes.hpp"
#include "data/numbers.hpp"
#include "data/for_each.hpp"
#include "gtest/gtest.h"

namespace data {
    
    template <typename N> void eratosthenes_test () {

        const data::uint32 max_primes = 100;
        
        using prime = math::number::prime<N>;
            
        list<N> primes = for_each (
            [] (const prime p) -> N {
                return p.Prime.Value;
            }, 
            data::math::number::eratosthenes<N> {N {max_primes}}.Primes);
        
        EXPECT_TRUE (primes.size () == max_primes);
        
        list<N> expected {
            541, 523, 521, 509, 503, 499, 491, 487, 479, 467, 463, 461, 457, 449,
            443, 439, 433, 431, 421, 419, 409, 401, 397, 389, 383, 379, 373, 367,
            359, 353, 349, 347, 337, 331, 317, 313, 311, 307, 293, 283, 281, 277,
            271, 269, 263, 257, 251, 241, 239, 233, 229, 227, 223, 211, 199, 197,
            193, 191, 181, 179, 173, 167, 163, 157, 151, 149, 139, 137, 131, 127,
            113, 109, 107, 103, 101, 97, 89, 83, 79, 73, 71, 67, 61, 59, 53, 47,
            43, 41, 37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
            
        EXPECT_EQ (primes, expected);
    }

    template <WholeNumber X>
    struct Eratosthenes : ::testing::Test {
        using number = X;
    };

    using test_cases = ::testing::Types<
        uint64, uint64_little, uint64_big,
        uint80, uint80_little, uint80_big,
        uint128, uint128_little, uint128_big,
        uint160, uint256, uint512,
        N, N_bytes_little, N_bytes_big,
        math::N_bytes<endian::little, unsigned short>,
        math::N_bytes<endian::little, unsigned int>,
        math::N_bytes<endian::little, unsigned long>,
        math::N_bytes<endian::little, unsigned long long>,
        dec_uint, hex_uint, //base58_uint,
        Z, Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        math::Z_bytes<endian::little, unsigned short>,
        math::Z_bytes<endian::little, unsigned int>,
        math::Z_bytes<endian::little, unsigned long>,
        math::Z_bytes<endian::little, unsigned long long>>;

    TYPED_TEST_SUITE (Eratosthenes, test_cases);

    TYPED_TEST (Eratosthenes, TestType) {
        eratosthenes_test<typename TestFixture::number> ();
    }

}
