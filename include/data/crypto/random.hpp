#ifndef DATA_CRYPTO_RANDOM_HPP
#define DATA_CRYPTO_RANDOM_HPP

#include <data/types.hpp>

namespace data {
    
    namespace crypto {
        
        template <typename R>
        class random {
            R Random; 
            
        public:
            random(R r) : Random{r} {}
            
            random& operator>>(byte& x) {
                Random.next(&x, 1);
                return *this;
            }
            
            random& operator>>(uint16_t& x) {
                Random.next(reinterpret_cast<byte*>(&x), 2);
                return *this;
            }
            
            random& operator>>(uint32_t& x) {
                Random.next(reinterpret_cast<byte*>(&x), 4);
                return *this;
            }
            
            random& operator>>(uint64_t& x) {
                Random.next(reinterpret_cast<byte*>(&x), 8);
                return *this;
            }
            
            random& operator>>(std::vector<byte>& x) {
                Random.next(x.data(), x.size());
                return *this;
            }
            
            // don't misuse or risk infinite iteration. 
            template <typename X>
            random& operator>>(X& x) {
                do operator>>(static_cast<std::vector<byte>&>(x)); 
                while (!x.valid());
                return *this;
            }
        };
    
    }

}

#endif
