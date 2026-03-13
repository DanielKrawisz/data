
// Copyright (c) 2024-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_MAC_HMAC
#define DATA_CRYPTO_MAC_HMAC

#include <data/crypto/hash.hpp>
#include <data/crypto/encrypted.hpp>

namespace data::crypto {

    template <hash::Engine engine>
    requires requires { engine::BlockSize; }
    struct HMAC {
        constexpr static size_t BlockSize = engine::BlockSize;
        constexpr static size_t DigestSize = engine::DigestSize;

    private:
        byte ipad[BlockSize];
        byte opad[BlockSize];

        engine inner;
        engine outer;

        template <size_t key_size>
        void pads_from_key (slice<const byte, key_size> key) {
            byte K[BlockSize];

            int begin_pad;

            // If key_size > block_size
            // hash the key
            // copy digest into K0
            if (key_size > BlockSize) {
                engine g {};
                g.Update (key.data (), key_size);
                g.Final (K);
                begin_pad = DigestSize;
            // else copy key into K0
            } else {
                std::copy (key.begin (), key.end (), K);
                begin_pad = key_size;
            }
            // zero pad the rest.
            for (int i = begin_pad; i < BlockSize; i++) K[i] = 0;

            for (int i = 0; i < BlockSize; i++) {
                ipad[i] = K[i] ^ 0x36;
                opad[i] = K[i] ^ 0x5c;
            }
        }

        void initialize () {
            inner.Update (ipad, BlockSize);
            outer.Update (opad, BlockSize);
        }
    public:

        template <size_t key_size>
        HMAC (const symmetric_key<key_size> &key) {
            pads_from_key<key_size> (key);
            initialize ();
        }

        HMAC &Update (const byte *b, size_t x) {
            inner.Update (b, x);
            return *this;
        }

        void Final (byte mac[DigestSize]) {
            byte hash[DigestSize];
            inner.Final (hash);
            outer.Update (hash, DigestSize);
            outer.Final (mac);
        }

        HMAC &Restart () {
            inner.Restart ();
            outer.Restart ();
            initialize ();
        }

    };

}

#endif
