// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ERATOSTHENES
#define DATA_MATH_NUMBER_ERATOSTHENES

#include <data/types.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/list/linked_list.hpp>

namespace data::math::number {
    template <typename N>
    struct eratosthenes {
        using queue = priority_queue<N, N>;
        using list = linked_list<N>;
        
        list Primes;
        N Max;
        queue Queue;
        
    private:
        eratosthenes(list p, N m, queue q) : Primes{p}, Max{m}, Queue{q} {}
        
        static queue insert_prime(queue q, N p) {
            return q.insert(p, p * 2);
        };
        
        static queue cycle(queue q) {
            typename queue::priority p = q.first();
            return q.rest().insert(p.Entry, p.Priority + p.Entry);
        }
        
        eratosthenes step() const;
        
    public:
        eratosthenes() : Primes{}, Max{1}, Queue{} {}
        
        eratosthenes next(N n) const {
            N required = Primes.size() + n;
            eratosthenes e = *this;
            while(e.Primes.size() < required) e = e.step();
            return e;
        }
        
        eratosthenes next() const {
            return next(1);
        }
        
    };
    
}

#endif

