// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_STREAM_CYPHER
#define DATA_CRYPTO_STREAM_CYPHER

#include <data/bytes.hpp>
#include <data/arithmetic/complementary.hpp>

namespace data::crypto::cipher::stream {

    // a block cipher mode takes a cipher to produce
    // a procedure that can be replied repeatedly to
    // encrypt or decrypt a whole message.
    template <typename cipher, size_t key_size> concept Cipher = requires (cipher &y, symmetric_key<key_size> k) {
        { y.stream (k) } -> std::derived_from<data::reader<byte>>;
    };

    // for stream ciphers, encrypt and decrypt are the same.
    template <typename key_size, Cipher<key_size> cipher>
    bytes inline crypt (const cipher &c, const symmetric_key<key_size> &k, bytes_view plaintext) {
        auto x = c.stream (k);
        bytes tor {};
        tor.resize (plaintext.size ());
        x >> tor;
        arithmetic::bit_xor<byte> (tor.end (), tor.begin (), (const byte *) tor.data (), (const byte *) (plaintext.data ()));
        return tor;
    }

}

#endif
