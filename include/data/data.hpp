// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

// basic types
#include <data/types.hpp>

// endian types
#include <data/encoding/endian.hpp>

// This library uses a valid() method and a Valid member in a stereotyped way. 
#include <data/valid.hpp>

// Interfaces related to data structures. 
#include <data/interface.hpp> 
#include <data/set.hpp>
#include <data/stack.hpp>
#include <data/map.hpp>
#include <data/tree.hpp>

// A implementations of data structures. 
#include <data/list/linked.hpp>
#include <data/map/rb.hpp>
#include <data/tools/map_set.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/tree/linked.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/tools/ordered_list.hpp>

namespace data {
    
    template <typename X> using stack = functional::stack::linked<X>;
        
    // functional queue built using the list. 
    template <typename X> using list = tool::functional_queue<stack<X>>;
    
    // tree. 
    template <typename X> using tree = functional::tree::linked<X>;
    
    // a functional map implemented as a red-black tree
    // wrapper of Milewski's implementation of Okasaki.
    template <typename K, typename V> using map = functional::rb_map<K, V>;
    
    // priority queue. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using priority_queue = tool::priority_queue<X, list<X>>;
    
    // ordered_list. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using ordered_list = tool::ordered_list<X>;
    
    // set implemented as a map. 
    template <typename X> using set = tool::map_set<map<X, unit>>;
    
    // get all values from a map with the given keys. 
    template <typename key, typename value, typename map>
    list<value> get_all(map m, list<key> k) {
        struct inner {
            map M;
                    
            inner(map m) : M{m} {}
                    
            list<value> operator()(key k, list<value> l) {
                value v = M[k];
                if (v == value{}) return l;
                return l + v;
            }
        };
                
        return reduce(inner{m}, k);
    }

}

// functional constructs
#include <data/fold.hpp>
#include <data/for_each.hpp>

// Natural numbers and integers
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/number/eratosthenes.hpp>
#include <data/math/number/gmp/aks.hpp>

namespace data {
    // we use a wrapper around gmp for natural numbers and integers. 
    
    // Natural numbers
    using N = math::number::N;
    
    // Integers
    using Z = math::number::Z;
    
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

// Algebra 
#include <data/math/number/rational.hpp>
#include <data/math/octonian.hpp>
#include <data/math/algebra/finite_field.hpp>
#include <data/math/polynomial.hpp>
#include <data/math/permutation.hpp>

// Geometry
#include <data/math/point.hpp>

namespace data {
        
    // Rationals. 
    using Q = math::number::fraction<Z, N>;
    
    // Gaussian numbers (complex rationals)
    using G = math::complex<Q>;
        
    // rational quaternions
    using H = math::quaternion<Q>;
        
    // rational octonions
    using O = math::octonion<Q>;
    
    template <auto & mod>
    using prime_field_element = math::algebra::prime_field_element<N, Z, mod>;
    
    template <auto & mod>
    using prime_field = math::algebra::prime_field<N, Z, mod>;
    
    // Polynomials 
    template <typename X> 
    using polynomial = data::math::polynomial<X, N>;
    
    // permutations
    template <typename X>
    using permutation = data::math::permutation<X>;
    
}

// Geometry
#include <data/math/point.hpp>

namespace data {
    
    template <typename X, size_t size>
    using vector = data::math::vector<X, size>;
    
    template <typename X, size_t size>
    using point = data::math::point<X, size>;
}

// Some important math functions. 
#include <data/math/number/abs.hpp>

namespace data {
    
    template <typename X> math::sign 
    inline sign(const X& x) {
        return math::number::sign<X>{}(x);
    }

    template <typename N, typename Z> 
    inline N abs(const Z& i) {
        return math::number::abs<N, Z>{}(i);
    }
    
    template <typename X> 
    inline X arg(const X& x) {
        return math::number::arg<X>{}(x);
    }
    
}

#include <data/tools/channel.hpp>

namespace data {
    
    // Thread safe communication channel, similar to golang. 
    template <typename X> using chan = tool::channel<X>;
    
}

// Other representations of numbers that don't work yet. 

#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bounded/bounded.hpp>

namespace data {
    /*
    // fixed-size numbers of any size. 
    template <size_t size> using uint = math::number::bounded<size, endian::little, false>;
    template <size_t size> using integer = math::number::bounded<size, endian::little, true>;
    
    using uint160 = uint<20>;
    using uint256 = uint<32>;
    using uint512 = uint<64>;

    using int160 = integer<20>;
    using int256 = integer<32>;
    using int512 = integer<64>;*/

    // representations of the naturals and integers of any size as byte strings. 
    template <endian::order o> using N_bytes = math::number::N_bytes<o>;
    template <endian::order o> using Z_bytes = math::number::Z_bytes<o>;
    
}

// explicit function instantiations. 
namespace data {
    
    template math::sign sign<N>(const N&);
    template math::sign sign<Z>(const Z&);
    /*
    template math::sign sign<N_bytes<endian::big>>(const N_bytes<endian::big>&);
    template math::sign sign<Z_bytes<endian::big>>(const Z_bytes<endian::big>&);
    
    template math::sign sign<N_bytes<endian::little>>(const N_bytes<endian::little>&);
    template math::sign sign<Z_bytes<endian::little>>(const Z_bytes<endian::little>&);*/
    /*
    template math::sign sign<uint160>(const uint160&);
    template math::sign sign<int160>(const int160&);
    
    template math::sign sign<uint256>(const uint256&);
    template math::sign sign<int256>(const int256&);
    
    template math::sign sign<uint512>(const uint512&);
    template math::sign sign<int512>(const int512&);*/
    
    template math::sign sign<Q>(const Q&);
    
    template N abs<N, N>(const N&);
    template N abs<N, Z>(const Z&);
    template Z abs<Z, Z>(const Z&);
    /*
    template N_bytes<endian::big> abs<N_bytes<endian::big>, N_bytes<endian::big>>(const N_bytes<endian::big>&);
    template N_bytes<endian::big> abs<N_bytes<endian::big>, Z_bytes<endian::big>>(const Z_bytes<endian::big>&);
    
    template N_bytes<endian::little> abs<N_bytes<endian::little>, N_bytes<endian::little>>(const N_bytes<endian::little>&);
    template N_bytes<endian::little> abs<N_bytes<endian::little>, Z_bytes<endian::little>>(const Z_bytes<endian::little>&);*/
    /*
    template uint160 abs<uint160, uint160>(const uint160&);
    template uint160 abs<uint160, int160>(const int160&);
    
    template uint256 abs<uint256, uint256>(const uint256&);
    template uint256 abs<uint256, int256>(const int256&);
    
    template uint512 abs<uint512, uint512>(const uint512&);
    template uint512 abs<uint512, int512>(const int512&);*/

    template Q abs<Q, Q>(const Q&);
    
    template N arg<N>(const N&);
    template Z arg<Z>(const Z&);
    /*
    template N_bytes<endian::big> arg<N_bytes<endian::big>>(const N_bytes<endian::big>&);
    template Z_bytes<endian::big> arg<Z_bytes<endian::big>>(const Z_bytes<endian::big>&);
    
    template N_bytes<endian::little> arg<N_bytes<endian::little>>(const N_bytes<endian::little>&);
    template Z_bytes<endian::little> arg<Z_bytes<endian::little>>(const Z_bytes<endian::little>&);*/
    /*
    template uint160 arg<uint160>(const uint160&);
    template int160 arg<int160>(const int160&);
    
    template uint256 arg<uint256>(const uint256&);
    template int256 arg<int256>(const int256&);
    
    template uint512 arg<uint512>(const uint512&);
    template int512 arg<int512>(const int512&);*/
    
    template Q arg<Q>(const Q&);
    
}

#endif
