// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/block.hpp>
#include <data/crypto/block/cryptopp.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/MAC/SHA3.hpp>
#include <data/crypto/stream/one_way.hpp>

#include <type_traits>
#include <cstddef>

template <std::size_t I, typename... Ts>
struct get_type;

template <typename T, typename... Ts>
struct get_type<0, T, Ts...> {
    using type = T;
};

template <std::size_t I, typename T, typename... Ts>
struct get_type<I, T, Ts...> {
    using type = typename get_type<I - 1, Ts...>::type;
};

template <std::size_t I, auto... Ts>
struct get_value;

template <auto T, auto... Ts>
struct get_value<0, T, Ts...> {
    constexpr static const auto value = T;
};

template <std::size_t I, auto T, auto... Ts>
struct get_value<I, T, Ts...> {
    constexpr static const auto value = get_value<I - 1, Ts...>::value;
};

namespace data::crypto {

    // we don't test that things actually work, merely that all the functions are implemented and no errors are thrown.

    template <size_t... X> struct keys : tuple<symmetric_key<X>...> {
        using parent = tuple<symmetric_key<X>...>;
        using typename parent::tuple;
    };

    template <size_t size> symmetric_key<size> read_key (const std::string &x) {
        symmetric_key<size> k;
        boost::algorithm::unhex (x.begin (), x.end (), k.begin ());
        return k;
    }

    // here are the keys we will try using.
    // if a particular key is not supported for a block cipher, it will be skipped.
    keys<16, 20, 24, 28, 32, 40, 48, 56> kkk {
        read_key<16> ("0123456789abcdef0369cf258be147ad"),
        read_key<20> ("0123456789abcdef0369cf258be147ad05af49e3"),
        read_key<24> ("0123456789abcdef0369cf258be147ad05af49e38d27c16b"),
        read_key<28> ("0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a1"),
        read_key<32> ("0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29"),
        read_key<40> ("0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"),
        read_key<48> ("0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e70b61c72d83e94fa5"),
        read_key<56> ("0123456789abcdef0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e70b61c72d83e94fa50da741eb852fc963")};

    // first test is MACs. We have two kinds of MACs. SHA3 and HMAC.
    const char *MACMessage = "This is a message that will be hashed using a secret key to produce a MAC. Thank you.";
    const char *MACAltered = "This is x message thxt will be hxshed using x secret key to produce x MAC. Thxnk you.";

    template <typename w, size_t key_size> void test_MAC (const char *message1, const char *message2) {
        w ww {std::get<symmetric_key<key_size>> (kkk)};
        w wv {std::get<symmetric_key<key_size>> (kkk)};
        ww << bytes (string {message1});
        wv << bytes (string {message2});
        EXPECT_NE (ww << end_message {}, wv << end_message {});
    }

    template <typename w> struct test_MACs {
        test_MACs (const char *message, const char *altered) {
            test_MAC<w, 16> (message, altered);
            test_MAC<w, 20> (message, altered);
            test_MAC<w, 28> (message, altered);
            test_MAC<w, 32> (message, altered);
        }
    };

    // test for all hash functions we have
    TEST (SymmetricCrypto, HMAC) {

        // Note: should use the versions of the hash functions defined in data, not cryptopp.
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA1, 20>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD128, 16>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD160, 20>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD256, 32>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::RIPEMD320, 40>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA224, 28>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA256, 32>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA384, 48>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA512, 64>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<16>, 16>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<20>, 20>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<32>, 32>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<40>, 40>> {MACMessage, MACAltered};
        test_MACs<MAC::HMAC_writer<CryptoPP::SHA3_Final<20>, 20>> {MACMessage, MACAltered};

    }

    // NOTE: this should be KMAC
    template <size_t size> struct test_SHA3MAC {
        test_SHA3MAC (const char *m1, const char *m2) {
            test_MAC<MAC::SHA3_MAC_writer<size, 16>, 16> (m1, m2);
            test_MAC<MAC::SHA3_MAC_writer<size, 20>, 20> (m1, m2);
            test_MAC<MAC::SHA3_MAC_writer<size, 28>, 28> (m1, m2);
            test_MAC<MAC::SHA3_MAC_writer<size, 32>, 32> (m1, m2);
        }
    };

    // can test with different sizes.
    TEST (SymmetricCrypto, SHA3MAC) {

        test_SHA3MAC<16> {MACMessage, MACAltered};
        test_SHA3MAC<20> {MACMessage, MACAltered};
        test_SHA3MAC<28> {MACMessage, MACAltered};
        test_SHA3MAC<32> {MACMessage, MACAltered};

    }

    // this is the only block size we use.
    constexpr const size_t block_size = 16;

    // next we test block ciphers.
    template <typename...> struct ciphers {};
    ciphers<Rijndael, Serpent, Twofish, MARS, RC6> supported_ciphers {};

    byte_array<block_size> IV1 = read_byte_array<block_size> (hex_string {"0123456789abcdef0369cf258be147ad"});

    byte_array<block_size> IV2 = read_byte_array<block_size> (hex_string {"abcdef01234567890000000000000000"});

    // modes of operation that we support and that can be used as block ciphers.
    tuple<ECB<block_size>, CBC<block_size>> block_modes {{}, {IV1}};

    template <block_padding_scheme...> struct padding {};

    // we should be able to encrypt a message and decrypt it to the same thing.
    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_and_decrypt (const mode &m, const symmetric_key<key_size> &key, block_padding_scheme p, const bytes &message) {
        bytes decrypted = decrypt<cipher, block_size> (encrypt<cipher, block_size> (message, m, key, p), m, key, p);
        EXPECT_EQ (message, decrypted) << "tried to decrypt to " << message << " but got " << decrypted;
    };

    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_change_key (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        block_padding_scheme p, const bytes &message) {

        bytes en1 = encrypt<cipher, block_size> (message, m, k1, p);
        bytes en2 = encrypt<cipher, block_size> (message, m, k2, p);

        EXPECT_NE (en1, en2);

        // TODO we should try to decrypt with the wrong key and get an error.
    };

    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_change_padding (mode &m,
        const symmetric_key<key_size> &key,
        block_padding_scheme p1, block_padding_scheme p2, const bytes &message) {

        bytes en1 = encrypt<cipher, block_size> (message, m, key, p1);
        bytes en2 = encrypt<cipher, block_size> (message, m, key, p2);

        EXPECT_NE (en1, en2);

        // TODO we should try to decrypt with the wrong padding and get an error.
    };

    // do nothing if the configuration is not valid.
    template <typename cipher, size_t key_size, typename mode>
    void test_block_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        block_padding_scheme p1,
        block_padding_scheme p2,
        const bytes &message) {};

    template <typename cipher, size_t key_size,
        block_cipher_mode<cipher, block_size, key_size> mode>
    void test_block_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        block_padding_scheme p1,
        block_padding_scheme p2,
        const bytes &message) {

        test_encrypt_and_decrypt<cipher> (m, k1, p1, message);
        test_encrypt_and_decrypt<cipher> (m, k2, p2, message);

        test_encrypt_change_key<cipher> (m, k1, k2, p1, message);
        test_encrypt_change_padding<cipher> (m, k1, p1, p2, message);

    }

    template <size_t key_size> symmetric_key<key_size> mutate_key (const symmetric_key<key_size> &k) {
        symmetric_key<key_size> new_key = k;
        new_key[0] = ~new_key[0];
        return new_key;
    }

    template <size_t cipher_index, size_t mode_index, size_t key_index,
        typename... cph, typename... modes, size_t... key_sizes>
    void test_block_ciphers_for (ciphers<cph...> c, tuple<modes...> m, keys<key_sizes...> k, const bytes &msg) {
        if constexpr (cipher_index >= sizeof...(cph)) return;
        else if constexpr (mode_index >= std::tuple_size<tuple<modes...>>::value) {
            test_block_ciphers_for<cipher_index + 1, 0, 0> (c, m, k, msg);
        } else if constexpr (key_index >= std::tuple_size<typename keys<key_sizes...>::parent>::value) {
            test_block_ciphers_for<cipher_index, mode_index + 1, 0> (c, m, k, msg);
        } else {

            auto key = std::get<key_index> (k);
            auto key_size = get_value<key_index, key_sizes...>::value;
            using cipher = typename get_type<cipher_index, cph...>::type;
            test_block_cipher<cipher> (std::get<mode_index> (m), key, mutate_key (key),
                block_padding::PKCS_PADDING, block_padding::ONE_AND_ZEROS_PADDING, msg);

            test_block_ciphers_for<cipher_index, mode_index, key_index + 1> (c, m, k, msg);
        }
    }

    template <typename... cph, typename... modes, size_t... key_sizes>
    void test_block_ciphers (ciphers<cph...> c, tuple<modes...> m, keys<key_sizes...> k, const bytes &msg) {
        test_block_ciphers_for<0, 0, 0> (c, m, k, msg);
    }

    TEST (SymmetricCrypto, BlockCipher) {

        test_block_ciphers (supported_ciphers, block_modes, kkk, bytes {string {""}});
        test_block_ciphers (supported_ciphers, block_modes, kkk, bytes {string {"a"}});
        test_block_ciphers (supported_ciphers, block_modes, kkk,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}});

    }

    template <size_t index, typename cipher, size_t block_size, typename mode, typename keys>
    void run_from (mode &m, const keys &k, const std::string &message) {
        test_encryption_mode<cipher, block_size> (m, std::get<index> (k), message);

        constexpr size_t size = std::tuple_size<typename keys::parent>::value;
        if constexpr (index >= size) run_from<index + 1, keys, cipher, mode> (m, k, message);
    };

    template <typename cipher, size_t block_size, typename mode, typename keys>
    void run (mode &m, const keys &k, const std::string &message) {
        return run_from<0, cipher, block_size> (m, k, message);
    }

    // for this test, we will encrypt and decrypt messages using a block cipher with padding.

    // Here we test that we can change the IV and that decryption will fail.

    // Here we test that we can change the padding scheme and that the decryption will fail when we use a MAC.

    template <typename ...X> struct ciphers;
    template <size_t ...x> struct key_sizes;

    template <typename X, typename x> struct test_output_feedback_stream;

    template <typename X, size_t x>
    requires block_cipher<X, block_size, x>
    struct test_output_feedback_stream<ciphers<X>, key_sizes<x>> {
        test_output_feedback_stream () {
            output_feedback_stream<block_size, x, X> (IV1, std::get<symmetric_key<x>> (kkk)).crypt (0);
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
            SHA3_stream<block_size> (IV1).crypt (0);
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
    TEST (SymmetricCrypto, OneWayStream) {
        test_output_feedback_stream<ciphers<Rijndael, Serpent, Twofish, RC6, MARS>, key_sizes<16, 20, 24, 28, 32, 40, 48, 56>> {};
        test_SHA3_stream<key_sizes<16, 20, 24, 28, 32, 40, 48, 56>> {};
    }

}
