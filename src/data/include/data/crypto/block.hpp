// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK
#define DATA_CRYPTO_BLOCK

#include <data/crypto/block/writer.hpp>
#include <data/crypto/block/cryptopp.hpp>

namespace data::crypto {

    template <typename alg, size_t block_size, typename mode, size_t key_size>
    requires (cipher::block::template Mode<mode, alg, key_size>)
    bytes encrypt (bytes msg, const mode &m, const symmetric_key<key_size> &k, cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING) {
        // only PKCS and one and zeros are supported for this operation.
        if (p == cipher::block::padding::DEFAULT_PADDING) p = cipher::block::padding::ONE_AND_ZEROS_PADDING;
        else if (p != cipher::block::padding::ONE_AND_ZEROS_PADDING && p != cipher::block::padding::PKCS_PADDING)
            throw exception {} << "Supported padding modes are PKCS and one and zeros";

        buffer_writer bw;
        cipher::block::session<key_size, alg, mode, CryptoPP::ENCRYPTION> bs {new CryptoPP::Redirector (bw.Sink), m, k};
        cipher::block::add_padding_session pp {bs, block_size, p};
        pp << msg;
        pp.complete ();
        return bw.Result;
    }

    template <typename alg, size_t block_size, typename mode, size_t key_size>
    requires (cipher::block::template Mode<mode, alg, key_size>)
    bytes decrypt (bytes encrypted, const mode &m, const symmetric_key<key_size> &k, cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING) {
        if (p == cipher::block::padding::DEFAULT_PADDING) p = cipher::block::padding::ONE_AND_ZEROS_PADDING;
        else if (p != cipher::block::padding::ONE_AND_ZEROS_PADDING && p != cipher::block::padding::PKCS_PADDING)
            throw exception {} << "Supported padding modes are PKCS and one and zeros";

        buffer_writer bw;
        cipher::block::session<key_size, alg, mode, CryptoPP::DECRYPTION> bs {new CryptoPP::Redirector (bw.Sink), m, k};
        bs << encrypted;
        bs.complete ();
        return cipher::block::remove_padding (p, block_size, bw.Result);
    }

}

#endif
