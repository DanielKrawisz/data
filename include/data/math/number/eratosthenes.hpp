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
        using list = linked_list<N>;
        using queue = functional_queue<prime<N>>;
        
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
        
        eratosthenes step() const {
            N n = Max + 1;
            heap sieve = Sieve;
            entry e;
            while (true) {
                e = sieve.first();
                if (e.Multiple >= n) break;
                sieve = sieve.insert(entry{e.Prime, e.Multiple + e.Prime});
            }
            if (e.Multiple == n) return eratosthenes{Primes, n, sieve};
            return eratosthenes{Primes.insert(prime<N>{n}), n, insert_prime(sieve, n)};
        }
        
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

