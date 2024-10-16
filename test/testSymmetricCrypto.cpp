// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/block/cipher.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/MAC/SHA3.hpp>
#include <data/crypto/stream/one_way.hpp>

namespace data::crypto {

    // we don't test that things actually work, merely that all the functions are implemented and no erros are thrown.

    // this is the only block size we use.
    constexpr const size_t block_size = 16;

    // NOTE: need padding.
    // do nothing if the configuration is not valid.
    template <typename cipher, size_t block_size, size_t key_size, typename mode>
    void test_encryption_mode (mode &m, const symmetric_key<key_size> &key, const char *message) {};

    // we should be able to encrypt a message and decrypt it to the same thing.
    template <typename cipher, size_t block_size, size_t key_size, block_cipher_mode<cipher, block_size, key_size> mode>
    void test_encryption_mode (mode &m, const symmetric_key<key_size> &key, const char *message);

    template <size_t... X> struct keys : tuple<symmetric_key<X>...> {
        using tuple<symmetric_key<X>...>::tuple;

        template <size_t index, typename cipher, typename mode>
        void run_from (mode &m, const char *message) {
            auto &k = std::get<index> (static_cast<tuple<symmetric_key<X>...> &> (*this));
            test_encryption_mode<cipher, block_size> (m, k, message);

            constexpr size_t size = std::tuple_size<tuple<symmetric_key<X>...>>::value;
            if constexpr (index >= size) this->template run_from<index + 1, cipher, mode> (m, message);
        };

        template <typename cipher, typename mode>
        void run (mode &m, const char *message) {
            return this->template run_from<0, cipher, mode> (m, message);
        }
    };

    // here are the keys we will try using.
    // if a particular key is not supported for a block cipher, it will be skipped.
    keys<16, 20, 24, 28, 32, 40, 48, 56> kkk {
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e3"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e38d27c16b"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a1"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e70b61c72d83e94fa5"}},
        {encoding::hex::string {"0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e70b61c72d83e94fa50da741eb852fc963"}}};

    // first test is MACs. We have two kinds of MACs. SHA3 and HMAC.

    const char *MACMessage = "This is a message that will be hashed using a secret key to produce a MAC. Thank you.";

    template <typename w, size_t key_size> void test_MAC (const char *message) {
        w ww {std::get<symmetric_key<key_size>> (kkk)};
        ww << bytes (string {message});
        ww << end_message {};
    }

    template <typename w> struct test_MACs {
        test_MACs (const char *message) {
            test_MAC<w, 16> (message);
            test_MAC<w, 20> (message);
            test_MAC<w, 28> (message);
            test_MAC<w, 32> (message);
        }
    };

    // test for all hash functions we have
    TEST (TestSymmetricCrypto, TestHMAC) {

        test_MACs<MAC::HMAC_writer<CryptoPP::SHA1, 20>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD128, 16>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD160, 20>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD256, 32>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD320, 40>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA224, 28>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA256, 32>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA384, 48>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA512, 64>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<16>, 16>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<20>, 20>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<32>, 32>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<40>, 40>> {MACMessage};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<20>, 20>> {MACMessage};

    }

    template <size_t size> struct test_SHA3MAC {
        test_SHA3MAC (const char *message) {
            test_MAC<MAC::SHA3_MAC_writer<size, 16>, 16> (message);
            test_MAC<MAC::SHA3_MAC_writer<size, 20>, 20> (message);
            test_MAC<MAC::SHA3_MAC_writer<size, 28>, 28> (message);
            test_MAC<MAC::SHA3_MAC_writer<size, 32>, 32> (message);
        }
    };

    // can test with different sizes.
    TEST (TestSymmetricCrypto, TestSHA3MAC) {

        test_SHA3MAC<16> {MACMessage};
        test_SHA3MAC<20> {MACMessage};
        test_SHA3MAC<28> {MACMessage};
        test_SHA3MAC<32> {MACMessage};

    }

    byte_array<block_size> IV {encoding::hex::string {"0123456789abcdef0369cf258be147ad"}};

    template <typename ...X> struct ciphers;
    template <size_t ...x> struct key_sizes;

    template <typename X, typename x> struct test_output_feedback_stream;

    template <typename X, size_t x>
    requires block_cipher<X, block_size, x>
    struct test_output_feedback_stream<ciphers<X>, key_sizes<x>> {
        test_output_feedback_stream () {
            output_feedback_stream<block_size, x, X> (IV, std::get<symmetric_key<x>> (kkk)).crypt (0);
        };
    };

    template <typename X, size_t x>
    struct test_output_feedback_stream<ciphers<X>, key_sizes<x>> {
        test_output_feedback_stream () {};
    };

    template <typename X, size_t a, size_t b, size_t ...x>
    struct test_output_feedback_stream<ciphers<X>, key_sizes<a, b, x...>> :
        test_output_feedback_stream<ciphers<X>, key_sizes<a>>,
        test_output_feedback_stream<ciphers<X>, key_sizes<b, x...>> {
        test_output_feedback_stream ():
            test_output_feedback_stream<ciphers<X>, key_sizes<a>> {},
            test_output_feedback_stream<ciphers<X>, key_sizes<b, x...>> {} {}
    };

    template <typename A, typename B, typename ...X, size_t ...x>
    struct test_output_feedback_stream<ciphers<A, B, X...>, key_sizes<x...>> :
        test_output_feedback_stream<ciphers<A>, key_sizes<x...>>,
        test_output_feedback_stream<ciphers<B, X...>, key_sizes<x...>> {
        test_output_feedback_stream ():
            test_output_feedback_stream<ciphers<A>, key_sizes<x...>> {},
            test_output_feedback_stream<ciphers<B, X...>, key_sizes<x...>> {} {}
    };

    template <typename x> struct test_SHA3_stream;

    // right now we don't have different sized IVs, so we can only test key_size == block_size
    template <size_t x> requires (x == block_size) struct test_SHA3_stream<key_sizes<x>> {
        test_SHA3_stream () {
            SHA3_stream<block_size> (IV).crypt (0);
        }
    };

    template <size_t x> struct test_SHA3_stream<key_sizes<x>> {
        test_SHA3_stream () {}
    };

    template <size_t a, size_t b, size_t ...x>
    struct test_SHA3_stream<key_sizes<a, b, x...>> :
        test_SHA3_stream<key_sizes<a>>, test_SHA3_stream<key_sizes<b, x...>> {
        test_SHA3_stream (): test_SHA3_stream<key_sizes<a>> {}, test_SHA3_stream<key_sizes<b, x...>> {} {}
    };

    // can test with SHA3 and with all block ciphers using OFB
    TEST (TestSymmetricCrypto, TestOneWayStream) {
        test_output_feedback_stream<ciphers<Rijndael, Serpent, Twofish, RC6, MARS>, key_sizes<16, 20, 24, 28, 32, 40, 48, 56>> {};
        test_SHA3_stream<key_sizes<16, 20, 24, 28, 32, 40, 48, 56>> {};
    }

    // next we test block ciphers. WE don't have complete tests yet because we don't have padding.
    template <typename... M> struct modes : public tuple<M...> {
        using tuple<M...>::tuple;

        template <size_t index, typename cipher, typename keys>
        void run_from (keys &k, const char *message) {
            auto &elem = std::get<index> (static_cast<tuple<M...> &> (*this));
            k.template run<cipher> (elem, message);

            constexpr size_t size = std::tuple_size<tuple<M...>>::value;
            if constexpr (index >= size)
                this->template run_from<index + 1, cipher, keys> (k, message);
        };

        template <typename cipher, typename keys>
        void run (keys &k, const char *message) {
            return this->template run_from<0, cipher, keys> (k, message);
        }
    };

    template <typename... C> struct test_block_ciphers;

    template <typename c, typename... C>
    struct test_block_ciphers<c, C...> :
        test_block_ciphers<c>, test_block_ciphers<C...> {

        template <typename modes, typename keys>
        void run (modes &m, keys &k, const char *plaintext) const {
            test_block_ciphers<c>::run (m, k, plaintext);
            test_block_ciphers<C...>::run (m, k, plaintext);
        }
    };

    template <typename c>
    struct test_block_ciphers<c> {

        template <typename modes, typename keys>
        void run (modes &m, keys &k, const char *plaintext) const {
            m.template run<c> (k, plaintext);
        }
    };

    const char *Plaintext = "This is a message that will be encrypted using various keys, modes, and padding schemes.";

    // we have four working modes now.
    // we should be able to encrypt and decrypt the message to the same thing.
    TEST (TestSymmetricCrypto, TestEncryptionModes) {

        modes<ECB<block_size>, CBC<block_size>, OFB<block_size>, CTR<block_size, endian::big>> m {{}, {IV}, {IV}, {uint_big<block_size> {0}}};

        test_block_ciphers<
            Rijndael,
            Serpent,
            Twofish,
            RC6,
            MARS> {}.run (m, kkk, Plaintext);

    }

}
