// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

// basic types
#include <data/types.hpp>

// endian types
#include <data/encoding/endian/arithmetic.hpp>

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

// Some important math functions. 
#include <data/math/number/abs.hpp>
#include <data/math/number/sqrt.hpp>

namespace data {

    template <typename N, typename Z> 
    inline N abs(const Z& i) {
        return math::number::abs<N, Z>{}(i);
    }
    
    template <typename X> 
    inline X arg(const X& x) {
        return math::number::arg<X>{}(x);
    }
    
    template <typename X, typename N>
    inline X sqrt(const N& n) {
        return math::number::sqrt<X, N>{}(n);
    }
    
}

// Natural numbers and integers
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/number/eratosthenes.hpp>
#include <data/math/number/gmp/aks.hpp>

namespace data {
    // we use a wrapper around gmp for natural numbers and integers. 
    
    // Natural numbers
    using N = math::number::N;
    
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
    
    // explicit instantiations of functions
    template math::sign sign<N>(const N&);
    
    template N abs<N, N>(const N&);
    
    template N arg<N>(const N&);
    
    template N sqrt<N, N>(const N&);
    
}

namespace data {
    
    // Integers
    using Z = math::number::Z;
    
    template math::sign sign<Z>(const Z&);
    
    template N abs<N, Z>(const Z&);
    template Z abs<Z, Z>(const Z&);
    
    template Z arg<Z>(const Z&);
    
    template N sqrt<N, Z>(const Z&);
    
}

// Some algebra
#include <data/math/polynomial.hpp>
#include <data/math/permutation.hpp>

namespace data {
    
    // Polynomials 
    template <typename X> 
    using polynomial = data::math::polynomial<X, N>;
    
    // permutations
    template <typename X>
    using permutation = data::math::permutation<X>;
    
}

#include <data/math/number/rational.hpp>

// rationals
namespace data {
    
    using Q = math::fraction<Z, N>;
    
    template math::sign sign<Q>(const Q&);
    
    using QPlus = math::nonnegative<Q>;

    template QPlus abs<QPlus, Q>(const Q&);
    
    template Q arg<Q>(const Q&);
}

#include <data/math/octonian.hpp>

// other normed division algebras. 
namespace data {
    
    // Gaussian numbers (complex rationals)
    using G = math::complex<Q>;
        
    // rational quaternions
    using H = math::quaternion<Q>;
        
    // rational octonions
    using O = math::octonion<Q>;
    
}

// more algebra
#include <data/math/algebra/finite_field.hpp>
#include <data/math/algebra/cyclic_group.hpp>
#include <data/math/algebra/dihedral_group.hpp>
#include <data/math/algebra/symmetric_group.hpp>
#include <data/math/algebra/alternating_group.hpp>

// algebraic constructs
namespace data {
    
    template <auto & mod>
    using prime_field = math::algebra::prime_field_element<N, Z, mod>;
    
    template <auto & mod>
    using cyclic_group = math::algebra::cyclic_group<N, mod>;
    
    template <auto & mod>
    using dihedral_group = math::algebra::dihedral_group<N, mod>;
    /*
    template <auto & mod>
    using symmetric_group = math::algebra::symmetric_group<N, mod>;
    
    template <auto & mod>
    using alternating_group = math::algebra::alternating_group<N, mod>;*/
    
}

// Geometry
#include <data/math/point.hpp>

namespace data {
    
    template <typename X, size_t size>
    using point = data::math::point<X, size>;
    
    template <typename X, size_t size>
    using vector = data::math::vector<X, size>;
}


#endif
