// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_CIPHER
#define DATA_CRYPTO_CIPHER

#include <data/array.hpp>

namespace data::crypto::cipher {

    enum direction {
        encryption,
        decryption
    };

    template <direction dir, typename X, auto ...> struct reader;
    template <direction dir, typename Y, auto ...> struct writer;

    template <size_t key_size> using symmetric_key = byte_array<key_size>;

    template <size_t block_size>
    using initialization_vector = byte_array<block_size>;



}

#endif

