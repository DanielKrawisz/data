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
    
    // A number that is known to be prime. 
    // So far eratosthenes is the only way
    // to construct this type. 
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
        N Next;
        
        struct entry {
            N Prime;
            N Multiple;
            
            bool operator<=(const entry& e) const {
                return Multiple <= e.Multiple;
            }
            
            entry increment() const {
                return {Prime, Multiple + Prime};
            }
        };
        
        using heap = priority::queue<entry>;
        heap Sieve;
        
        eratosthenes(queue p, N m, heap sieve) : Primes{p}, Next{m}, Sieve{sieve} {}
        
        static heap insert_prime(heap sieve, N prime) {
            return sieve.insert(entry{prime, prime * 2});
        }; 
        
        eratosthenes step() const ;
        static bool test_next_prime(const N next, heap& q);
        
    public:
        eratosthenes() : Primes{}, Next{2}, Sieve{} {}
        eratosthenes(N n) : eratosthenes{eratosthenes{}.next(n)} {}
        
        // generate next n primes. 
        eratosthenes next(N n) const {
            N required = n + Primes.size();
            eratosthenes e = *this;
            while(required > e.Primes.size()) e = e.step();
            return e;
        }
        
        // generate next prime. 
        eratosthenes next() const {
            return next(1);
        }
        
    };
    
    template <typename N>
    bool eratosthenes<N>::test_next_prime(const N next, heap& q) {
        if (q.empty()) return true; 
        N multiple;
        while(true) {
            multiple = q.first().Multiple; 
            if (next == multiple) return false;
            if (next < multiple) return true;
            q = q.rest().insert(entry{q.first().increment()});
        } 
    }
    
    template <typename N>
    eratosthenes<N> eratosthenes<N>::step() const {
        heap q = Sieve;
        if (test_next_prime(Next, q)) return {Primes.append(Next), Next + 1, insert_prime(q, Next)};
        else return {Primes, Next + 1, q};
    }
    
}

#endif

