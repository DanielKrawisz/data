// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_RANDOM
#define DATA_CRYPTO_RANDOM

#include <data/iterable.hpp>

namespace data::crypto {
    
    struct random {
        random& operator>>(byte& x) {
            get(&x, 1);
            return *this;
        }
        
        random& operator>>(uint16_t& x) {
            get(reinterpret_cast<byte*>(&x), 2);
            return *this;
        }
        
        random& operator>>(uint32_t& x) {
            get(reinterpret_cast<byte*>(&x), 4);
            return *this;
        }
        
        random& operator>>(uint64_t& x) {
            get(reinterpret_cast<byte*>(&x), 8);
            return *this;
        }
        
        random& operator>>(bytes& x) {
            get(x.data(), x.size());
            return *this;
        }
        
    protected: 
        virtual void get(byte*, size_t) = 0;
    };
    
    struct entropy {
        struct fail : std::exception {
            
        };
        
        virtual bytes get(size_t) = 0;
    };

}

#endif
