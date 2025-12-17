// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_WRITER
#define DATA_CRYPTO_BLOCK_WRITER

#include <data/crypto/block/padding.hpp>
#include <data/crypto/block/mode.hpp>
#include <data/crypto/transform.hpp>

namespace data::crypto::CryptoPP {

    // should conatin a typedef named type that returns the cryptopp type.
    template <typename cipher> struct get_cipher_type;

    // should conatin a typedef named type that returns the cryptopp type.
    template <typename mode, typename cipher> struct get_block_mode_type;

    template <typename T, CryptoPP::CipherDir Dir>
    struct select_cipher_direction;

    template <typename T>
    struct select_cipher_direction<T, CryptoPP::ENCRYPTION> {
        using type = typename T::Encryption;
    };

    template <typename T>
    struct select_cipher_direction<T, CryptoPP::DECRYPTION> {
        using type = typename T::Decryption;
    };

}

namespace data::crypto {

    using dir = CryptoPP::CipherDir;

    template <typename T, dir D> using cryptopp_cipher_dir = typename CryptoPP::select_cipher_direction<T, D>::type;
    template <typename cipher> using cryptopp_cipher = typename CryptoPP::get_cipher_type<cipher>::type;
    template <typename mode, typename cipher> using cryptopp_block_mode = typename CryptoPP::get_block_mode_type<mode, cipher>::type;

    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    struct block_session final : session<byte> {
        using cryptopp_cipher_type = cryptopp_cipher_dir<cryptopp_cipher<cipher>, direction>;
        using cryptopp_mode_type = cryptopp_block_mode<mode, cipher>;
        using cryptopp_algorithm_type = cryptopp_cipher_dir<cryptopp_mode_type, direction>;;

        cryptopp_cipher_type Cipher;
        cryptopp_algorithm_type Mode;
        ptr<CryptoPP::StreamTransformationFilter> Filter;
        void write (const byte *, size_t size) final override;

        void complete () final override;

        void update (const mode &);

        block_session (
            CryptoPP::BufferedTransformation *next_step,
            const mode &, const symmetric_key<key_size> &);

        ~block_session ();
    };

    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    struct block_writer final : message_writer<bytes, byte> {
        block_session<mode, cipher, block_size, key_size, direction> Session;
        buffer_writer Buffer;

        void write (const byte *, size_t size) final override;

        // add the final padding.
        bytes complete () final override;

        void update (const mode &);

        block_writer (
            const mode &, const symmetric_key<key_size> &);

    };

    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    void inline block_session<mode, cipher, block_size, key_size, direction>::write (const byte *b, size_t size) {
        Filter->Put (b, size);
    }

    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    void inline block_session<mode, cipher, block_size, key_size, direction>::complete () {
        Filter->MessageEnd ();
    }

    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    inline block_session<mode, cipher, block_size, key_size, direction>::~block_session () {
        Filter->Detach ();
    }

    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    inline block_session<mode, cipher, block_size, key_size, direction>::block_session
        (CryptoPP::BufferedTransformation *next_step, const mode &m, const symmetric_key<key_size> &k):
        Cipher {}, Mode {}, Filter {} {

        Cipher.SetKey (k.data (), k.size ());
        Filter = std::make_shared<CryptoPP::StreamTransformationFilter> (Mode, next_step, block_padding::NO_PADDING);

        if constexpr (requires { m.IV; }) {
            Mode.SetCipherWithIV (Cipher, m.IV.data (), m.IV.size ());
        } else {
            Mode.SetCipher (Cipher);
        }
    }

    // note: this won't work.
    template <typename mode, typename cipher, size_t block_size, size_t key_size, dir direction>
    requires BlockCipherMode<mode, cipher, block_size, key_size>
    void inline block_session<mode, cipher, block_size, key_size, direction>::update (const mode &m) {
        auto next_step = Filter->AttachedTransformation ();
        Filter->Detach ();
        Filter = std::make_shared<CryptoPP::StreamTransformationFilter> (Mode, next_step, block_padding::NO_PADDING);
    }

}

#endif
