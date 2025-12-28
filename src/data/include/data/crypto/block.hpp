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

    template <typename Cipher, cipher::block::mode mode, auto ...v> struct block_cipher;

    template <typename Cipher, cipher::block::mode mode, auto ...v, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes encrypt (const block_cipher<Cipher, mode, v...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext);

    template <typename Cipher, cipher::block::mode mode, auto ...v, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes decrypt (const block_cipher<Cipher, mode, v...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext);

    namespace {
        template <cipher::block::mode mode, size_t block_size, auto ...v> struct get_mode_state {
            using type = cipher::block::state<mode, block_size, v...>;
        };

        template <size_t block_size, auto ...v> struct get_mode_state<cipher::block::mode::ECB, block_size, v...> {
            using type = cipher::block::state<cipher::block::mode::ECB, v...>;
        };

        template <cipher::block::mode mode, size_t block_size, auto ...v>
        using mode_state = get_mode_state<mode, block_size, v...>::type;
    }

    template <typename Cipher, cipher::block::mode Mode, auto ...v>
    struct block_cipher : mode_state<Mode, Cipher::BlockSize, v...> {
        cipher::block::padding_scheme Padding;

        template <typename T> requires requires (T t) {
            mode_state<Mode, Cipher::BlockSize, v...> {t};
        }
        constexpr block_cipher (T &&x, cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING):
        mode_state<Mode, Cipher::BlockSize, v...> {std::forward<T> (x)}, Padding {p} {}

        constexpr block_cipher (cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING):
        mode_state<Mode, Cipher::BlockSize, v...> {}, Padding {p} {}

        template <size_t key_size> bytes encrypt (const symmetric_key<key_size> &k, byte_slice plaintext) const;
        template <size_t key_size> bytes decrypt (const symmetric_key<key_size> &k, byte_slice ciphertext) const;

        bool valid () const {
            return is_streamable (Mode) || Padding != cipher::block::padding::NO_PADDING;
        }
    };

    template <typename Cipher, cipher::block::mode Mode, auto ...v>
    std::ostream &operator << (std::ostream &, const block_cipher<Cipher, Mode, v...>);

    template <typename Cipher, cipher::block::mode mode, auto ...v, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes inline encrypt (const block_cipher<Cipher, mode, v...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext) {
        return algorithm.encrypt (key, plaintext);
    }

    template <typename Cipher, cipher::block::mode mode, auto ...v, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes inline decrypt (const block_cipher<Cipher, mode, v...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext) {
        return algorithm.decrypt (key, plaintext);
    }

    // we require padding schemes that enable identification of the end
    // of the message. Valid padding schemes are ONE_AND_ZEROS_PADDING,
    // PKCS_PADDING, and, for streamable modes, NO_PADDING. For
    // streamable modes, default padding is NO_PADDING. Otherwise, it
    // is ONE_AND_ZEROS_PADDING.
    template <cipher::block::mode mode>
    cipher::block::padding_scheme inline validate_padding (cipher::block::padding_scheme p) {
        if (p == cipher::block::padding::DEFAULT_PADDING)
            return cipher::block::is_streamable (mode) ?
                cipher::block::padding::NO_PADDING :
                cipher::block::padding::ONE_AND_ZEROS_PADDING;

        if (cipher::block::is_streamable (mode) && p == cipher::block::padding::NO_PADDING)
            return p;

        if (p != cipher::block::padding::ONE_AND_ZEROS_PADDING && p != cipher::block::padding::PKCS_PADDING)
            throw exception {} << "Supported padding modes are PKCS, one and zeros, and no padding for streamable modes.";

        return p;
    }

    template <typename Cipher, cipher::block::mode Mode, auto ...v> template <size_t key_size>
    bytes block_cipher<Cipher, Mode, v...>::encrypt (const symmetric_key<key_size> &k, byte_slice plaintext) const {

        cipher::block::padding_scheme p = validate_padding<Mode> (Padding);
        bytes ciphertext;

        {
            lazy_bytes_writer lazy {ciphertext};
            cipher::block::cryptor<Cipher, mode_state<Mode, Cipher::BlockSize, v...>, key_size, cipher::encryption>
            cryptor {*this, k, lazy};

            if (p == cipher::block::padding::NO_PADDING) {
                cryptor << plaintext;
            } else {
                cipher::block::add_padding_writer padded {cryptor, Cipher::BlockSize, validate_padding<Mode> (Padding)};
                padded << plaintext;
            } // padding added here

        } //result is written to here.

        return ciphertext;
    }

    template <typename Cipher, cipher::block::mode Mode, auto ...v> template <size_t key_size>
    bytes block_cipher<Cipher, Mode, v...>::decrypt (const symmetric_key<key_size> &k, byte_slice ciphertext) const {
        auto p = validate_padding<Mode> (Padding);

        bytes plaintext;
        {
            lazy_bytes_writer lazy {plaintext};
            cipher::block::cryptor<Cipher, mode_state<Mode, Cipher::BlockSize, v...>, key_size, cipher::decryption>
            cryptor {*this, k, lazy};
            cryptor << ciphertext;
        }

        return p == cipher::block::padding::NO_PADDING ?
            plaintext :
            cipher::block::remove_padding (p, Cipher::BlockSize, plaintext);
    }
}

namespace data::crypto::cipher {

    template <typename Cipher, cipher::block::mode mode, auto ...v> struct encryptor<block_cipher<Cipher, mode, v...>>;
    template <typename Cipher, cipher::block::mode mode, auto ...v> struct decryptor<block_cipher<Cipher, mode, v...>>;

}

#endif
