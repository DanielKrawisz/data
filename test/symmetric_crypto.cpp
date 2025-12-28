// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/hash.hpp>
#include <data/crypto/block/cryptopp.hpp>
#include <data/crypto/block.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/MAC/SHA3.hpp>
#include <data/crypto/stream/cryptopp.hpp>
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
    keys<8, 16, 20, 24, 28, 32, 40, 48, 56> test_keys {
        read_key<8>  ("0123456789abcdef"),
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
            w ww {D1, std::get<symmetric_key<key_size>> (test_keys)};
            w wv {D2, std::get<symmetric_key<key_size>> (test_keys)};
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
    TEST (MAC, HMAC) {

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

    // TODO test KMAC
}

namespace data::crypto::cipher::block {

    // Invalid key and block sizes for a given algorithm should
    // fail to compile, and valid ones should succeed.

    template <typename Cipher, size_t block_size, size_t key_size>
    concept TestEncryptable = requires (bytes msg, symmetric_key<key_size> key) {
        encrypt<Cipher> (block_cipher {ECB<block_size> {}}, key, msg);
        decrypt<Cipher> (block_cipher {ECB<block_size> {}}, key, msg);
    } && requires (bytes msg, symmetric_key<key_size> key, byte_array<block_size> iv) {
        encrypt<Cipher> (block_cipher {CBC<block_size> {iv}}, key, msg);
        decrypt<Cipher> (block_cipher {CBC<block_size> {iv}}, key, msg);
        encrypt<Cipher> (block_cipher {CFB<block_size> {iv}}, key, msg);
        decrypt<Cipher> (block_cipher {CFB<block_size> {iv}}, key, msg);
        encrypt<Cipher> (block_cipher {OFB<block_size> {iv}}, key, msg);
        decrypt<Cipher> (block_cipher {OFB<block_size> {iv}}, key, msg);
        encrypt<Cipher> (block_cipher {CTR<block_size, endian::little> {math::uint_little<block_size, byte> {iv}}}, key, msg);
        decrypt<Cipher> (block_cipher {CTR<block_size, endian::little> {math::uint_little<block_size, byte> {iv}}}, key, msg);
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

    template <typename alg, size_t key_size, typename mode>
    void test_block_cipher (string key, mode m, string plaintext, string ciphertext) {
        byte_array<key_size> k {encoding::hex::fixed<key_size> (key)};

        {

            byte_array<16> pt {encoding::hex::fixed<16> (plaintext)};
            byte_array<16> ct {encoding::hex::fixed<16> (ciphertext)};

            mode encryptor = m;
            EXPECT_EQ ((encrypt<alg, key_size, mode> (encryptor, k, pt)), ct);
            mode decryptor = m;
            auto decrypted = decrypt<alg, key_size, mode> (decryptor, k, ct);
            EXPECT_EQ (decrypted, pt) << "Expected " << ct << " to decrypt to " <<
                pt << " using key " << k << " but got " << decrypted;
        }

        // since phase two of the test adds and checks padding, it proves that
        // the block mode correctly inverts itself.
        {

            bytes pt = *encoding::hex::read (plaintext);

            // we test this against the function for encrypting whole messages rather than just blocks.
            auto padded = data::crypto::encrypt<alg> (block_cipher {m}, k, pt);

            // we should expect the encrypted message to have a whole block of padding.
            EXPECT_EQ (padded.size (), pt.size () + 16);

            bytes ct = *encoding::hex::read (ciphertext);
            EXPECT_EQ (ct, (take (padded, pt.size ())));

            auto decrypted = data::crypto::decrypt<alg> (block_cipher {m}, k, padded);

            EXPECT_EQ (pt, decrypted);

        }
    }

    template <typename alg, typename mode, size_t key_size> struct block_test_case {
        block_test_case (string key, string IV, string plaintext, string ciphertext) {
            test_block_cipher<alg, key_size> (key, mode {byte_array<16> {encoding::hex::fixed<16> (IV)}}, plaintext, ciphertext);
        }
    };

    template <typename alg, size_t block_size, size_t key_size> struct block_test_case<alg, ECB<block_size>, key_size> {
        block_test_case (string key, string plaintext, string ciphertext) {
            test_block_cipher<alg, key_size> (key, ECB<block_size> {}, plaintext, ciphertext);
        }
    };

    template <typename alg, size_t block_size, size_t key_size> struct block_test_case<alg, CTR<block_size, endian::big>, key_size> {
        block_test_case (string key, string IV, string plaintext, string ciphertext) {
            test_block_cipher<alg, key_size> (key,
                CTR<block_size, endian::big> {math::uint_big<16, byte> {byte_array<16> {encoding::hex::fixed<16> (IV)}}}, plaintext, ciphertext);
        }
    };

    // A series of tests for each block mode with AES to test that the block modes are correct.
    TEST (BlockCipher, ECB) {

        // Since ECB doesn't really do anything, this test
        // proves that the underlying cipher works correctly.
        block_test_case<AES, ECB<16>, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "3ad78e726c1ec02b7ebfe92b23d9ec34"};

        block_test_case<AES, ECB<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "6cd02513e8d4dc986b4afe087a60bd0c"};

        block_test_case<AES, ECB<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "ddc6bf790c15760d8d9aeb6f9a75fd4e"};

        block_test_case<AES, ECB<16>, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "7888beae6e7a426332a7eaa2f808e637"};

        block_test_case<AES, ECB<16>, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "24f40c4eecd9c49825000fcb4972647a"};

        block_test_case<AES, ECB<16>, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "df8634ca02b13a125b786e1dce90658b"};

        block_test_case<Twofish, ECB<16>, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "73b9ff14cf2589901ff52a0d6f4b7ede"};

        block_test_case<Serpent, ECB<16>, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "a3b35de7c358ddd82644678c64b8bcbb"};

        block_test_case<MARS, ECB<16>, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "d962efeba75817bf6ed24bbbb5b7820d"};

        block_test_case<RC6, ECB<16>, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "f71f65e7b80c0c6966fee607984b5cdf"};

        block_test_case<Twofish, ECB<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "62ef193edb7d399aca50ec1cbe5398d8"};

        block_test_case<Serpent, ECB<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "23f5f432ad687e0d4574c16459618abb"};

        block_test_case<MARS, ECB<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "4433fccbf2d4aa14783835d0968bb7a6"};

        block_test_case<RC6, ECB<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "a600b72aac4dfb5271dc45f563fae428"};

        block_test_case<Twofish, ECB<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "23a385f617f313dac05bcb7eabd61807"};

        block_test_case<Serpent, ECB<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "8314675e8ad5c3ecd83d852bcf7f566e"};

        block_test_case<MARS, ECB<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "7dec45d013733a2fc3acb05ac5edcda0"};

        block_test_case<RC6, ECB<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "30f5c183f6c81a72f39b618b086cc2de"};

        block_test_case<Twofish, ECB<16>, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "1c0bda78c854cf6b6f7e2b0095659b66"};

        block_test_case<Serpent, ECB<16>, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "60da3514a0dd32237bdd3546bef56093"};

        block_test_case<MARS, ECB<16>, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "540848258fc6da64af9410878465d495"};

        block_test_case<RC6, ECB<16>, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "b8414291d911c6a9d97fc3b49e8c3253"};

        block_test_case<Twofish, ECB<16>, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "b9d2d9a16b495e35c7131c502e457543"};

        block_test_case<Serpent, ECB<16>, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "915730610cc1f93f2ab72db112655f9e"};

        block_test_case<MARS, ECB<16>, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "cb24760bf14501e4134c8eda2f8a0a59"};

        block_test_case<RC6, ECB<16>, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "9bd0b1c332e51586ea6888dc6543dcad"};

        block_test_case<Twofish, ECB<16>, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "0df2210c88a3aa8fe59a0ac0cf8e80e7"};

        block_test_case<Serpent, ECB<16>, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "998b13f6702ace671a59c117524033a1"};

        block_test_case<MARS, ECB<16>, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "651a4fb35403c9b72f0ef002e8f58684"};

        block_test_case<RC6, ECB<16>, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "c10b5b4c3f20ab5b8a34c13c627eb332"};

    }

    // TODO we need tests for more than just one block.
    TEST (BlockCipher, AESCBC) {
        block_test_case<AES, CBC<16>, 16> {
            "00000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "3ad78e726c1ec02b7ebfe92b23d9ec34"};

        block_test_case<AES, CBC<16>, 24> {
            "000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "6cd02513e8d4dc986b4afe087a60bd0c"};

        block_test_case<AES, CBC<16>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "ddc6bf790c15760d8d9aeb6f9a75fd4e"};

        block_test_case<AES, CBC<16>, 16> {
            "1f8e4973953f3fb0bd6b16662e9a3c17",
            "2fe2b333ceda8f98f4a99b40d2cd34a8",
            "45cf12964fc824ab76616ae2f4bf0822",
            "0f61c4d44c5147c03c195ad7e2cc12b2"};

        block_test_case<AES, CBC<16>, 24> {
            "ba75f4d1d9d7cf7f551445d56cc1a8ab2a078e15e049dc2c",
            "531ce78176401666aa30db94ec4a30eb",
            "c51fc276774dad94bcdc1d2891ec8668",
            "70dd95a14ee975e239df36ff4aee1d5d"};

        block_test_case<AES, CBC<16>, 32> {
            "6ed76d2d97c69fd1339589523931f2a6cff554b15f738f21ec72dd97a7330907",
            "851e8764776e6796aab722dbb644ace8",
            "6282b8c05c5c1530b97d4816ca434762",
            "6acc04142e100a65f51b97adf5172c41"};

    }

    TEST (BlockCipher, AESCFB) {

        block_test_case<AES, CFB<16>, 16> {
            "085b8af6788fa6bc1a0b47dcf50fbd35",
            "58cb2b12bb52c6f14b56da9210524864",
            "4b5a872260293312eea1a570fd39c788",
            "e92c80e0cfb6d8b1c27fd58bc3708b16"};

        block_test_case<AES, CFB<16>, 24> {
            "1bbb30016d3a908827693352ece9833415433618b1d97595",
            "b2b48e8d60240bf2d9fa05cc2f90c161",
            "b4e499de51e646fad80030da9dc5e7e2",
            "8b7ba98982063a55fca3492269bbe437"};

        block_test_case<AES, CFB<16>, 32> {
            "e1c6e6884eee69552dbfee21f22ca92685d5d08ef0e3f37e5b338c533bb8d72c",
            "cea9f23ae87a637ab0cda6381ecc1202",
            "b72606c98d8e4fabf08839abf7a0ac61",
            "2981761d979bb1765a28b2dd19125b54"};

    }

    TEST (BlockCipher, AESOFB) {

        block_test_case<AES, OFB<16>, 16> {
            "d7d57bd847154af9722a8df096e61a42",
            "fdde201c91e401d9723868c2a612b77a",
            "81883f22165282ba6a442a8dd2a768d4",
            "84cc130b6867623696aa8f523d968ade"};

        block_test_case<AES, OFB<16>, 24> {
            "2943e3edfa815260a8a697b386ca3ae3eee914f22b3857dc",
            "c6995f00318c241217cdc82cf2fa43f9",
            "67e2cf5d63334ae03dbda91100ab781b",
            "225e8bfb133c4332ba6e95ddb841370d"};

        block_test_case<AES, OFB<16>, 32> {
            "6f419b4c683a44d67d234eaa6b57f622f912de657dddb280a14d0cb967ed951f",
            "19b888800ff1d0116124f79dfae54ffe",
            "3d12989faf41ba75bfa70e2bcc2fa222",
            "2d6b005e8d3bc6ea9f62dca36d47aea5"};

    }

    TEST (BlockCipher, AESCTR) {
        block_test_case<AES, CTR<16, endian::big>, 16> {
            "00000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "e6e94bd4ef8a2c3b884cfa59ca342b2e"};

        block_test_case<AES, CTR<16, endian::big>, 24> {
            "000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "2ae06992acbf52a3e8f4a96ec9300bd7"};

        block_test_case<AES, CTR<16, endian::big>, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "5c95c078a2408989ad48a21492842087"};

        block_test_case<AES, CTR<16, endian::big>, 16> {
            "1f8e4973953f3fb0bd6b16662e9a3c17",
            "2fe2b333ceda8f98f4a99b40d2cd34a8",
            "45cf12964fc824ab76616ae2f4bf0822",
            "a58571b5efb6c81dcc698c18381cb572"};

        block_test_case<AES, CTR<16, endian::big>, 24> {
            "ba75f4d1d9d7cf7f551445d56cc1a8ab2a078e15e049dc2c",
            "531ce78176401666aa30db94ec4a30eb",
            "c51fc276774dad94bcdc1d2891ec8668",
            "d968c321beda5728e6bd144f04fe0613"};

        block_test_case<AES, CTR<16, endian::big>, 32> {
            "6ed76d2d97c69fd1339589523931f2a6cff554b15f738f21ec72dd97a7330907",
            "851e8764776e6796aab722dbb644ace8",
            "6282b8c05c5c1530b97d4816ca434762",
            "3eb753155764df1edbfa5793f95d8867"};
    }

    // At this point we can say that the AES candidates work and that the block modes mostly work.

    // this is the only block size we use.
    constexpr const size_t block_size = 16;
}

namespace data::crypto::cipher {

    template <typename cipher> std::string print_cipher () {
        if constexpr (Same<cipher, block::Rijndael>) {
            return "Rijndael";
        } else if constexpr (Same<cipher, block::DES>) {
            return "DES";
        } else if constexpr (Same<cipher, block::TripleDES>) {
            return "Triple DES";
        } else if constexpr (Same<cipher, block::Twofish>) {
            return "Twofish";
        } else if constexpr (Same<cipher, block::Serpent>) {
            return "Serpent";
        } else if constexpr (Same<cipher, block::MARS>) {
            return "MARS";
        } else if constexpr (Same<cipher, block::RC6>) {
            return "RC6";
        } else if constexpr (Same<cipher, stream::XChaCha20>) {
            return "XChaCha20";
        } else if constexpr (Same<cipher, stream::Salsa20>) {
            return "Salsa20";
        } else if constexpr (Same<cipher, stream::HC128>) {
            return "HC128";
        } else if constexpr (Same<cipher, stream::HC256>) {
            return "HC256";
        } else if constexpr (Same<cipher, stream::Panama>) {
            return "Panama";
        }
        return "";
    }

    template <typename mode> struct print_mode;

    template <size_t block_size> struct print_mode<block::ECB<block_size>> {
        std::string operator () () {
            return string::write ("mode = ECB; block size = ", block_size);
        }
    };

    template <size_t block_size> struct print_mode<block::OFB<block_size>> {
        std::string operator () () {
            return string::write ("mode = OFB; block size = ", block_size);
        }
    };

    template <size_t block_size> struct print_mode<block::CBC<block_size>> {
        std::string operator () () {
            return string::write ("mode = CBC; block size = ", block_size);
        }
    };

    template <size_t block_size> struct print_mode<block::CFB<block_size>> {
        std::string operator () () {
            return string::write ("mode = CFB; block size = ", block_size);
        }
    };

    template <size_t block_size, endian::order r> struct print_mode<block::CTR<block_size, r>> {
        std::string operator () () {
            std::string e;
            if constexpr (r == endian::order::little) {
                e = "little";
            } else {
                e = "big";
            }

            return string::write ("mode = CTR; block size = ", block_size, "; endian = ", e);
        }
    };

    tuple<byte_array<8>, byte_array<16>, byte_array<24>, byte_array<32>> IV1 = {
        byte_array<8>  (encoding::hex::fixed<8>  {"0123456789abcdef"}),
        byte_array<16> (encoding::hex::fixed<16> {"0123456789abcdef0369cf258be147ad"}),
        byte_array<24> (encoding::hex::fixed<24> {"0123456789abcdef0369cf258be147ad0123456789abcdef"}),
        byte_array<32> (encoding::hex::fixed<32> {"0123456789abcdef0369cf258be147ad0123456789abcdef0369cf258be147ad"})
    };

    tuple<byte_array<8>, byte_array<16>, byte_array<24>, byte_array<32>> IV2 = {
        byte_array<8>  (encoding::hex::fixed<8>  {"abcdef0123456789"}),
        byte_array<16> (encoding::hex::fixed<16> {"abcdef01234567890000000000000000"}),
        byte_array<24> (encoding::hex::fixed<24> {"abcdef01234567890000000000000000abcdef0123456789"}),
        byte_array<32> (encoding::hex::fixed<32> {"abcdef01234567890000000000000000abcdef01234567890000000000000000"})
    };

    // we should be able to encrypt a message and decrypt it to the same thing.
    template <typename alg, size_t key_size, typename mode>
    void block_encrypt_and_decrypt (const mode &m, const symmetric_key<key_size> &key, block::padding_scheme p, const bytes &message) {
        DATA_LOG (normal) << "attempt to block encrypt with \n\talgorithm = " <<
            print_cipher<alg> () << "\n\t" << print_mode<mode> {} () << "\n\tkey_size = " << key_size;
        bytes decrypted = decrypt<alg> (block_cipher {m, p}, key, encrypt<alg> (block_cipher {m, p}, key, message));
        EXPECT_EQ (message, decrypted) << "tried to decrypt to " << message << " but got " << decrypted;
    }

    template <typename alg, size_t key_size, typename mode>
    void block_encrypt_and_decrypt (const mode &m, const symmetric_key<key_size> &key, const bytes &message) {
        DATA_LOG (normal) << "attempt to stream encrypt with \n\talgorithm = " <<
            print_cipher<alg> () << "\n\t" << print_mode<mode> {} () << "\n\tkey_size = " << key_size;
        bytes decrypted = decrypt<alg> (virtual_stream_cipher {m}, key, encrypt<alg> (virtual_stream_cipher {m}, key, message));
        EXPECT_EQ (message, decrypted) << "tried to decrypt to " << message << " but got " << decrypted;
    }

    // next we test block ciphers.
    template <typename...> struct ciphers {};

    ciphers<block::Rijndael, block::Serpent, block::Twofish, block::MARS, block::RC6> supported_block_ciphers_128 {};
    //ciphers<DES, TripleDES> supported_block_ciphers_64 {};

    // modes of operation that we support and that can be used as block ciphers.
    tuple<block::ECB<16>, block::CBC<16>, block::CFB<16>, block::OFB<16>, block::CTR<16, endian::little>>
        block_modes_128 {{},
            {std::get<byte_array<16>> (IV1)},
            {std::get<byte_array<16>> (IV1)},
            {std::get<byte_array<16>> (IV1)},
            {math::uint<endian::little, 16, byte> {std::get<byte_array<16>> (IV1)}}};

    tuple<block::ECB<8>, block::CBC<8>, block::CFB<8>, block::OFB<8>, block::CTR<8, endian::little>>
        block_modes_64 {{},
            {std::get<byte_array<8>> (IV1)},
            {std::get<byte_array<8>> (IV1)},
            {std::get<byte_array<8>> (IV1)},
            {math::uint<endian::little, 8, byte> {std::get<byte_array<8>> (IV1)}}};

    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_change_key (const mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        block::padding_scheme p, const bytes &message) {

        block_cipher cryptor {m, p};

        bytes en1 = encrypt<cipher> (cryptor, k1, message);
        bytes en2 = encrypt<cipher> (cryptor, k2, message);

        EXPECT_NE (en1, en2);

        EXPECT_THROW (decrypt<cipher> (cryptor, k2, en1), data::exception);
        EXPECT_THROW (decrypt<cipher> (cryptor, k1, en2), data::exception);
    };

    template <typename cipher, size_t key_size, typename mode>
    void test_encrypt_change_padding (const mode &m,
        const symmetric_key<key_size> &key,
        block::padding_scheme p1, block::padding_scheme p2, const bytes &message) {

        block_cipher crypt1 {m, p1};
        block_cipher crypt2 {m, p2};

        bytes en1 = encrypt<cipher> (crypt1, key, message);
        bytes en2 = encrypt<cipher> (crypt2, key, message);

        EXPECT_NE (en1, en2);

        ASSERT_THROW (decrypt<cipher> (crypt2, key, en1), block::invalid_padding);
        ASSERT_THROW (decrypt<cipher> (crypt1, key, en2), block::invalid_padding);

    };

    // do nothing if the configuration is not valid.
    template <typename cipher, size_t key_size, typename mode>
    void test_block_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        block::padding_scheme p1,
        block::padding_scheme p2,
        const bytes &message) {};

    template <typename alg, size_t key_size,
        cipher::block::Mode<alg, key_size> mode>
    void test_block_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        block::padding_scheme p1,
        block::padding_scheme p2,
        const bytes &message) {

        block_encrypt_and_decrypt<alg> (m, k1, p1, message);
        block_encrypt_and_decrypt<alg> (m, k2, p1, message);

        test_encrypt_change_key<alg> (m, k1, k2, p1, message);
        test_encrypt_change_padding<alg> (m, k1, p1, p2, message);

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
                block::padding::PKCS_PADDING, block::padding::ONE_AND_ZEROS_PADDING, msg);

            test_block_ciphers_for<cipher_index, mode_index, key_index + 1> (c, m, k, msg);
        }
    }

    template <typename... cph, typename... modes, size_t... key_sizes>
    void test_block_ciphers (ciphers<cph...> c, tuple<modes...> m, keys<key_sizes...> k, const bytes &msg) {
        test_block_ciphers_for<0, 0, 0> (c, m, k, msg);
    }

    TEST (BlockCipher, Crypt) {

        test_block_ciphers (supported_block_ciphers_128, block_modes_128, test_keys, bytes {string {""}});
        test_block_ciphers (supported_block_ciphers_128, block_modes_128, test_keys, bytes {string {"a"}});
        test_block_ciphers (supported_block_ciphers_128, block_modes_128, test_keys,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}});
/*
        test_block_ciphers (supported_block_ciphers_64, block_modes_64, test_keys, bytes {string {""}});
        test_block_ciphers (supported_block_ciphers_64, block_modes_64, test_keys, bytes {string {"a"}});
        test_block_ciphers (supported_block_ciphers_64, block_modes_64, test_keys,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}});*/

    }

    tuple<block::CFB<16>, block::OFB<16>, block::CTR<16, endian::little>>
        streamable_modes_128 {
            {std::get<byte_array<16>> (IV1)},
            {std::get<byte_array<16>> (IV1)},
            {math::uint<endian::little, 16, byte> {std::get<byte_array<16>> (IV1)}}};

    tuple<block::CFB<8>, block::OFB<8>, block::CTR<8, endian::little>>
        streamable_modes_64 {
            {std::get<byte_array<8>> (IV1)},
            {std::get<byte_array<8>> (IV1)},
            {math::uint<endian::little, 8, byte> {std::get<byte_array<8>> (IV1)}}};

    template <typename alg, size_t key_size, typename IV>
    void stream_encrypt_and_decrypt (const symmetric_key<key_size> &key, const IV &iv, const bytes &message) {
        DATA_LOG (normal) << "attempt to stream encrypt with \n\talgorithm = " <<
            print_cipher<alg> () << "\n\tkey_size = " << key_size;
        bytes decrypted = decrypt<alg> (key, iv, encrypt<alg> (key, iv, message));
        EXPECT_EQ (message, decrypted) << "tried to decrypt to " << message << " but got " << decrypted;
    }

    template <typename alg, size_t key_size, typename mode>
    void test_virtual_stream_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        const bytes &message) {}

    template <typename alg, size_t key_size,
        cipher::block::StreamableMode<alg, key_size> mode>
    void test_virtual_stream_cipher (mode &m,
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        const bytes &message) {

        block_encrypt_and_decrypt<alg> (m, k1, message);
        block_encrypt_and_decrypt<alg> (m, k2, message);

    }

    template <size_t cipher_index, size_t mode_index, size_t key_index,
        typename... cph, typename... modes, size_t... key_sizes>
    void test_virtual_stream_ciphers_for (ciphers<cph...> c, tuple<modes...> m, keys<key_sizes...> k, const bytes &msg) {
        if constexpr (cipher_index >= sizeof...(cph)) return;
        else if constexpr (mode_index >= std::tuple_size<tuple<modes...>>::value) {
            test_virtual_stream_ciphers_for<cipher_index + 1, 0, 0> (c, m, k, msg);
        } else if constexpr (key_index >= std::tuple_size<typename keys<key_sizes...>::parent>::value) {
            test_virtual_stream_ciphers_for<cipher_index, mode_index + 1, 0> (c, m, k, msg);
        } else {

            auto key = std::get<key_index> (k);
            auto key_size = meta::get_value<key_index, key_sizes...>::value;
            using cipher = typename meta::get_type<cipher_index, cph...>::type;
            test_virtual_stream_cipher<cipher> (std::get<mode_index> (m), key, mutate_key (key), msg);

            test_virtual_stream_ciphers_for<cipher_index, mode_index, key_index + 1> (c, m, k, msg);
        }
    }

    template <typename... cph, typename... modes, size_t... key_sizes>
    void test_virtual_stream_ciphers (ciphers<cph...> c, tuple<modes...> m, keys<key_sizes...> k, const bytes &msg) {
        test_virtual_stream_ciphers_for<0, 0, 0> (c, m, k, msg);
    }

    TEST (VirtualStreamCipher, Crypt) {

        test_virtual_stream_ciphers (supported_block_ciphers_128, streamable_modes_128, test_keys, bytes {string {""}});
        test_virtual_stream_ciphers (supported_block_ciphers_128, streamable_modes_128, test_keys, bytes {string {"a"}});
        test_virtual_stream_ciphers (supported_block_ciphers_128, streamable_modes_128, test_keys,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}});
/*
        test_block_ciphers (supported_ciphers_64, block_modes_64, test_keys, bytes {string {""}});
        test_block_ciphers (supported_ciphers_64, block_modes_64, test_keys, bytes {string {"a"}});
        test_block_ciphers (supported_ciphers_64, block_modes_64, test_keys,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}});*/

    }

    // TODO need to try encrypting and decrypting full messages with the session interface.

    ciphers<stream::XChaCha20, stream::XSalsa20, stream::Salsa20,
        stream::HC128, stream::HC256, stream::Panama> supported_stream_ciphers {};

    using IV_sizes = std::index_sequence<24, 24, 8, 16, 32, 0>;

    template <typename alg, size_t key_size, typename IV>
    void test_stream_cipher (
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        const IV &iv,
        const bytes &message) {}

    template <typename alg, size_t key_size, typename IV>
    requires requires (const symmetric_key<key_size> &k, const IV &iv) {
        alg {k, iv};
    } void test_stream_cipher (
        const symmetric_key<key_size> &k1,
        const symmetric_key<key_size> &k2,
        const IV &iv,
        const bytes &message) {

        stream_encrypt_and_decrypt<alg> (k1, iv, message);
        stream_encrypt_and_decrypt<alg> (k2, iv, message);

    }

    template <size_t cipher_index, size_t key_index,
        typename... cph, size_t... key_sizes, size_t ...iv_sizes>
    void test_stream_ciphers_for (ciphers<cph...> c, keys<key_sizes...> k, const bytes &msg, std::index_sequence<iv_sizes...> ivs) {
        if constexpr (cipher_index >= sizeof...(cph)) return;
        else if constexpr (key_index >= std::tuple_size<typename keys<key_sizes...>::parent>::value) {
            test_stream_ciphers_for<cipher_index + 1, 0> (c, k, msg, ivs);
        } else {

            auto key = std::get<key_index> (k);
            auto key_size = meta::get_value<key_index, key_sizes...>::value;
            using cipher = typename meta::get_type<cipher_index, cph...>::type;
            constexpr static const size_t iv_size = meta::get_value<cipher_index, iv_sizes...>::value;

            if constexpr (iv_size == 0) {
                test_stream_cipher<cipher> (key, mutate_key (key), bytes {}, msg);
            } else {
                test_stream_cipher<cipher> (key, mutate_key (key), std::get<byte_array<iv_size>> (IV1), msg);
            }

            test_stream_ciphers_for<cipher_index, key_index + 1> (c, k, msg, ivs);
        }
    }

    template <typename... cph, size_t... key_sizes, typename IV_sizes>
    void test_stream_ciphers (ciphers<cph...> c, keys<key_sizes...> k, const bytes &msg, const IV_sizes &ivs) {
        test_stream_ciphers_for<0, 0> (c, k, msg, ivs);
    }

    TEST (CryptoppStreamCipher, Crypt) {

        test_stream_ciphers (supported_stream_ciphers, test_keys, bytes {string {""}}, IV_sizes {});
        test_stream_ciphers (supported_stream_ciphers, test_keys, bytes {string {"a"}}, IV_sizes {});
        test_stream_ciphers (supported_stream_ciphers, test_keys,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}}, IV_sizes {});

    }

}

// TODO Stream ciphers from cryptopp

// TODO Keccak
