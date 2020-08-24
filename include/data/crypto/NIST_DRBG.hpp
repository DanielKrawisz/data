// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_NIST
#define DATA_CRYPTO_NIST

#include <data/crypto/random.hpp>
#include <crypto++/drbg.h>

namespace data::crypto::nist {
    
    struct drbg final : random {
        // supported rngs
        enum type {
            HMAC_DRBG, 
            HASH_DRBG
        };
        
        ptr<entropy> Entropy;
        ptr<CryptoPP::NIST_DRBG> Random;
        uint32 BytesBeforeReseed;
        
        drbg(type, ptr<entropy>, bytes personalization, uint32 nonce) {
            
        }
        
        bool valid() const {
            return Entropy != nullptr && Random != nullptr;
        }
        
    private:
        void get(byte* b, size_t x) override {
            if (BytesBeforeReseed < x) { 
                bytes entropy = Entropy->get(Random->SecurityStrength());
                Random->IncorporateEntropy(entropy.data(), entropy.size());
                BytesBeforeReseed = 65536;
            }
            
            Random->GenerateBlock(b, x);
            BytesBeforeReseed -= x;
        }
    };

}

#endif

