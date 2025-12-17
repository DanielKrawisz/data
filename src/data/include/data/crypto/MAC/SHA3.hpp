// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA3
#define DATA_CRYPTO_SHA3

#include <data/crypto/MAC.hpp>
#include <data/crypto/hash.hpp>
#include <cryptopp/hmac.h>

namespace data::crypto::MAC {

    // note: this is not really how you're supposed to use sha3 as a stream cipher.
    template <size_t size>
    struct SHA3_MAC_writer : data::writer<byte> {
        constexpr static size_t Size = size;
        using digest = hash::digest<Size>;

        digest &Digest;
        hash::SHA3<size> Hash;

        SHA3_MAC_writer (digest &d, byte_slice k) : Digest {d}, Hash {} {
            Hash.Update (k.data (), k.size ());
        }

        void write (const byte *b, size_t x) final override {
            Hash.Update (b, x);
        }

        ~SHA3_MAC_writer () {
            Hash.Final (Digest.data ());
        }
    };

}

#endif
