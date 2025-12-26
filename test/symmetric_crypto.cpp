// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/hash.hpp>
#include <data/crypto/block/cryptopp.hpp>
#include <data/crypto/block.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/MAC/SHA3.hpp>
#include <data/tuple.hpp>
#include <data/io/log.hpp>

#include <type_traits>
#include <cstddef>

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

    template <MAC::Writer w, size_t key_size> void test_MAC (const char *message1, const char *message2) {
        typename w::digest D1;
        typename w::digest D2;
        {
            w ww {D1, std::get<symmetric_key<key_size>> (kkk)};
            w wv {D2, std::get<symmetric_key<key_size>> (kkk)};
            ww << bytes (string {message1});
            wv << bytes (string {message2});
        }
        EXPECT_NE (D1, D2);
    }

    template <MAC::Writer w> struct test_MACs {
        test_MACs (const char *message, const char *altered) {
            test_MAC<w, 16> (message, altered);
            test_MAC<w, 20> (message, altered);
            test_MAC<w, 28> (message, altered);
            test_MAC<w, 32> (message, altered);
        }
    };
    template <hash::Engine e> void HMAC_test_case (
        const bytes &key,
        const string &msg,
        const hash::digest<e::DigestSize> &result) {
        bytes msg_bytes (msg);
        auto got = MAC::calculate<HMAC<e>> (key, msg_bytes);
        EXPECT_EQ (got, result) << "HMAC key " << key << " and msg " << msg << "; expected " << result << "; got " << got;
    }

    // test for all hash functions we have
    TEST (SymmetricCrypto, HMAC) {

        test_MACs<MAC::writer<HMAC<hash::SHA1>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::RIPEMD<16>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::RIPEMD<20>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::RIPEMD<32>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::RIPEMD<40>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::SHA2<28>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::SHA2<32>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::SHA2<48>>>> {MACMessage, MACAltered};
        test_MACs<MAC::writer<HMAC<hash::SHA2<64>>>> {MACMessage, MACAltered};

        HMAC_test_case<hash::SHA1> (bytes (20, 0x0b), "Hi There",
            hash::digest<20> {"b617318655057264e28bc0b6fb378c8ef146be00"});

        HMAC_test_case<hash::SHA1> (bytes (string ("Jefe")), "what do ya want for nothing?",
            hash::digest<20> {"effcdf6ae5eb2fa2d27416d5f184df9c259a7c79"});

        HMAC_test_case<hash::SHA1> (bytes (80, 0xaa), "Test Using Larger Than Block-Size Key - Hash Key First",
            hash::digest<20> {"aa4ae5e15272d00e95705637ce8a3b55ed402112"});

        HMAC_test_case<hash::SHA2<32>> (bytes (20, 0x0b), "Hi There",
            hash::digest<32> {"b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7"});

        HMAC_test_case<hash::SHA2<32>> (bytes (string ("Jefe")), "what do ya want for nothing?",
            hash::digest<32> {"5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843"});

        HMAC_test_case<hash::SHA2<32>> (bytes (string ("key")), "",
            hash::digest<32> {"5d5d139563c95b5967b9bd9a8c9b233a9dedb45072794cd232dc1b74832607d0"});

    }

    // NOTE: this should be KMAC
    template <size_t size> struct test_SHA3MAC {
        test_SHA3MAC (const char *m1, const char *m2) {
            test_MAC<MAC::SHA3_MAC_writer<size>, 16> (m1, m2);
            test_MAC<MAC::SHA3_MAC_writer<size>, 20> (m1, m2);
            test_MAC<MAC::SHA3_MAC_writer<size>, 28> (m1, m2);
            test_MAC<MAC::SHA3_MAC_writer<size>, 32> (m1, m2);
        }
    };

    // can test with different sizes.
    TEST (SymmetricCrypto, SHA3MAC) {

        test_SHA3MAC<16> {MACMessage, MACAltered};
        test_SHA3MAC<20> {MACMessage, MACAltered};
        test_SHA3MAC<28> {MACMessage, MACAltered};
        test_SHA3MAC<32> {MACMessage, MACAltered};

    }
}

namespace data::crypto::cipher::block {

    // Invalid key and block sizes for a given algorithm should
    // fail to compile, and valid ones should succeed.

    template <typename Cipher, size_t block_size, size_t key_size>
    concept TestEncryptable = requires (bytes msg, symmetric_key<key_size> key) {
        data::crypto::encrypt<Cipher, block_size> (msg, ECB<block_size> {}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::decrypt<Cipher, block_size> (msg, ECB<block_size> {}, key, padding_scheme::DEFAULT_PADDING);
    } && requires (bytes msg, symmetric_key<key_size> key, byte_array<block_size> iv) {
        data::crypto::encrypt<Cipher, block_size> (msg, CBC<block_size> {iv}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::decrypt<Cipher, block_size> (msg, CBC<block_size> {iv}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::encrypt<Cipher, block_size> (msg, CFB<block_size> {iv}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::decrypt<Cipher, block_size> (msg, CFB<block_size> {iv}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::encrypt<Cipher, block_size> (msg, OFB<block_size> {iv}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::decrypt<Cipher, block_size> (msg, OFB<block_size> {iv}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::encrypt<Cipher, block_size> (msg,
            CTR<block_size, endian::little> {math::uint_little<block_size, byte> {iv}}, key, padding_scheme::DEFAULT_PADDING);
        data::crypto::decrypt<Cipher, block_size> (msg,
            CTR<block_size, endian::little> {math::uint_little<block_size, byte> {iv}}, key, padding_scheme::DEFAULT_PADDING);
    };

    static_assert (TestEncryptable<DES, 8, 7>);
    static_assert (TestEncryptable<DES, 8, 8>);
    static_assert (!TestEncryptable<DES, 16, 8>);
    static_assert (!TestEncryptable<DES, 8, 16>);

    static_assert (TestEncryptable<TripleDES, 8, 14>);
    static_assert (TestEncryptable<TripleDES, 8, 16>);
    static_assert (!TestEncryptable<TripleDES, 16, 16>);
    static_assert (!TestEncryptable<TripleDES, 8, 32>);

    static_assert (TestEncryptable<AES, 16, 16>);
    static_assert (!TestEncryptable<AES, 16, 8>);
    static_assert (!TestEncryptable<AES, 8, 16>);
    static_assert (!TestEncryptable<AES, 16, 20>);
    static_assert (TestEncryptable<AES, 16, 24>);
    static_assert (TestEncryptable<AES, 16, 32>);
    static_assert (!TestEncryptable<AES, 16, 56>);

    static_assert (TestEncryptable<Serpent, 16, 16>);
    static_assert (!TestEncryptable<Serpent, 16, 8>);
    static_assert (!TestEncryptable<Serpent, 8, 16>);
    static_assert (!TestEncryptable<Serpent, 16, 20>);
    static_assert (TestEncryptable<Serpent, 16, 24>);
    static_assert (TestEncryptable<Serpent, 16, 32>);
    static_assert (!TestEncryptable<Serpent, 16, 56>);

    static_assert (TestEncryptable<Twofish, 16, 16>);
    static_assert (!TestEncryptable<Twofish, 16, 8>);
    static_assert (!TestEncryptable<Twofish, 8, 16>);
    static_assert (!TestEncryptable<Twofish, 16, 20>);
    static_assert (TestEncryptable<Twofish, 16, 24>);
    static_assert (TestEncryptable<Twofish, 16, 32>);
    static_assert (!TestEncryptable<Twofish, 16, 56>);

    static_assert (TestEncryptable<RC6, 16, 16>);
    static_assert (!TestEncryptable<RC6, 16, 8>);
    static_assert (!TestEncryptable<RC6, 8, 16>);
    static_assert (!TestEncryptable<RC6, 16, 20>);
    static_assert (TestEncryptable<RC6, 16, 24>);
    static_assert (TestEncryptable<RC6, 16, 32>);
    static_assert (!TestEncryptable<RC6, 16, 56>);

    static_assert (TestEncryptable<MARS, 16, 16>);
    static_assert (!TestEncryptable<MARS, 16, 8>);
    static_assert (!TestEncryptable<MARS, 8, 16>);
    static_assert (!TestEncryptable<MARS, 16, 20>);
    static_assert (TestEncryptable<MARS, 16, 24>);
    static_assert (TestEncryptable<MARS, 16, 32>);
    static_assert (TestEncryptable<MARS, 16, 56>);

    template <typename mode, size_t key_size> struct AES_test_case {
        AES_test_case (string key, string IV, string plaintext, string ciphertext) {
            byte_array<key_size> k {encoding::hex::fixed<key_size> (key)};
            byte_array<16> iv {encoding::hex::fixed<16> (IV)};
            DATA_LOG (normal) << "AES test case with key " << k;

            {

                byte_array<16> pt {encoding::hex::fixed<16> (plaintext)};
                byte_array<16> ct {encoding::hex::fixed<16> (ciphertext)};

                mode encryptor {iv};
                DATA_LOG (normal) << "try to encrypt ";
                EXPECT_EQ ((encrypt<AES, key_size, mode> (encryptor, k, pt)), ct);
                mode decryptor {iv};

                DATA_LOG (normal) << "try to decrypt";
                auto decrypted = decrypt<AES, key_size, mode> (decryptor, k, ct);
                EXPECT_EQ (decrypted, pt) << "Expected " << ct << " to decrypt to " <<
                    pt << " using key " << k << " and IV " << iv << " but got " << decrypted;
            }

            {

                DATA_LOG (normal) << "phase 2";
                mode m {iv};
                bytes pt = *encoding::hex::read (plaintext);

                DATA_LOG (normal) << "try to encrypt";
                // we test this against the function for encrypting whole messages rather than just blocks.
                auto padded = data::crypto::encrypt<AES, 16> (pt, m, k);

                // we should expect the encrypted message to have a whole block of padding.
                EXPECT_EQ (padded.size (), pt.size () + 16);

                DATA_LOG (normal) << "try to decrypt";
                bytes ct = *encoding::hex::read (ciphertext);
                EXPECT_EQ (ct, (take (padded, pt.size ())));

                auto decrypted = data::crypto::decrypt<AES, 16> (padded, m, k);

                EXPECT_EQ (pt, decrypted);

            }
        }
    };

    template <size_t key_size> struct AES_test_case<ECB<16>, key_size> {
        AES_test_case (string key, string plaintext, string ciphertext) {
            byte_array<key_size> k {encoding::hex::fixed<key_size> (key)};
            {
                byte_array<16> pt {encoding::hex::fixed<16> (plaintext)};
                byte_array<16> ct {encoding::hex::fixed<16> (ciphertext)};
                ECB<16> encryptor {};
                EXPECT_EQ ((encrypt<AES, key_size, ECB<16>> (encryptor, k, pt)), ct);
                ECB<16> decryptor {};
                EXPECT_EQ ((decrypt<AES, key_size, ECB<16>> (decryptor, k, ct)), pt);
            }

            {

                ECB<16> m {};
                bytes pt = *encoding::hex::read (plaintext);

                // we test this against the function for encrypting whole messages rather than just blocks.
                auto padded = data::crypto::encrypt<AES, 16> (pt, m, k);

                // we should expect the encrypted message to have a whole block of padding.
                EXPECT_EQ (padded.size (), pt.size () + 16);

                bytes ct = *encoding::hex::read (ciphertext);
                EXPECT_EQ (ct, (take (padded, pt.size ())));

                auto decrypted = data::crypto::decrypt<AES, 16> (padded, m, k);

                EXPECT_EQ (pt, decrypted);

            }
        }
    };

    // A series of tests for each block mode with AES to test that the block modes are correct.
    TEST (BlockCipher, AESECB) {

        AES_test_case<ECB<16>, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "3ad78e726c1ec02b7ebfe92b23d9ec34"};

        AES_test_case<ECB<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "6cd02513e8d4dc986b4afe087a60bd0c"};

        AES_test_case<ECB<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "ddc6bf790c15760d8d9aeb6f9a75fd4e"};

        AES_test_case<ECB<16>, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "7888beae6e7a426332a7eaa2f808e637"};

        AES_test_case<ECB<16>, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "24f40c4eecd9c49825000fcb4972647a"};

        AES_test_case<ECB<16>, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "df8634ca02b13a125b786e1dce90658b"};

    }

    TEST (BlockCipher, AESCBC) {
        AES_test_case<CBC<16>, 16> {
            "00000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "3ad78e726c1ec02b7ebfe92b23d9ec34"};

        AES_test_case<CBC<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "6cd02513e8d4dc986b4afe087a60bd0c"};

        AES_test_case<CBC<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "ddc6bf790c15760d8d9aeb6f9a75fd4e"};

        AES_test_case<CBC<16>, 16> {
            "1f8e4973953f3fb0bd6b16662e9a3c17",
            "2fe2b333ceda8f98f4a99b40d2cd34a8",
            "45cf12964fc824ab76616ae2f4bf0822",
            "0f61c4d44c5147c03c195ad7e2cc12b2"};

        AES_test_case<CBC<16>, 24> {
            "ba75f4d1d9d7cf7f551445d56cc1a8ab2a078e15e049dc2c",
            "531ce78176401666aa30db94ec4a30eb",
            "c51fc276774dad94bcdc1d2891ec8668",
            "70dd95a14ee975e239df36ff4aee1d5d"};

        AES_test_case<CBC<16>, 32> {
            "6ed76d2d97c69fd1339589523931f2a6cff554b15f738f21ec72dd97a7330907",
            "851e8764776e6796aab722dbb644ace8",
            "6282b8c05c5c1530b97d4816ca434762",
            "6acc04142e100a65f51b97adf5172c41"};

    }

    // we don't have CTR because according to NIST, we cannot
    // have automated tests for CTR because of the requirement
    // to start with a unique value every time we use it.

    // TODO get test cases with CTR using cryptopp

    // this is the only block size we use.
    constexpr const size_t block_size = 16;

    // next we test block ciphers.
    template <typename...> struct ciphers {};
    ciphers<Rijndael, Serpent, Twofish, MARS, RC6> supported_ciphers {};

    byte_array<block_size> IV1 = byte_array<block_size> (encoding::hex::fixed<block_size> {"0123456789abcdef0369cf258be147ad"});

    byte_array<block_size> IV2 = byte_array<block_size> (encoding::hex::fixed<block_size> {"abcdef01234567890000000000000000"});

    // modes of operation that we support and that can be used as block ciphers.
    tuple<ECB<block_size>, CBC<block_size>> block_modes {{}, {IV1}};

    // we should be able to encrypt a message and decrypt it to the same thing.
    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_and_decrypt (const mode &m, const symmetric_key<key_size> &key, padding_scheme p, const bytes &message) {
        bytes decrypted = data::crypto::decrypt<cipher, block_size> (data::crypto::encrypt<cipher, block_size> (message, m, key, p), m, key, p);
        EXPECT_EQ (message, decrypted) << "tried to decrypt to " << message << " but got " << decrypted;
    };

    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_change_key (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        padding_scheme p, const bytes &message) {

        bytes en1 = data::crypto::encrypt<cipher, block_size> (message, m, k1, p);
        bytes en2 = data::crypto::encrypt<cipher, block_size> (message, m, k2, p);

        EXPECT_NE (en1, en2);

        // TODO we should try to decrypt with the wrong key and get an error.
    };

    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_change_padding (mode &m,
        const symmetric_key<key_size> &key,
        padding_scheme p1, padding_scheme p2, const bytes &message) {

        bytes en1 = data::crypto::encrypt<cipher, block_size> (message, m, key, p1);
        bytes en2 = data::crypto::encrypt<cipher, block_size> (message, m, key, p2);

        EXPECT_NE (en1, en2);

        // TODO we should try to decrypt with the wrong padding and get an error.
    };

    // do nothing if the configuration is not valid.
    template <typename cipher, size_t key_size, typename mode>
    void test_block_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        padding_scheme p1,
        padding_scheme p2,
        const bytes &message) {};

    template <typename cipher, size_t key_size,
        Mode<cipher, key_size> mode>
    void test_block_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        padding_scheme p1,
        padding_scheme p2,
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
            auto key_size = meta::get_value<key_index, key_sizes...>::value;
            using cipher = typename meta::get_type<cipher_index, cph...>::type;
            test_block_cipher<cipher> (std::get<mode_index> (m), key, mutate_key (key),
                padding::PKCS_PADDING, padding::ONE_AND_ZEROS_PADDING, msg);

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
}
