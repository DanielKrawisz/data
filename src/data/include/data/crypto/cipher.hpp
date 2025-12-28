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

    template <typename X> struct encryptor;
    template <typename Y> struct decryptor;

    template <size_t key_size> using symmetric_key = byte_array<key_size>;

    template <size_t block_size>
    using initialization_vector = byte_array<block_size>;



}

#endif

