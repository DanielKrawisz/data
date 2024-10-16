// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA3
#define DATA_CRYPTO_SHA3

#include <data/crypto/MAC/MAC.hpp>
#include <data/crypto/hash/hash.hpp>
#include <cryptopp/hmac.h>

namespace data::crypto::MAC {

    template <size_t Size, size_t key_size>
    struct SHA3_MAC_writer : message_writer<digest<Size>, byte> {
        constexpr static size_t size = Size;

        hash::SHA3<size> Hash;
        symmetric_key<key_size> Key;

        SHA3_MAC_writer (const symmetric_key<key_size> k) : Hash {}, Key {k} {
            Hash.write (Key.data (), key_size);
        }

        void write (const byte *b, size_t x) final override {
            Hash.write (b, x);
        }

        digest<size> complete () final override {
            digest<size> d = Hash.complete ();
            Hash.write (Key.data (), key_size);
            return d;
        }

    };

}

#endif
