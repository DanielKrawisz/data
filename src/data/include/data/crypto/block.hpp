// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK
#define DATA_CRYPTO_BLOCK

#include <data/crypto/block/writer.hpp>

namespace data::crypto {

    template <typename cipher, size_t block_size, typename mode, size_t key_size>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    bytes encrypt (bytes msg, const mode &m, const symmetric_key<key_size> &k, block_padding_scheme p = block_padding::ONE_AND_ZEROS_PADDING) {
        // only PKCS and one and zeros are supported for this operation.
        if (p == block_padding::DEFAULT_PADDING) p = block_padding::ONE_AND_ZEROS_PADDING;
        else if (p != block_padding::ONE_AND_ZEROS_PADDING && p != block_padding::PKCS_PADDING)
            throw exception {} << "Supported padding modes are PKCS and one and zeros";

        buffer_writer bw;
        block_session<mode, cipher, block_size, key_size, CryptoPP::ENCRYPTION> bs {new CryptoPP::Redirector (bw.Sink), m, k};
        add_padding_session pp {bs, block_size, p};
        pp << msg;
        pp.complete ();
        return bw.Result;
    }

    template <typename cipher, size_t block_size, typename mode, size_t key_size>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    bytes decrypt (bytes encrypted, const mode &m, const symmetric_key<key_size> &k, block_padding_scheme p) {
        if (p == block_padding::DEFAULT_PADDING) p = block_padding::ONE_AND_ZEROS_PADDING;
        else if (p != block_padding::ONE_AND_ZEROS_PADDING && p != block_padding::PKCS_PADDING)
            throw exception {} << "Supported padding modes are PKCS and one and zeros";

        buffer_writer bw;
        block_session<mode, cipher, block_size, key_size, CryptoPP::DECRYPTION> bs {new CryptoPP::Redirector (bw.Sink), m, k};
        bs << encrypted;
        bs.complete ();
        return remove_padding (p, block_size, bw.Result);
    }

}

#endif
