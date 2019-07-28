// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/eratosthenes.hpp>

namespace data::math::number {
        
    template <typename N>
    eratosthenes<N> eratosthenes<N>::step() const {
        N n = Max + 1;
        bool presumed_prime = true;
        queue q = queue();
        if (!q.empty()) do {
            N next = q.first().Priority;
            if (next > n) break;
            if (next == n) presumed_prime = false;
            q = cycle(q);
        } while (true);
        if (presumed_prime) return {Primes.append(n), n, insert_prime(q, n)};
        return {Primes, n, q};
    }
    
}

