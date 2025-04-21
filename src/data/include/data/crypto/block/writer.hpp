// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_WRITER
#define DATA_CRYPTO_BLOCK_WRITER

#include <data/stream.hpp>
#include <data/crypto/block/mode.hpp>
#include <data/crypto/transform.hpp>

namespace data::crypto::CryptoPP {
    using namespace ::CryptoPP;

    // should conatin a typedef named value that returns the cryptopp type.
    template <typename cipher, size_t block_size, size_t key_size>
    requires block_cipher<cipher, block_size, key_size> struct get_cipher_type;

    template <typename mode, typename cipher, size_t block_size, size_t key_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size> struct get_mode_type;

    void inline detach_all (BufferedTransformation *tf) {
        if (tf == nullptr) return;
        detach_all (tf->AttachedTransformation ());
        tf->Detach ();
    }

}

namespace data::crypto {
    using block_padding = CryptoPP::BlockPaddingSchemeDef;

    template <size_t block_size>
    struct update_IV_session : session<byte> {
        void update_IV (const byte_array<block_size> &) = 0;

        virtual ~update_IV_session () {}
    };

    // add padding to a block cypher once a message is complete.
    template <typename mode, typename cipher, size_t block_size, size_t key_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size>
    struct encrypt_pad_session final : update_IV_session<block_size> {
        session<byte> &Output;
        void write (const byte *, size_t size) final override;

        // add the final padding.
        void complete () final override;

        encrypt_pad_session (session<byte> &, const mode &, const symmetric_key<key_size> &, block_padding);
    };

    template <typename mode, typename cipher, size_t block_size, size_t key_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size>
    struct prepend_IV_session final : session<byte> {
        update_IV_session<block_size> &Output;
        void write (const byte *, size_t size) final override;

        // add the final padding.
        void complete () final override;

        prepend_IV_session ();
    };

}

#endif
