// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_PRIME
#define DATA_MATH_NUMBER_PRIME

#include <data/types.hpp>

namespace data::math::number {
    template <typename N> struct eratosthenes;
    template <typename N> struct AKS;
    
    // A number that is known to be prime. 
    // So far eratosthenes is the only way
    // to construct this type. 
    // TODO support AKS.
    template <typename N> 
    struct prime {
        N Prime;
        
        bool valid() const {
            return Prime != N(0);
        }
        
        prime() : Prime{0} {}
    private:
        prime(N p) : Prime{p} {}
        
        friend struct eratosthenes<N>;
        friend struct AKS<N>;
    };
    
}

#endif


