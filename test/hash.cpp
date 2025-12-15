// Copyright (c) 2023 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/crypto/hash.hpp"
#include "data/math/number/bytes.hpp"
#include "data/list.hpp"
#include "data/encoding/endian.hpp"
#include "gtest/gtest.h"

namespace data {

    // TODO include MD5
    struct SHA_test_case {
        string Test;
        string ExpectedSHA1;
        string ExpectedSHA2_224;
        string ExpectedSHA2_256;
        string ExpectedSHA2_384;
        string ExpectedSHA2_512;
        string ExpectedSHA3_224;
        string ExpectedSHA3_256;
        string ExpectedSHA3_384;
        string ExpectedSHA3_512;

        template <hash::Engine w>
        static void run (const string &test, const string &expected) {
            hash::digest<w::Size> expected_digest {expected};
            hash::digest<w::Size> result = hash::calculate<w> (bytes (test));
            EXPECT_EQ (expected_digest, result) << "expected " << test << " to hash to " << expected_digest << " but got " << result;
        }
    };

    TEST (Hash, SHA) {

        // from https://www.di-mgt.com.au/sha_testvectors.html
        for (const auto &test_case : list<SHA_test_case> {
            {"abc",
                "a9993e364706816aba3e25717850c26c9cd0d89d",
                "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7",
                "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
                "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7",
                "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
                    "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f",
                "e642824c3f8cf24ad09234ee7d3c766fc9a3a5168d0c94ad73b46fdf",
                "3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532",
                "ec01498288516fc926459f58e2c6ad8df9b473cb0fc08c2596da7cf0e49be4b298d88cea927ac7f539f1edf228376d25",
                "b751850b1a57168a5693cd924b6b096e08f621827444f70d884f5d0240d2712e"
                    "10e116e9192af3c91a7ec57647e3934057340b4cf408d5a56592f8274eec53f0"},
            {"",
                "da39a3ee5e6b4b0d3255bfef95601890afd80709",
                "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f",
                "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
                "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b",
                "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
                    "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
                "6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7",
                "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a",
                "0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac3713831264adb47fb6bd1e058d5f004",
                "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a6"
                    "15b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26"},
            {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                "84983e441c3bd26ebaae4aa1f95129e5e54670f1",
                "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525",
                "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
                "3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b",
                "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c335"
                    "96fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445",
                "8a24108b154ada21c9fd5574494479ba5c7e7ab76ef264ead0fcce33",
                "41c0dba2a9d6240849100376a8235e2c82e1b9998a999e21db32dd97496d3376",
                "991c665755eb3a4b6bbdfb75c78a492e8c56a22c5c4d7e429bfdbc32b9d4ad5aa04a1f076e62fea19eef51acd0657c22",
                "04a371e84ecfb5b8b77cb48610fca8182dd457ce6f326a0fd3d7ec2f1e91636d"
                    "ee691fbe0c985302ba1b0d8dc78c086346b533b49c030d99a27daf1139d6e75e"},
            {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                "a49b2446a02c645bf419f995b67091253a04a259",
                "c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3",
                "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1",
                "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039",
                "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018"
                    "501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909",
                "543e6868e1666c1a643630df77367ae5a62a85070a51c14cbf665cbc",
                "916f6061fe879741ca6469b43971dfdb28b1a32dc36cb3254e812be27aad1d18",
                "79407d3b5916b59c3e30b09822974791c313fb9ecc849e406f23592d04f625dc8c709b98b43b3852b337216179aa7fc7",
                "afebb2ef542e6579c50cad06d2e578f9f8dd6881d7dc824d26360feebf18a4fa"
                    "73e3261122948efcfd492e74e82e2189ed0fb440d187f382270cb455f21dd185"}}) {

            SHA_test_case::run<crypto::hash::SHA1> (test_case.Test, test_case.ExpectedSHA1);
            SHA_test_case::run<crypto::hash::SHA2<28>> (test_case.Test, test_case.ExpectedSHA2_224);
            SHA_test_case::run<crypto::hash::SHA2<32>> (test_case.Test, test_case.ExpectedSHA2_256);
            SHA_test_case::run<crypto::hash::SHA2<48>> (test_case.Test, test_case.ExpectedSHA2_384);
            SHA_test_case::run<crypto::hash::SHA2<64>> (test_case.Test, test_case.ExpectedSHA2_512);
            SHA_test_case::run<crypto::hash::SHA3<28>> (test_case.Test, test_case.ExpectedSHA3_224);
            SHA_test_case::run<crypto::hash::SHA3<32>> (test_case.Test, test_case.ExpectedSHA3_256);
            SHA_test_case::run<crypto::hash::SHA3<48>> (test_case.Test, test_case.ExpectedSHA3_384);
            SHA_test_case::run<crypto::hash::SHA3<64>> (test_case.Test, test_case.ExpectedSHA3_512);
        }

    }

    void RIPEMD_test_case (std::string test, string dig) {
        EXPECT_EQ (crypto::RIPEMD_160 (test), hash::digest<20> {dig});
        EXPECT_EQ (hash::calculate<crypto::hash::RIPEMD<20>> (test), hash::digest<20> {dig});
        EXPECT_EQ (hash::calculate<hash::writer<crypto::hash::RIPEMD<20>>> (test), hash::digest<20> {dig});

        hash::digest<20> result; {
            hash::writer<crypto::hash::RIPEMD<20>> w {result};
            w << test;
        }

        EXPECT_EQ (result, hash::digest<20> {dig});
    }

    // from https://rosettacode.org/wiki/RIPEMD-160
    TEST (Hash, RIPEMD) {

        RIPEMD_test_case ("", "9c1185a5c5e9fc54612808977ee8f548b2258d31");
        RIPEMD_test_case ("a", "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe");
        RIPEMD_test_case ("abc", "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc");
        RIPEMD_test_case ("message digest", "5d0689ef49d2fae572b881b123a85ffa21595f36");
        RIPEMD_test_case ("abcdefghijklmnopqrstuvwxyz",
            "f71c27109c692c1b56bbdceb5b9d2865b3708dbc");
        RIPEMD_test_case ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "12a053384a9c0c88e405a06c27dcf49ada62eb2b");

    }

    TEST (Hash, BitcoinHash) {
        bytes test = *encoding::hex::read ("00010203fdfeff");
        hash::digest256 expected {"be586c8b20dee549bdd66018c7a79e2b67bb88b7c7d428fa4c970976d2bec5ba"};

        EXPECT_EQ (crypto::Bitcoin_256 (test), expected);
    }

}
