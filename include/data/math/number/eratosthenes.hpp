// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ERATOSTHENES
#define DATA_MATH_NUMBER_ERATOSTHENES

#include <data/types.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/queue/functional_queue.hpp>

namespace data::math::number {
    template <typename N> struct eratosthenes;
    
    template <typename N> 
    struct prime {
        N Prime;
        
    private:
        prime(N p) : Prime{p} {}
        
        friend struct eratosthenes<N>;
    };
    
    template <typename N>
    struct eratosthenes {
        using list = data::list::linked<N>;
        using queue = functional_queue<prime<N>, data::list::linked<prime<N>>>;
        
        queue Primes;
        
    private:
        N Max;
        
        struct entry {
            N Prime;
            N Multiple;
            
            bool operator<=(const entry& e) const {
                return Multiple <= e.Multiple;
            }
        };
        
        using heap = priority::queue<entry>;
        heap Sieve;
        
        eratosthenes(queue p, N m, heap sieve) : Primes{p}, Max{m}, Sieve{sieve} {}
        
        static heap insert_prime(heap sieve, N prime) {
            return sieve.insert(prime, prime * 2);
        }; 
        
        eratosthenes step() const ;
        
    public:
        eratosthenes() : Primes{}, Max{1}, Sieve{} {}
        
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

