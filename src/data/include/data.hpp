// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

// basic types
#include <data/types.hpp>

// endian types
#include <data/arithmetic/endian.hpp>

// This library uses a valid() method and a Valid member in a stereotyped way.
#include <data/valid.hpp>

// A implementations of data structures.
#include <data/tools.hpp>

// functional constructs
#include <data/fold.hpp>
#include <data/for_each.hpp>

// Thread safe communication channel, similar to golang.
#include <data/tools/channel.hpp>

namespace data {

template <typename X> using chan = tool::channel<X>;

}

#include <data/numbers.hpp>

// Some important math functions.
#include <data/math/abs.hpp>
#include <data/math/root.hpp>

// Natural numbers and integers
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/number/eratosthenes.hpp>
#include <data/math/number/gmp/aks.hpp>
#include <data/math/number/gmp/sqrt.hpp>

namespace data {
    
    // we use a wrapper around gmp for natural numbers and integers. 
    
    // Natural numbers
    
    // Primes
    using prime = math::number::prime<N>;
    
    // right now there are only two ways to generate primes
    // and neither is all that useful. 
    
    // The sieve of Eratosthenes.
    // Good for generating all primes in sequence. 
    using Eratosthenes = math::number::eratosthenes<N>;
    
    // Primality testing. AKS is polynomial time but still too 
    // slow for practical use. 
    using AKS = math::number::AKS<N>;
    
}

#include <data/math.hpp>
#include <data/numbers.hpp>

#endif
