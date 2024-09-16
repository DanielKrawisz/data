// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_NIST_DRBG
#define DATA_CRYPTO_NIST_DRBG

#include <data/crypto/random.hpp>
#include <data/numbers.hpp>
#include <cryptopp/drbg.h>

namespace data::crypto::NIST {
    
    struct DRBG final : random {
        // supported rngs
        enum type {
            HMAC,
            Hash
        };

        constexpr static uint32 DefaultBytesBeforeReseed = 0xffff;
        
        uint32 BytesBeforeReseed;
        entropy &Entropy;
        ::CryptoPP::NIST_DRBG* Random;
        
        constexpr static uint32 SecurityStrength = 16;

        struct initialization {
            entropy &Entropy;
            bytes Personalization;
            uint32_little Nonce;

            initialization (entropy &e, bytes personalization = {}, uint32_little nonce = 0):
                Entropy {e}, Personalization {personalization}, Nonce {nonce} {}
        };
        
        DRBG (type t, initialization i, uint32 bytes_before_reseed = DefaultBytesBeforeReseed) :
            BytesBeforeReseed {bytes_before_reseed}, Entropy {i.Entropy}, Random {nullptr},
            BytesRemaining {bytes_before_reseed} {
                bytes entropy = Entropy.get (SecurityStrength);

                if (t == HMAC) {
                    Random = new ::CryptoPP::HMAC_DRBG
                        (entropy.data (), entropy.size (), i.Nonce.data (), i.Nonce.size (),
                            i.Personalization.data (), i.Personalization.size ());
                } else if (t == Hash) {
                    Random = new ::CryptoPP::Hash_DRBG
                        (entropy.data (), entropy.size (), i.Nonce.data (), i.Nonce.size (),
                            i.Personalization.data (), i.Personalization.size ());
                } else throw exception {} << "Invalid NIST DRBG type";
            }
        
        bool valid () const {
            return Random != nullptr;
        }
        
        ~DRBG () {
            delete Random;
        }

        void get (byte* b, size_t x) override {
            if (BytesRemaining < x) {
                bytes entropy = Entropy.get (Random->SecurityStrength ());
                Random->IncorporateEntropy (entropy.data (), entropy.size ());
                BytesRemaining = BytesBeforeReseed - (x - BytesRemaining);
            }

            Random->GenerateBlock (b, x);
            BytesRemaining -= x;
        }
        
    private:
        uint32 BytesRemaining;
    };

}

#endif

