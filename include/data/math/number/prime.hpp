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
        enum likelihood {
            certain, 
            probable, 
            impossible
        };
        
        N Prime;
        likelihood Likelihood;
        
        bool valid() const {
            return Likelihood != impossible;
        }
        
        prime() : Prime{0}, Likelihood{impossible} {}
    private:
        prime(N p, likelihood l) : Prime{p}, Likelihood{l} {}
        
        friend struct eratosthenes<N>;
        friend struct AKS<N>;
    };
    
}

#endif


