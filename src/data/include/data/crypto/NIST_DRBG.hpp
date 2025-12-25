// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_NIST_DRBG
#define DATA_CRYPTO_NIST_DRBG

#include <data/random.hpp>
#include <data/encoding/endian.hpp>
#include <cryptopp/drbg.h>

namespace data::crypto::NIST {

    template <std::derived_from<CryptoPP::HashTransformation> hash> struct Hash_DRBG;
    template <typename hmac> struct HMAC_DRBG;
    
    struct DRBG final : random::entropy {
        // supported rngs
        enum type {
            HMAC,
            Hash
        };

        constexpr static uint32 DefaultBytesBeforeReseed = 0xffffffff;
        
        uint32 BytesBeforeReseed;
        entropy &Entropy;
        ::CryptoPP::NIST_DRBG* Random;
        
        constexpr static uint32 SecurityStrength = 16;

        struct initialization {
            entropy &Entropy;
            bytes Personalization;
            uint32_little Nonce;

            initialization (entropy &e, const bytes &personalization = {}, uint32_little nonce = 0):
                Entropy {e}, Personalization {personalization}, Nonce {nonce} {}
        };
        
        DRBG (type t, initialization i, uint32 bytes_before_reseed = DefaultBytesBeforeReseed) :
            BytesBeforeReseed {bytes_before_reseed}, Entropy {i.Entropy}, Random {nullptr},
            BytesRemaining {bytes_before_reseed} {
                bytes entropy (SecurityStrength);
                Entropy >> entropy;

                if (t == HMAC) {
                    Random = new ::CryptoPP::HMAC_DRBG
                        (entropy.data (), data::size (entropy), i.Nonce.data (), data::size (i.Nonce),
                            i.Personalization.data (), i.Personalization.size ());
                } else if (t == Hash) {
                    Random = new ::CryptoPP::Hash_DRBG
                        (entropy.data (), data::size (entropy), i.Nonce.data (), data::size (i.Nonce),
                            i.Personalization.data (), i.Personalization.size ());
                } else throw exception {} << "Invalid NIST DRBG type";
            }
        
        bool valid () const {
            return Random != nullptr;
        }
        
        ~DRBG () {
            delete Random;
        }

        void read (byte* b, size_t x) final override {
            while (BytesRemaining < x) {
                Random->GenerateBlock (b, BytesRemaining);
                b += BytesRemaining;
                x -= BytesRemaining;
                bytes entropy (SecurityStrength);
                Entropy >> entropy;
                Random->IncorporateEntropy (entropy.data (), entropy.size ());
                BytesRemaining = BytesBeforeReseed;
            }

            Random->GenerateBlock (b, x);
            BytesRemaining -= x;
        }
        
    private:
        uint32 BytesRemaining;
    };

}

#endif

