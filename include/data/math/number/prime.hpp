// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ERATOSTHENES
#define DATA_MATH_NUMBER_ERATOSTHENES

#include <data/types.hpp>
#include <data/math/number/prime.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/list/linked.hpp>

namespace data::math::number {
    template <typename N> struct eratosthenes;
    template <typename N> struct AKS;
    
    template <typename N> 
    struct prime {
        N Prime;
        
    private:
        prime(N p) : Prime{p} {}
        
        friend struct eratosthenes<N>;
        friend struct AKS<N>;
    };
    
}

#endif


