// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_NIST
#define DATA_CRYPTO_NIST

#include <data/crypto/random.hpp>
#include <cryptopp/drbg.h>

namespace data::crypto::nist {
    
    struct drbg final : random {
        // supported rngs
        enum type {
            HMAC_DRBG, 
            Hash_DRBG
        };
        
        uint32 BytesBeforeReseed;
        ptr<entropy> Entropy;
        ptr<CryptoPP::NIST_DRBG> Random;
        
        drbg(type t, ptr<entropy> e, bytes personalization, uint32_little nonce) : 
            BytesBeforeReseed{65536}, Entropy{e}, Random{nullptr} {
                if (t == HMAC_DRBG) {
                    bytes entropy = Entropy->get(Random->SecurityStrength());
                    Random = std::static_pointer_cast<CryptoPP::NIST_DRBG>(
                        std::make_shared<CryptoPP::HMAC_DRBG>(
                            entropy.data(), entropy.size(), 
                            personalization.data(), personalization.size(), 
                            nonce.data(), nonce.size()));
                } else if (t == Hash_DRBG) {
                    bytes entropy = Entropy->get(Random->SecurityStrength());
                    Random = std::static_pointer_cast<CryptoPP::NIST_DRBG>(
                        std::make_shared<CryptoPP::Hash_DRBG>(
                            entropy.data(), entropy.size(), 
                            personalization.data(), personalization.size(), 
                            nonce.data(), nonce.size()));
                }
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

