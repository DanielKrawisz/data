// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_WRITER
#define DATA_CRYPTO_BLOCK_WRITER

#include <data/crypto/block/padding.hpp>
#include <data/crypto/block/mode.hpp>
#include <data/crypto/transform.hpp>

namespace data::crypto::cipher::block::CryptoPP {

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

namespace data::crypto::cipher::block {

    using dir = CryptoPP::CipherDir;

    template <typename T, dir D> using cryptopp_cipher_dir = typename CryptoPP::select_cipher_direction<T, D>::type;
    template <typename alg> using cryptopp_cipher = typename CryptoPP::get_cipher_type<alg>::type;
    template <typename mode, typename alg> using cryptopp_block_mode = typename CryptoPP::get_block_mode_type<mode, alg>::type;

    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    struct session final : data::out_session<byte> {
        using cryptopp_cipher_type = cryptopp_cipher_dir<cryptopp_cipher<cipher>, direction>;
        using cryptopp_mode_type = cryptopp_block_mode<mode, cipher>;
        using cryptopp_algorithm_type = cryptopp_cipher_dir<cryptopp_mode_type, direction>;

        cryptopp_cipher_type Cipher;
        cryptopp_algorithm_type Mode;
        ptr<CryptoPP::StreamTransformationFilter> Filter;
        void write (const byte *, size_t size) final override;

        void complete () final override;

        void update (const mode &);

        session (
            CryptoPP::BufferedTransformation *next_step,
            const mode &, const symmetric_key<key_size> &);

        ~session ();
    };

    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    struct writer final : message_writer<bytes, byte> {
        session<key_size, cipher, mode, direction> Session;
        buffer_writer Buffer;

        void write (const byte *, size_t size) final override;

        // add the final padding.
        bytes complete () final override;

        void update (const mode &);

        writer (const mode &, const symmetric_key<key_size> &);

    };

    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    void inline session<key_size, cipher, mode, direction>::write (const byte *b, size_t size) {
        Filter->Put (b, size);
    }

    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    void inline session<key_size, cipher, mode, direction>::complete () {
        Filter->MessageEnd ();
    }

    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    inline session<key_size, cipher, mode, direction>::~session () {
        Filter->Detach ();
    }

    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    inline session<key_size, cipher, mode, direction>::session
        (CryptoPP::BufferedTransformation *next_step, const mode &m, const symmetric_key<key_size> &k):
        Cipher {}, Mode {}, Filter {} {

        Cipher.SetKey (k.data (), k.size ());
        Filter = std::make_shared<CryptoPP::StreamTransformationFilter> (Mode, next_step, padding::NO_PADDING);

        if constexpr (requires { m.IV; }) {
            Mode.SetCipherWithIV (Cipher, m.IV.data (), m.IV.size ());
        } else {
            Mode.SetCipher (Cipher);
        }
    }

    // note: this won't work.
    template <size_t key_size, typename cipher, crypto::cipher::block::Mode<cipher, key_size> mode, dir direction>
    void inline session<key_size, cipher, mode, direction>::update (const mode &m) {
        auto next_step = Filter->AttachedTransformation ();
        Filter->Detach ();
        Filter = std::make_shared<CryptoPP::StreamTransformationFilter> (Mode, next_step, padding::NO_PADDING);
    }

}

#endif
