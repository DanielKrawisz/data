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
            541u, 523u, 521u, 509u, 503u, 499u, 491u, 487u, 479u, 467u, 463u, 461u, 457u, 449u,
            443u, 439u, 433u, 431u, 421u, 419u, 409u, 401u, 397u, 389u, 383u, 379u, 373u, 367u,
            359u, 353u, 349u, 347u, 337u, 331u, 317u, 313u, 311u, 307u, 293u, 283u, 281u, 277u,
            271u, 269u, 263u, 257u, 251u, 241u, 239u, 233u, 229u, 227u, 223u, 211u, 199u, 197u,
            193u, 191u, 181u, 179u, 173u, 167u, 163u, 157u, 151u, 149u, 139u, 137u, 131u, 127u,
            113u, 109u, 107u, 103u, 101u, 97u, 89u, 83u, 79u, 73u, 71u, 67u, 61u, 59u, 53u, 47u,
            43u, 41u, 37u, 31u, 29u, 23u, 19u, 17u, 13u, 11u, 7u, 5u, 3u, 2u};
            
        EXPECT_EQ (primes, expected);
    }

    template <typename X>
    struct Eratosthenes : ::testing::Test {
        using number = X;
    };

    // TODO should be able to work with
    // Z, Z_bytes, etc but not intX types.
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
        dec_uint, hex_uint, base58_uint>;

    TYPED_TEST_SUITE (Eratosthenes, test_cases);

    TYPED_TEST (Eratosthenes, TestType) {
        eratosthenes_test<typename TestFixture::number> ();
    }

}
