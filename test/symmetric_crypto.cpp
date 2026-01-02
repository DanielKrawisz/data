// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/hash.hpp>
#include <data/crypto/block.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/MAC/SHA3.hpp>
#include <data/crypto/stream/cryptopp.hpp>
#include <data/crypto/block/cryptopp.hpp>
#include <data/tuple.hpp>
#include <data/list.hpp>
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

    template <typename Cipher, size_t key_size>
    concept TestEncryptable = requires (bytes msg, symmetric_key<key_size> key) {
        encrypt (block_cipher<Cipher, mode::ECB> {}, key, msg);
        decrypt (block_cipher<Cipher, mode::ECB> {}, key, msg);
    } && requires (bytes msg, symmetric_key<key_size> key, byte_array<Cipher::BlockSize> iv) {
        encrypt (block_cipher<Cipher, mode::CBC> {iv}, key, msg);
        decrypt (block_cipher<Cipher, mode::CBC> {iv}, key, msg);
        encrypt (block_cipher<Cipher, mode::CFB> {iv}, key, msg);
        decrypt (block_cipher<Cipher, mode::CFB> {iv}, key, msg);
        encrypt (block_cipher<Cipher, mode::OFB> {iv}, key, msg);
        decrypt (block_cipher<Cipher, mode::OFB> {iv}, key, msg);
        encrypt (block_cipher<Cipher, mode::CTR, endian::little> {math::uint_little<Cipher::BlockSize, byte> {iv}}, key, msg);
        decrypt (block_cipher<Cipher, mode::CTR, endian::little> {math::uint_little<Cipher::BlockSize, byte> {iv}}, key, msg);
    };

    static_assert (TestEncryptable<DES, 8>);
    static_assert (!TestEncryptable<DES, 16>);

    static_assert (TestEncryptable<TripleDES2, 16>);
    static_assert (TestEncryptable<TripleDES3, 24>);

    static_assert (TestEncryptable<AES, 16>);
    static_assert (!TestEncryptable<AES, 8>);
    static_assert (!TestEncryptable<AES, 20>);
    static_assert (TestEncryptable<AES, 24>);
    static_assert (TestEncryptable<AES, 32>);
    static_assert (!TestEncryptable<AES, 56>);

    static_assert (TestEncryptable<Serpent, 16>);
    static_assert (!TestEncryptable<Serpent, 8>);
    static_assert (!TestEncryptable<Serpent, 20>);
    static_assert (TestEncryptable<Serpent, 24>);
    static_assert (TestEncryptable<Serpent, 32>);
    static_assert (!TestEncryptable<Serpent, 56>);

    static_assert (TestEncryptable<Twofish, 16>);
    static_assert (!TestEncryptable<Twofish, 8>);
    static_assert (!TestEncryptable<Twofish, 20>);
    static_assert (TestEncryptable<Twofish, 24>);
    static_assert (TestEncryptable<Twofish, 32>);
    static_assert (!TestEncryptable<Twofish, 56>);

    static_assert (TestEncryptable<RC6, 16>);
    static_assert (!TestEncryptable<RC6, 8>);
    static_assert (!TestEncryptable<RC6, 20>);
    static_assert (TestEncryptable<RC6, 24>);
    static_assert (TestEncryptable<RC6, 32>);
    static_assert (!TestEncryptable<RC6, 56>);

    static_assert (TestEncryptable<MARS, 16>);
    static_assert (!TestEncryptable<MARS, 8>);
    static_assert (!TestEncryptable<MARS, 20>);
    static_assert (TestEncryptable<MARS, 24>);
    static_assert (TestEncryptable<MARS, 32>);
    static_assert (TestEncryptable<MARS, 56>);

    template <typename alg, typename state, typename bc, size_t key_size, typename ...IVs>
    void test_block_cipher (string key, string plaintext, string ciphertext, IVs &&...ivs) {
        byte_array<key_size> k {encoding::hex::fixed<key_size> (key)};

        {
            byte_array<16> pt {encoding::hex::fixed<16> (plaintext)};
            byte_array<16> ct {encoding::hex::fixed<16> (ciphertext)};

            state encryptor {ivs...};
            EXPECT_EQ ((encrypt<alg> (encryptor, k, pt)), ct);
            state decryptor {ivs...};
            auto decrypted = decrypt<alg> (decryptor, k, ct);
            EXPECT_EQ (decrypted, pt) << "Expected " << ct << " to decrypt to " <<
                pt << " using key " << k << " but got " << decrypted;
        }

        // since phase two of the test adds and checks padding, it proves that
        // the block mode correctly inverts itself.
        {
            bytes pt = *encoding::hex::read (plaintext);

            // we test this against the function for encrypting whole messages rather than just blocks.
            auto padded = data::crypto::encrypt (bc {ivs...}, k, pt);

            // we should expect the encrypted message to have a whole block of padding.
            //EXPECT_EQ (padded.size (), pt.size () + 16);

            bytes ct = *encoding::hex::read (ciphertext);
            EXPECT_EQ (ct, (take (padded, pt.size ())));

            auto decrypted = data::crypto::decrypt (bc {ivs...}, k, padded);

            EXPECT_EQ (pt, decrypted) << "expected plaintext " << pt << " to equal decrypted " << decrypted;

        }
    }

    template <typename alg, mode m, size_t key_size> struct block_test_case {
        block_test_case (string key, string IV, string plaintext, string ciphertext) {
            test_block_cipher<alg, state<m, alg::BlockSize>, block_cipher<alg, m>, key_size>
            (key, plaintext, ciphertext, byte_array<16> {encoding::hex::fixed<16> (IV)});
        }
    };

    template <typename alg, size_t key_size> struct block_test_case<alg, mode::ECB, key_size> {
        block_test_case (string key, string plaintext, string ciphertext) {
            test_block_cipher<alg, state<mode::ECB>, block_cipher<alg, mode::ECB>, key_size> (key, plaintext, ciphertext);
        }
    };

    template <typename alg, size_t key_size> struct block_test_case<alg, mode::CTR, key_size> {
        block_test_case (string key, string IV, string plaintext, string ciphertext) {
            test_block_cipher<alg, state<mode::CTR, alg::BlockSize, endian::big>, block_cipher<alg, mode::CTR, endian::big>, key_size>
                (key, plaintext, ciphertext, math::uint_big<16, byte> {byte_array<16> {encoding::hex::fixed<16> (IV)}});
        }
    };

    // A series of tests for each block mode with AES to test that the block modes are correct.
    TEST (BlockCipher, ECB) {

        // Since ECB doesn't really do anything, this test
        // proves that the underlying cipher works correctly.
        block_test_case<AES, mode::ECB, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "3ad78e726c1ec02b7ebfe92b23d9ec34"};

        block_test_case<AES, mode::ECB, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "6cd02513e8d4dc986b4afe087a60bd0c"};

        block_test_case<AES, mode::ECB, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "ddc6bf790c15760d8d9aeb6f9a75fd4e"};

        block_test_case<AES, mode::ECB, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "7888beae6e7a426332a7eaa2f808e637"};

        block_test_case<AES, mode::ECB, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "24f40c4eecd9c49825000fcb4972647a"};

        block_test_case<AES, mode::ECB, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "df8634ca02b13a125b786e1dce90658b"};

        block_test_case<Twofish, mode::ECB, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "73b9ff14cf2589901ff52a0d6f4b7ede"};

        block_test_case<Serpent, mode::ECB, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "a3b35de7c358ddd82644678c64b8bcbb"};

        block_test_case<MARS, mode::ECB, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "d962efeba75817bf6ed24bbbb5b7820d"};

        block_test_case<RC6, mode::ECB, 16> {
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "f71f65e7b80c0c6966fee607984b5cdf"};

        block_test_case<Twofish, mode::ECB, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "62ef193edb7d399aca50ec1cbe5398d8"};

        block_test_case<Serpent, mode::ECB, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "23f5f432ad687e0d4574c16459618abb"};

        block_test_case<MARS, mode::ECB, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "4433fccbf2d4aa14783835d0968bb7a6"};

        block_test_case<RC6, mode::ECB, 24> {
            "000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "a600b72aac4dfb5271dc45f563fae428"};

        block_test_case<Twofish, mode::ECB, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "23a385f617f313dac05bcb7eabd61807"};

        block_test_case<Serpent, mode::ECB, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "8314675e8ad5c3ecd83d852bcf7f566e"};

        block_test_case<MARS, mode::ECB, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "7dec45d013733a2fc3acb05ac5edcda0"};

        block_test_case<RC6, mode::ECB, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "80000000000000000000000000000000",
            "30f5c183f6c81a72f39b618b086cc2de"};

        block_test_case<Twofish, mode::ECB, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "1c0bda78c854cf6b6f7e2b0095659b66"};

        block_test_case<Serpent, mode::ECB, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "60da3514a0dd32237bdd3546bef56093"};

        block_test_case<MARS, mode::ECB, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "540848258fc6da64af9410878465d495"};

        block_test_case<RC6, mode::ECB, 16> {
            "edfdb257cb37cdf182c5455b0c0efebb",
            "1695fe475421cace3557daca01f445ff",
            "b8414291d911c6a9d97fc3b49e8c3253"};

        block_test_case<Twofish, mode::ECB, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "b9d2d9a16b495e35c7131c502e457543"};

        block_test_case<Serpent, mode::ECB, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "915730610cc1f93f2ab72db112655f9e"};

        block_test_case<MARS, mode::ECB, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "cb24760bf14501e4134c8eda2f8a0a59"};

        block_test_case<RC6, mode::ECB, 24> {
            "61396c530cc1749a5bab6fbcf906fe672d0c4ab201af4554",
            "60bcdb9416bac08d7fd0d780353740a5",
            "9bd0b1c332e51586ea6888dc6543dcad"};

        block_test_case<Twofish, mode::ECB, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "0df2210c88a3aa8fe59a0ac0cf8e80e7"};

        block_test_case<Serpent, mode::ECB, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "998b13f6702ace671a59c117524033a1"};

        block_test_case<MARS, mode::ECB, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "651a4fb35403c9b72f0ef002e8f58684"};

        block_test_case<RC6, mode::ECB, 32> {
            "cc22da787f375711c76302bef0979d8eddf842829c2b99ef3dd04e23e54cc24b",
            "ccc62c6b0a09a671d64456818db29a4d",
            "c10b5b4c3f20ab5b8a34c13c627eb332"};

    }

    // TODO we need tests for more than just one block.
    TEST (BlockCipher, AESCBC) {
        block_test_case<AES, mode::CBC, 16> {
            "00000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "3ad78e726c1ec02b7ebfe92b23d9ec34"};

        block_test_case<AES, mode::CBC, 24> {
            "000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "6cd02513e8d4dc986b4afe087a60bd0c"};

        block_test_case<AES, mode::CBC, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "ddc6bf790c15760d8d9aeb6f9a75fd4e"};

        block_test_case<AES, mode::CBC, 16> {
            "1f8e4973953f3fb0bd6b16662e9a3c17",
            "2fe2b333ceda8f98f4a99b40d2cd34a8",
            "45cf12964fc824ab76616ae2f4bf0822",
            "0f61c4d44c5147c03c195ad7e2cc12b2"};

        block_test_case<AES, mode::CBC, 24> {
            "ba75f4d1d9d7cf7f551445d56cc1a8ab2a078e15e049dc2c",
            "531ce78176401666aa30db94ec4a30eb",
            "c51fc276774dad94bcdc1d2891ec8668",
            "70dd95a14ee975e239df36ff4aee1d5d"};

        block_test_case<AES, mode::CBC, 32> {
            "6ed76d2d97c69fd1339589523931f2a6cff554b15f738f21ec72dd97a7330907",
            "851e8764776e6796aab722dbb644ace8",
            "6282b8c05c5c1530b97d4816ca434762",
            "6acc04142e100a65f51b97adf5172c41"};

    }

    TEST (BlockCipher, AESCFB) {

        block_test_case<AES, mode::CFB, 16> {
            "085b8af6788fa6bc1a0b47dcf50fbd35",
            "58cb2b12bb52c6f14b56da9210524864",
            "4b5a872260293312eea1a570fd39c788",
            "e92c80e0cfb6d8b1c27fd58bc3708b16"};

        block_test_case<AES, mode::CFB, 24> {
            "1bbb30016d3a908827693352ece9833415433618b1d97595",
            "b2b48e8d60240bf2d9fa05cc2f90c161",
            "b4e499de51e646fad80030da9dc5e7e2",
            "8b7ba98982063a55fca3492269bbe437"};

        block_test_case<AES, mode::CFB, 32> {
            "e1c6e6884eee69552dbfee21f22ca92685d5d08ef0e3f37e5b338c533bb8d72c",
            "cea9f23ae87a637ab0cda6381ecc1202",
            "b72606c98d8e4fabf08839abf7a0ac61",
            "2981761d979bb1765a28b2dd19125b54"};

    }

    TEST (BlockCipher, AESOFB) {

        block_test_case<AES, mode::OFB, 16> {
            "d7d57bd847154af9722a8df096e61a42",
            "fdde201c91e401d9723868c2a612b77a",
            "81883f22165282ba6a442a8dd2a768d4",
            "84cc130b6867623696aa8f523d968ade"};

        block_test_case<AES, mode::OFB, 24> {
            "2943e3edfa815260a8a697b386ca3ae3eee914f22b3857dc",
            "c6995f00318c241217cdc82cf2fa43f9",
            "67e2cf5d63334ae03dbda91100ab781b",
            "225e8bfb133c4332ba6e95ddb841370d"};

        block_test_case<AES, mode::OFB, 32> {
            "6f419b4c683a44d67d234eaa6b57f622f912de657dddb280a14d0cb967ed951f",
            "19b888800ff1d0116124f79dfae54ffe",
            "3d12989faf41ba75bfa70e2bcc2fa222",
            "2d6b005e8d3bc6ea9f62dca36d47aea5"};

    }

    TEST (BlockCipher, AESCTR) {
        block_test_case<AES, mode::CTR, 16> {
            "00000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "e6e94bd4ef8a2c3b884cfa59ca342b2e"};

        block_test_case<AES, mode::CTR, 24> {
            "000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "2ae06992acbf52a3e8f4a96ec9300bd7"};

        block_test_case<AES, mode::CTR, 32> {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000",
            "80000000000000000000000000000000",
            "5c95c078a2408989ad48a21492842087"};

        block_test_case<AES, mode::CTR, 16> {
            "1f8e4973953f3fb0bd6b16662e9a3c17",
            "2fe2b333ceda8f98f4a99b40d2cd34a8",
            "45cf12964fc824ab76616ae2f4bf0822",
            "a58571b5efb6c81dcc698c18381cb572"};

        block_test_case<AES, mode::CTR, 24> {
            "ba75f4d1d9d7cf7f551445d56cc1a8ab2a078e15e049dc2c",
            "531ce78176401666aa30db94ec4a30eb",
            "c51fc276774dad94bcdc1d2891ec8668",
            "d968c321beda5728e6bd144f04fe0613"};

        block_test_case<AES, mode::CTR, 32> {
            "6ed76d2d97c69fd1339589523931f2a6cff554b15f738f21ec72dd97a7330907",
            "851e8764776e6796aab722dbb644ace8",
            "6282b8c05c5c1530b97d4816ca434762",
            "3eb753155764df1edbfa5793f95d8867"};
    }
}

namespace data::crypto::cipher {

    // At this point we can say that the AES candidates work and that the block modes mostly work.

    // in order to improve block mode tests, we would add tests for multiple blocks, which
    // are available.

    // we now test that we fail to decrypt messages with the wrong algorithm.

    template <typename...> struct ciphers {};

    byte_array<32> IV1 = byte_array<32> (encoding::hex::fixed<32> {"0123456789abcdef0369cf258be147ad0123456789abcdef0369cf258be147ad"});
    byte_array<32> IV2 = byte_array<32> (encoding::hex::fixed<32> {"1023456789abcdef0369cf258be147ad0123456789abcdef0369cf258be147ad"});

    template <size_t key_size> symmetric_key<key_size> mutate_key (const symmetric_key<key_size> &k) {
        symmetric_key<key_size> new_key = k;
        new_key[0] = ~new_key[0];
        return new_key;
    }

    // Some stuff for testing ciphers with readers and writers.

    constexpr const uint32_little Magic = 0x12345678;

    struct message : string {
        using string::string;
    } msg1 {""}, msg2 {"msg2"},
    msg3 {"this message is definitely longer than a single block if we are using a block cipher"};

    writer<byte> inline &operator << (writer<byte> &w, const message &m) {
        return w << Magic << uint32_little {static_cast<uint32> (m.size ())} << bytes (m);
    }

    reader<byte> inline &operator >> (reader<byte> &r, message &m) {
        uint32_little magic;
        r >> magic;
        if (magic != Magic) throw exception {} << "Nope";
        uint32_little size;
        r >> size;
        bytes result;
        result.resize (size_t (uint32 (size)));
        r >> result;
        m = message (string (result));
        return r;
    }

    template <typename E> void cipher_encrypt (E &e) {
        e << msg1;
        e << msg2;
        e << msg3;
    }

    template <typename E> void cipher_decrypt (E &e) {
        message result1;
        message result2;
        message result3;

        e >> result1;
        e >> result2;
        e >> result3;

        EXPECT_EQ (result1, msg1);
        EXPECT_EQ (result2, msg2);
        EXPECT_EQ (result3, msg3);
    }

    // TODO test change padding
    // TODO test change key
    // TODO test change MAC key
}

namespace data::crypto::cipher::block {

    template <mode m, auto ...rest> struct block_mode {
        constexpr static const mode Mode = m;
    };

    template <typename ...> struct modes {};
    template <padding_scheme ...> struct paddings {};

    template <size_t...sizes>
    using key_sizes = std::index_sequence<sizes...>;

    template <typename cipher, padding_scheme padding, typename mode> struct initialize_algorithm;

    template <typename cipher, padding_scheme padding>
    struct initialize_algorithm<cipher, padding, block_mode<mode::ECB>> {
        block_cipher<cipher, mode::ECB> operator () (const byte_array<32> &IV) {
            return block_cipher<cipher, mode::ECB> {padding};
        };
    };

    template <typename cipher, padding_scheme padding, endian::order r>
    struct initialize_algorithm<cipher, padding, block_mode<mode::CTR, r>> {
        block_cipher<cipher, mode::CTR, r> operator () (const byte_array<32> &IV) {
            math::uint<r, cipher::BlockSize, byte> iv;
            std::copy (IV.begin (), IV.begin () + cipher::BlockSize, iv.begin ());
            return block_cipher<cipher, mode::CTR, r> {iv, padding};
        };
    };

    template <typename cipher, padding_scheme padding, mode m>
    struct initialize_algorithm<cipher, padding, block_mode<m>> {
        block_cipher<cipher, m> operator () (const byte_array<32> &IV) {
            byte_array<cipher::BlockSize> iv;
            std::copy (IV.begin (), IV.begin () + cipher::BlockSize, iv.begin ());
            return block_cipher<cipher, m> {iv, padding};
        };
    };

    template <typename algorithm, size_t key_size>
    void test_block_decrypt_succeed (const algorithm &m, const symmetric_key<key_size> &key, const bytes &ciphertext, const bytes &plaintext) {
        if (!m.valid ()) return;
        bytes decrypted = data::crypto::decrypt (m, key, ciphertext);
        EXPECT_EQ (plaintext, decrypted) << "tried to decrypt to " << plaintext << " but got " << decrypted;
    }

    template <typename algorithm, size_t key_size>
    void test_block_decrypt_fail (const algorithm &m, const symmetric_key<key_size> &key, const bytes &ciphertext, const bytes &plaintext) {
        if (!m.valid ()) return;
        // we may fail because we throw invalid padding or because
        // the decrypted text does not match, depending on the options.
        try {
            bytes decrypted = data::crypto::decrypt (m, key, ciphertext);
            EXPECT_NE (plaintext, decrypted);
        } catch (exception &) {}
    }

    template <typename cipher, padding_scheme padding, size_t key_size>
    void test_block_decrypt_wrong_modes (
        const symmetric_key<key_size> &key,
        const bytes &ciphertext,
        const bytes &plaintext,
        modes<>) {}

    template <typename cipher, padding_scheme padding, size_t key_size, mode m, auto ...n, typename ...other_modes>
    void test_block_decrypt_wrong_modes (
        const symmetric_key<key_size> &key,
        const bytes &ciphertext,
        const bytes &plaintext,
        modes<block_mode<m, n...>, other_modes...> om)
    {
        auto algorithm = initialize_algorithm<cipher, padding, block_mode<m, n...>> {} (IV1);
        test_block_decrypt_fail (algorithm, key, ciphertext, plaintext);
        test_block_decrypt_wrong_modes<cipher, padding> (key, ciphertext, plaintext, modes<other_modes...> {});
    }

    template <padding_scheme padding, typename mode, size_t key_size>
    void test_block_decrypt_wrong_ciphers (
        const symmetric_key<key_size> &key,
        const bytes &ciphertext,
        const bytes &plaintext,
        ciphers<>) {}

    template <padding_scheme padding, typename mode, size_t key_size, typename wrong_cipher, typename ...other_ciphers>
    void test_block_decrypt_wrong_ciphers (
        const symmetric_key<key_size> &key,
        const bytes &ciphertext,
        const bytes &plaintext,
        ciphers<wrong_cipher, other_ciphers...> oc)
    {
        if constexpr (Cipher<wrong_cipher, key_size>) {
            auto algorithm = initialize_algorithm<wrong_cipher, padding, mode> {} (IV1);
            test_block_decrypt_fail (algorithm, key, ciphertext, plaintext);
        }

        test_block_decrypt_wrong_ciphers<padding, mode> (key, ciphertext, plaintext, ciphers<other_ciphers...> {});
    }

    template <typename cipher, typename mode, size_t key_size>
    void test_block_decrypt_wrong_padding (
        const symmetric_key<key_size> &key,
        const bytes &ciphertext,
        const bytes &plaintext,
        paddings<>) {}

    template <typename cipher, typename mode, size_t key_size, padding_scheme wrong_padding, padding_scheme ...other_paddings>
    void test_block_decrypt_wrong_padding (
        const symmetric_key<key_size> &key,
        const bytes &ciphertext,
        const bytes &plaintext,
        paddings<wrong_padding, other_paddings...> op)
    {

        if (wrong_padding == padding::NO_PADDING && is_streamable (mode::Mode)) {
            auto algorithm = initialize_algorithm<cipher, wrong_padding, mode> {} (IV1);
            test_block_decrypt_fail (algorithm, key, ciphertext, plaintext);
        }

        test_block_decrypt_wrong_padding<cipher, mode> (key, ciphertext, plaintext, paddings<other_paddings...> {});
    }

    // do nothing if the configuration is not valid.
    template <typename cipher, typename mode, padding_scheme padding, size_t key_size,
        typename ...other_ciphers, typename ...other_modes, padding_scheme ...other_paddings>
    void test_block_cipher (
        const symmetric_key<key_size> &k,
        const bytes &message,
        ciphers<other_ciphers...> oc,
        modes<other_modes...> om,
        paddings<other_paddings...> op) {};

    template <typename cipher, typename mode, padding_scheme padding, size_t key_size,
        typename ...other_ciphers, typename ...other_modes, padding_scheme ...other_paddings>
    requires Cipher<cipher, key_size> && (is_streamable (mode::Mode) || padding != padding::NO_PADDING)
    void test_block_cipher (
        const symmetric_key<key_size> &k,
        const bytes &message,
        ciphers<other_ciphers...> oc,
        modes<other_modes...> om,
        paddings<other_paddings...> op) {

        auto algorithm = initialize_algorithm<cipher, padding, mode> {} (IV1);

        bytes ciphertext = encrypt (algorithm, k, message);

        test_block_decrypt_succeed (algorithm, k, ciphertext, message);

        auto k2 = mutate_key (k);

        test_block_decrypt_fail (algorithm, k2, ciphertext, message);

        if constexpr (mode::Mode != block::mode::ECB) {
            auto wrong_algorithm = initialize_algorithm<cipher, padding, mode> {} (IV2);
            test_block_decrypt_fail (wrong_algorithm, k, ciphertext, message);
        }

        test_block_decrypt_wrong_ciphers<padding, mode> (k, ciphertext, message, oc);
        test_block_decrypt_wrong_modes<cipher, padding> (k, ciphertext, message, om);
        test_block_decrypt_wrong_padding<cipher, mode> (k, ciphertext, message, op);

    };

    template <size_t cipher_index, size_t mode_index, size_t key_index, size_t padding_index,
        typename first_cipher, typename ...rest_ciphers,
        typename first_mode, typename ...rest_modes,
        size_t first_key_size, size_t ...rest_key_sizes,
        padding_scheme first_padding, padding_scheme ...rest_paddings>
    void test_block_ciphers_for (
        const bytes &message,
        ciphers<first_cipher, rest_ciphers...>,
        modes<first_mode, rest_modes...>,
        key_sizes<first_key_size, rest_key_sizes...>,
        paddings<first_padding, rest_paddings...>) {

        if constexpr (cipher_index > sizeof... (rest_ciphers)) return;

        else if constexpr (mode_index > sizeof... (rest_modes))
            return test_block_ciphers_for<cipher_index + 1, 0, 0, 0> (message,
                ciphers<rest_ciphers..., first_cipher> {},
                modes<first_mode, rest_modes...> {},
                key_sizes<first_key_size, rest_key_sizes...> {},
                paddings<first_padding, rest_paddings...> {});

        else if constexpr (key_index > sizeof... (rest_key_sizes))
            return test_block_ciphers_for<cipher_index, mode_index + 1, 0, 0> (message,
                ciphers<first_cipher, rest_ciphers...> {},
                modes<rest_modes..., first_mode> {},
                key_sizes<first_key_size, rest_key_sizes...> {},
                paddings<first_padding, rest_paddings...> {});

        else if constexpr (padding_index > sizeof... (rest_paddings))
            return test_block_ciphers_for<cipher_index, mode_index, key_index + 1, 0> (message,
                ciphers<first_cipher, rest_ciphers...> {},
                modes<first_mode, rest_modes...> {},
                key_sizes<rest_key_sizes..., first_key_size> {},
                paddings<first_padding, rest_paddings...> {});

        else {

            auto key = std::get<symmetric_key<first_key_size>> (test_keys);

            test_block_cipher<first_cipher, first_mode, first_padding> (key, message,
                ciphers<rest_ciphers...> {}, modes<rest_modes...> {}, paddings<rest_paddings...> {});

            return test_block_ciphers_for<cipher_index, mode_index, key_index, padding_index + 1> (message,
                ciphers<first_cipher, rest_ciphers...> {},
                modes<first_mode, rest_modes...> {},
                key_sizes<first_key_size, rest_key_sizes...> {},
                paddings<rest_paddings..., first_padding> {});
        }
    };

    template <typename ...all_ciphers, typename ...all_modes, size_t ...all_key_sizes, padding_scheme ...all_paddings>
    void test_block_ciphers (
        ciphers<all_ciphers...> c,
        modes<all_modes...> m,
        key_sizes<all_key_sizes...> k,
        paddings<all_paddings...> p,
        const bytes &message) {
        test_block_ciphers_for<0, 0, 0, 0> (message, c, m, k, p);
    }

    template <typename... cph, typename... modes, size_t... key_sizes>
    void test_block_ciphers (ciphers<cph...> c, tuple<modes...> m, keys<key_sizes...> k, const bytes &msg) {
        test_block_ciphers_for<0, 0, 0> (c, m, k, msg);
    }

    // next we test block ciphers.
    ciphers<DES, TripleDES2, TripleDES3, Rijndael, Serpent, Twofish, MARS, RC6> supported_block_ciphers {};

    modes<
        block_mode<mode::ECB>,
        block_mode<mode::CBC>,
        block_mode<mode::OFB>,
        block_mode<mode::CFB>,
        block_mode<mode::CTR, endian::big>,
        block_mode<mode::CTR, endian::little>> supported_block_modes {};

    paddings<padding::NO_PADDING, padding::ONE_AND_ZEROS_PADDING, padding::PKCS_PADDING> supported_padding {};

    TEST (BlockCipher, EncryptDecrypt) {

        key_sizes<8, 16, 20, 24, 28, 32, 40, 48, 56> key_test_sizes {};

        test_block_ciphers (supported_block_ciphers, supported_block_modes, key_test_sizes, supported_padding,
            bytes {string {"hi, this is a message that is definitely longer than a single block"}});

    }

    template <typename ...all_ciphers, typename ...all_modes, size_t ...all_key_sizes, padding_scheme ...all_paddings>
    void test_block_cipher_writers (
        ciphers<all_ciphers...> c,
        modes<all_modes...> m,
        key_sizes<all_key_sizes...> k,
        paddings<all_paddings...> p,
        list<bytes> messages) {}


    TEST (BlockCipher, WriteRead) {

        key_sizes<8, 16, 20, 24, 28, 32, 40, 48, 56> key_test_sizes {};

        test_block_cipher_writers (supported_block_ciphers, supported_block_modes, key_test_sizes, supported_padding,
            {bytes {string {"hi, this is a message that is definitely longer than a single block"}}});

    }
}

namespace data::crypto::cipher {

    ciphers<stream::XChaCha20, stream::XSalsa20, stream::Salsa20,
        stream::HC128, stream::HC256, stream::Panama> supported_stream_ciphers {};

    using IV_sizes = std::index_sequence<24, 24, 8, 16, 32, 32>;

    template <typename alg, size_t key_size, typename IV>
    void stream_encrypt_and_decrypt (const symmetric_key<key_size> &key, const IV &iv, const bytes &message) {
        bytes decrypted = decrypt<alg> (key, iv, encrypt<alg> (key, iv, message));
        EXPECT_EQ (message, decrypted) << "tried to decrypt to " << message << " but got " << decrypted;
    }

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
        typename... cph, size_t ...key_sizes, size_t ...iv_sizes>
    void test_stream_ciphers_for (ciphers<cph...> c, keys<key_sizes...> k, const bytes &msg, std::index_sequence<iv_sizes...> ivs) {
        if constexpr (cipher_index >= sizeof... (cph)) return;
        else if constexpr (key_index >= sizeof... (key_sizes))
            test_stream_ciphers_for<cipher_index + 1, 0> (c, k, msg, ivs);
        else {

            auto key = std::get<key_index> (k);
            using cipher = typename meta::get_type<cipher_index, cph...>::type;
            constexpr static const size_t iv_size = meta::get_value<cipher_index, iv_sizes...>::value;

            byte_array<iv_size> iv;
            std::copy (IV1.begin (), IV1.begin () + iv_size, iv.begin ());
            test_stream_cipher<cipher> (key, mutate_key (key), iv, msg);

            test_stream_ciphers_for<cipher_index, key_index + 1> (c, k, msg, ivs);
        }
    }

    template <typename... cph, typename keys, typename IV_sizes>
    void test_stream_ciphers (ciphers<cph...> c, keys k, const bytes &msg, const IV_sizes &ivs) {
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
