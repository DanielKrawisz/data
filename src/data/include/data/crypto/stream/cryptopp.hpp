// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_STREAM_CRYPTOPP
#define DATA_CRYPTO_STREAM_CRYPTOPP

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>

#include <cryptopp/chacha.h>
#include <cryptopp/salsa.h>
#include <cryptopp/hc128.h>
#include <cryptopp/hc256.h>
#include <cryptopp/panama.h>

#include <cryptopp/arc4.h>
#include <cryptopp/seal.h>
#include <cryptopp/wake.h>

#include <data/crypto/one_way.hpp>
#include <data/crypto/cipher.hpp>
#include <data/crypto/block/mode.hpp>
#include <data/crypto/block/padding.hpp>

#include <data/tools/lazy_writer.hpp>

namespace data::crypto::cipher::stream {

    template <std::derived_from<CryptoPP::StreamTransformation> cppstream>
    struct cryptopp_stream : data::reader<byte> {
        cppstream Stream;
        void read (byte *, size_t) final override;
        ~cryptopp_stream () noexcept (false) {}
    };

    struct XChaCha20_stream : cryptopp_stream<CryptoPP::XChaCha20::Encryption> {
        constexpr static const security Security = security::modern;
        XChaCha20_stream (const symmetric_key<32> &key, const initialization_vector<24> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), key.size (), iv.data (), 24);
        }
        ~XChaCha20_stream () noexcept (false) {}
    };

    struct XSalsa20_stream : cryptopp_stream<CryptoPP::XSalsa20::Encryption> {
        constexpr static const security Security = security::modern;
        XSalsa20_stream (const symmetric_key<32> &key, const initialization_vector<24> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), key.size (), iv.data (), 24);
        }
        ~XSalsa20_stream () noexcept (false) {}
    };

    struct Salsa20_stream : cryptopp_stream<CryptoPP::Salsa20::Encryption> {
        constexpr static const security Security = security::modern;
        Salsa20_stream (const symmetric_key<32> &key, const initialization_vector<8> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), key.size (), iv.data (), 8);
        }

        Salsa20_stream (const symmetric_key<16> &key, const initialization_vector<8> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), key.size (), iv.data (), 8);
        }
    };

    struct HC128_stream : cryptopp_stream<CryptoPP::HC128::Encryption> {
        constexpr static const security Security = security::modern;
        HC128_stream (const symmetric_key<16> &key, const initialization_vector<16> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), key.size (), iv.data (), 16);
        }
    };

    struct HC256_stream : cryptopp_stream<CryptoPP::HC256::Encryption> {
        constexpr static const security Security = security::modern;
        HC256_stream (const symmetric_key<32> &key, const initialization_vector<32> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), key.size (), iv.data (), 32);
        }
    };

    struct Panama_stream : cryptopp_stream<CryptoPP::PanamaCipher<CryptoPP::LittleEndian>::Encryption> {
        constexpr static const security Security = security::modern;
        Panama_stream (const symmetric_key<32> &key, const initialization_vector<32> &iv) {
            this->Stream.SetKeyWithIV (reinterpret_cast<const CryptoPP::byte*> (key.data ()), 32, iv.data (), 32);
        }
    };

    struct XChaCha20 {
        constexpr static const security Security = security::modern;
        initialization_vector<24> IV;
        XChaCha20_stream stream (const symmetric_key<32> &key);
    };

    struct XSalsa20 {
        constexpr static const security Security = security::modern;
        initialization_vector<24> IV;
        XSalsa20_stream stream (const symmetric_key<32> &key);
    };

    struct Salsa20 {
        constexpr static const security Security = security::modern;
        initialization_vector<8> IV;
        Salsa20_stream stream (const symmetric_key<32> &key);
    };

    struct HC128 {
        constexpr static const security Security = security::modern;
        initialization_vector<16> IV;
        HC128_stream stream (const symmetric_key<32> &key);
    };

    struct HC256 {
        constexpr static const security Security = security::modern;
        initialization_vector<32> IV;
        HC256_stream stream (const symmetric_key<32> &key);
    };

    struct Panama {
        constexpr static const security Security = security::modern;
        initialization_vector<32> IV;
        Panama_stream stream (const symmetric_key<32> &key);
    };
}

namespace data::crypto {

    template <typename stream, size_t key_size, size_t iv_size> requires
    std::derived_from<stream, CryptoPP::StreamTransformation> &&
    requires (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv) {
        { stream {key, iv} };
    } bytes encrypt (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv, const bytes &plaintext);

    template <typename stream, size_t key_size, size_t iv_size> requires
    std::derived_from<stream, CryptoPP::StreamTransformation> &&
    requires (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv) {
        { stream {key, iv} };
    } bytes decrypt (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv, const bytes &ciphertext);

    // encrypt and decrypt are the same for stream ciphers.
    template <typename stream, size_t key_size, size_t iv_size> requires
    std::derived_from<stream, CryptoPP::StreamTransformation> &&
    requires (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv) {
        { stream {key, iv} };
    } bytes crypt (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv, const bytes &plaintext);

    template <typename stream, size_t key_size, size_t iv_size> requires
    std::derived_from<stream, CryptoPP::StreamTransformation> &&
    requires (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv) {
        { stream {key, iv} };
    } bytes inline encrypt (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv, const bytes &plaintext) {
        return crypt<stream> (key, iv, plaintext);
    }

    template <typename stream, size_t key_size, size_t iv_size> requires
    std::derived_from<stream, CryptoPP::StreamTransformation> &&
    requires (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv) {
        { stream {key, iv} };
    } bytes inline decrypt (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv, const bytes &ciphertext) {
        return crypt<stream> (key, iv, ciphertext);
    }

    template <typename stream, size_t key_size, size_t iv_size> requires
    std::derived_from<stream, CryptoPP::StreamTransformation> &&
    requires (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv) {
        { stream {key, iv} };
    } bytes crypt (const symmetric_key<key_size> &key, const initialization_vector<iv_size> &iv, const bytes &input) {
        stream cipher {key, iv};
        bytes output (input.size ());

        cipher.ProcessData (
            reinterpret_cast<CryptoPP::byte*> (output.data ()),
            reinterpret_cast<const CryptoPP::byte*> (input.data ()),
            input.size ());

        return output;
    }

}

#endif
