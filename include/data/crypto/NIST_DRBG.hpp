// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_NIST_DRBG
#define DATA_CRYPTO_NIST_DRBG

#include <data/crypto/random.hpp>
#include <cryptopp/drbg.h>

namespace data::crypto::NIST {
    
    struct DRBG final : random {
        // supported rngs
        enum type {
            HMAC,
            Hash
        };
        
        uint32 BytesBeforeReseed;
        entropy &Entropy;
        ::CryptoPP::NIST_DRBG* Random;
        
        constexpr static uint32 SecurityStrength = 16;
        
        DRBG (type t, entropy &e, bytes personalization = {}, uint32_little nonce = 0) :
            BytesBeforeReseed {65536}, Entropy {e}, Random {nullptr} {
                bytes entropy = Entropy.get (SecurityStrength);
                if (t == HMAC) {
                    Random = new ::CryptoPP::HMAC_DRBG
                        (entropy.data (), entropy.size (), nonce.data (), nonce.size (),
                            personalization.data (), personalization.size ());
                } else if (t == Hash) {
                    Random = new ::CryptoPP::Hash_DRBG
                        (entropy.data (), entropy.size (), nonce.data (), nonce.size (),
                            personalization.data (), personalization.size ());
                }
            }
        
        bool valid () const {
            return Random != nullptr;
        }
        
        ~DRBG () {
            delete Random;
        }
        
    private:
        
        void get (byte* b, size_t x) override {
            if (BytesBeforeReseed < x) { 
                bytes entropy = Entropy.get (Random->SecurityStrength ());
                Random->IncorporateEntropy (entropy.data (), entropy.size ());
                BytesBeforeReseed = 65536;
            }
            
            Random->GenerateBlock (b, x);
            BytesBeforeReseed -= x;
        }
    };

}

#endif

