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

    template <typename Cipher, cipher::block::mode mode, auto ...mode_params> struct block_cipher;

    template <typename Cipher, cipher::block::mode mode, auto ...mode_params, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes encrypt (const block_cipher<Cipher, mode, mode_params...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext);

    template <typename Cipher, cipher::block::mode mode, auto ...mode_params, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes decrypt (const block_cipher<Cipher, mode, mode_params...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext);
}

namespace data::crypto::cipher {

    template <direction dir, typename Cipher, cipher::block::mode mode, size_t key_size, auto ...mode_params>
    struct reader<dir, block_cipher<Cipher, mode, mode_params...>, key_size>;

    template <direction dir, typename Cipher, cipher::block::mode mode, size_t key_size, auto ...mode_params>
    struct writer<dir, block_cipher<Cipher, mode, mode_params...>, key_size>;

}

namespace data::crypto {

    namespace {
        template <cipher::block::mode mode, size_t block_size, auto ...mode_params> struct get_mode_state {
            using type = cipher::block::state<mode, block_size, mode_params...>;
        };

        template <size_t block_size> struct get_mode_state<cipher::block::mode::ECB, block_size> {
            using type = cipher::block::state<cipher::block::mode::ECB>;
        };

        template <cipher::block::mode mode, size_t block_size, auto ...mode_params>
        using mode_state = get_mode_state<mode, block_size, mode_params...>::type;
    }

    template <typename Cipher, cipher::block::mode Mode, auto ...mode_params>
    struct block_cipher : mode_state<Mode, Cipher::BlockSize, mode_params...> {
        cipher::block::padding_scheme Padding;

        template <typename T> requires requires (T t) {
            mode_state<Mode, Cipher::BlockSize, mode_params...> {t};
        }
        constexpr block_cipher (T &&x, cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING):
        mode_state<Mode, Cipher::BlockSize, mode_params...> {std::forward<T> (x)}, Padding {p} {}

        constexpr block_cipher (cipher::block::padding_scheme p = cipher::block::padding::DEFAULT_PADDING):
        mode_state<Mode, Cipher::BlockSize, mode_params...> {}, Padding {p} {}

        template <size_t key_size> bytes encrypt (const symmetric_key<key_size> &k, byte_slice plaintext) const;
        template <size_t key_size> bytes decrypt (const symmetric_key<key_size> &k, byte_slice ciphertext) const;

        bool valid () const {
            return is_streamable (Mode) || Padding != cipher::block::padding::NO_PADDING;
        }
    };

    template <typename Cipher, cipher::block::mode Mode, auto ...mode_params>
    std::ostream &operator << (std::ostream &, const block_cipher<Cipher, Mode, mode_params...>);

    template <typename Cipher, cipher::block::mode mode, auto ...mode_params, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes inline encrypt (const block_cipher<Cipher, mode, mode_params...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext) {
        return algorithm.encrypt (key, plaintext);
    }

    template <typename Cipher, cipher::block::mode mode, auto ...mode_params, size_t key_size>
    requires cipher::block::Cipher<Cipher, key_size>
    bytes inline decrypt (const block_cipher<Cipher, mode, mode_params...> &algorithm, const symmetric_key<key_size> &key, const bytes &plaintext) {
        return algorithm.decrypt (key, plaintext);
    }
}

namespace data::crypto::cipher {

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

    template <direction dir, typename Cipher, cipher::block::mode mode, size_t key_size, auto ...mode_params>
    struct writer<dir, block_cipher<Cipher, mode, mode_params...>, key_size> : data::writer<byte> {
        cipher::block::writer<Cipher, mode_state<mode, Cipher::BlockSize, mode_params...>, key_size, dir> Encryptor;
        block::padding_scheme Padding;
        size_t BytesWritten {0};

        writer (
            const block_cipher<Cipher, mode, mode_params...> &algorithm,
            const symmetric_key<key_size> &k,
            data::writer<byte> &next): Encryptor {algorithm, k, next}, Padding {validate_padding<mode> (algorithm.Padding)} {}

        void write (const byte *b, size_t size) final override {
            Encryptor.write (b, size);
            BytesWritten += size;
        }

        ~writer () noexcept (false) {
            if (std::uncaught_exceptions () == 0 && Padding != block::padding::NO_PADDING)
                cipher::block::add_padding (Encryptor, Padding, Cipher::BlockSize, BytesWritten);
        }
    };

    template <direction dir, typename Cipher, cipher::block::mode mode, size_t key_size, auto ...mode_params>
    struct reader<dir, block_cipher<Cipher, mode, mode_params...>, key_size> : data::reader<byte> {
        cipher::block::reader<Cipher, mode_state<mode, Cipher::BlockSize, mode_params...>, key_size, dir> Decryptor;
        block::padding_scheme Padding;
        size_t BytesRead {0};

        reader (
            const block_cipher<Cipher, mode, mode_params...> &algorithm,
            const symmetric_key<key_size> &k, data::reader<byte> &prev):
            Decryptor {algorithm, k, prev}, Padding {validate_padding<mode> (algorithm.padding)} {}

        void read (byte *b, size_t size) final override {
            Decryptor.read (b, size);
            BytesRead += size;
        }

        // TODO this doesn't work because we don't use the number of bytes written.
        ~reader () noexcept (false) {
            if (std::uncaught_exceptions () == 0 && Padding != block::padding::NO_PADDING)
                block::remove_padding_reader {Decryptor, Cipher::BlockSize, Padding};
        }
    };

}

namespace data::crypto {

    template <typename Cipher, cipher::block::mode Mode, auto ...mode_params> template <size_t key_size>
    bytes block_cipher<Cipher, Mode, mode_params...>::encrypt (const symmetric_key<key_size> &k, byte_slice plaintext) const {
        bytes ciphertext;

        {
            lazy_bytes_writer lazy {ciphertext};
            cipher::writer<cipher::encryption, block_cipher<Cipher, Mode, mode_params...>, key_size> cryptor {*this, k, lazy};
            cryptor << plaintext;
        } //result is written to here.

        return ciphertext;
    }

    template <typename Cipher, cipher::block::mode Mode, auto ...v> template <size_t key_size>
    bytes block_cipher<Cipher, Mode, v...>::decrypt (const symmetric_key<key_size> &k, byte_slice ciphertext) const {
        auto p = cipher::validate_padding<Mode> (Padding);

        bytes plaintext;
        {
            lazy_bytes_writer lazy {plaintext};
            cipher::block::writer<Cipher, mode_state<Mode, Cipher::BlockSize, v...>, key_size, cipher::decryption>
            cryptor {*this, k, lazy};
            cryptor << ciphertext;
        }

        return p == cipher::block::padding::NO_PADDING ?
            plaintext :
            cipher::block::remove_padding (p, Cipher::BlockSize, plaintext);
    }
}

#endif
