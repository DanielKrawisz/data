// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK
#define DATA_CRYPTO_BLOCK

#include <data/crypto/cipher.hpp>
#include <data/crypto/block/mode.hpp>
#include <data/crypto/block/padding.hpp>

#include <data/tools/lazy_writer.hpp>

#include <data/io/log.hpp>

namespace data::crypto {

    template <typename mode>
    struct block_cipher {
        mode Mode;
        cipher::block::padding_scheme Padding;
        block_cipher (const mode &m, cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING):
            Mode {m}, Padding {p} {}
    };

    template <typename mode> block_cipher (const mode &) -> block_cipher<mode>;

    template <typename mode>
    struct virtual_stream_cipher {
        mode Mode;
        virtual_stream_cipher (const mode &m): Mode {m} {}
    };

    template <typename mode> virtual_stream_cipher (const mode &) -> virtual_stream_cipher<mode>;

    // We require padding styles that can be used to detect a decryption error.
    template <typename alg, size_t key_size, cipher::block::Mode<alg, key_size> mode>
    bytes encrypt (const block_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext);

    template <typename alg, size_t key_size, cipher::block::Mode<alg, key_size> mode>
    bytes decrypt (const block_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &ciphertext);

    template <typename alg, size_t key_size, cipher::block::StreamableMode<alg, key_size> mode>
    bytes encrypt (const virtual_stream_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext);

    template <typename alg, size_t key_size, cipher::block::StreamableMode<alg, key_size> mode>
    bytes decrypt (const virtual_stream_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &ciphertext);

    cipher::block::padding_scheme inline validate_padding (cipher::block::padding_scheme p) {
        if (p == cipher::block::padding::DEFAULT_PADDING) return cipher::block::padding::ONE_AND_ZEROS_PADDING;
        if (p != cipher::block::padding::ONE_AND_ZEROS_PADDING && p != cipher::block::padding::PKCS_PADDING)
            throw exception {} << "Supported padding modes are PKCS and one and zeros";
        return p;
    }

    // We require padding styles that can be used to detect a decryption error.
    template <typename alg, size_t key_size, cipher::block::Mode<alg, key_size> mode>
    bytes encrypt (const block_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext) {
        bytes ciphertext;

        {
            lazy_bytes_writer lazy {ciphertext};
            cipher::block::cryptor<alg, key_size, mode, cipher::encryption> cryptor {algorithm.Mode, key, lazy};
            cipher::block::add_padding_session ww {cryptor, mode::BlockSize, validate_padding (algorithm.Padding)};
            ww << plaintext << end_message {};
        } //padding is added and result is written to here on destruction of writers.

        return ciphertext;
    }

    template <typename alg, size_t key_size, cipher::block::Mode<alg, key_size> mode>
    bytes decrypt (const block_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &ciphertext) {
        auto p = validate_padding (algorithm.Padding);

        bytes plaintext;
        {
            lazy_bytes_writer lazy {plaintext};
            cipher::block::cryptor<alg, key_size, mode, cipher::decryption> cryptor {algorithm.Mode, key, lazy};
            cryptor << ciphertext << end_message {};
        }

        return cipher::block::remove_padding (p, mode::BlockSize, plaintext);
    }

    template <typename alg, size_t key_size, cipher::block::StreamableMode<alg, key_size> mode>
    bytes encrypt (const virtual_stream_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext) {
        bytes result;

        {
            lazy_bytes_writer lazy {result};
            cipher::block::cryptor<alg, key_size, mode, cipher::encryption> cryptor {algorithm.Mode, key, lazy};
            cryptor << plaintext;
        } // result is written to on destruction.

        return result;
    }

    template <typename alg, size_t key_size, cipher::block::StreamableMode<alg, key_size> mode>
    bytes decrypt (const virtual_stream_cipher<mode> &algorithm, const symmetric_key<key_size> &key, const bytes &ciphertext) {

        bytes result;
        {
            lazy_bytes_writer lazy {result};
            cipher::block::cryptor<alg, key_size, mode, cipher::decryption> cryptor {algorithm.Mode, key, lazy};
            cryptor << ciphertext;
        } // result is written to on destruction.

        return result;
    }

}

#endif
