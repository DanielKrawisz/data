// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_STREAM_CYPHER
#define DATA_CRYPTO_STREAM_CYPHER

#include <data/types.hpp>

namespace data::crypto {

    // a block cipher mode takes a cipher to produce
    // a procedure that can be replied repeatedly to
    // encrypt or decrypt a whole message.
    template <typename cipher>
    concept stream_cipher = requires (cipher &y, byte in) {
        { y.crypt (in) } -> std::same_as<byte>;
    };

}

#endif
