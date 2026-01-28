// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/random.hpp>
#include <data/crypto/hash.hpp>
#include <data/crypto/block.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/MAC/SHA3.hpp>
#include <data/crypto/stream/cryptopp.hpp>
#include <data/crypto/block/cryptopp.hpp>
#include <data/crypto/NIST_DRBG.hpp>
#include <data/tuple.hpp>
#include <data/list.hpp>
#include <data/io/log.hpp>

#include <type_traits>
#include <cstddef>

namespace data::crypto {

    template <typename R>
    void test_instantiate_and_reseed () {

        size_t good_bytes = R::MaxStrength / 8;
        size_t bad_bytes = good_bytes - 1;

        EXPECT_THROW ((R {bytes (bad_bytes), byte_slice {}}), NIST::insufficient_entropy);
        EXPECT_NO_THROW ((R {bytes (good_bytes), byte_slice {}}));
        R r {bytes (good_bytes), byte_slice {}};
        EXPECT_THROW (r.reseed (bytes (bad_bytes)), NIST::insufficient_entropy);
        EXPECT_NO_THROW (r.reseed (bytes (good_bytes)));
    }

    TEST (NIST_DRBG, MaxStrength) {

        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA1>::MaxStrength), 128);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_224>::MaxStrength), 112);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_256>::MaxStrength), 128);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_384>::MaxStrength), 192);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_512>::MaxStrength), 256);

        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA1>::SeedBits), 440);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_224>::SeedBits), 440);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_256>::SeedBits), 440);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_384>::SeedBits), 888);
        EXPECT_EQ ((NIST::Hash_DRBG<hash::SHA2_512>::SeedBits), 888);

        EXPECT_EQ ((NIST::HMAC_DRBG<hash::SHA1>::MaxStrength), 128);
        EXPECT_EQ ((NIST::HMAC_DRBG<hash::SHA2_224>::MaxStrength), 112);
        EXPECT_EQ ((NIST::HMAC_DRBG<hash::SHA2_256>::MaxStrength), 128);
        EXPECT_EQ ((NIST::HMAC_DRBG<hash::SHA2_384>::MaxStrength), 192);
        EXPECT_EQ ((NIST::HMAC_DRBG<hash::SHA2_512>::MaxStrength), 256);

        EXPECT_EQ ((NIST::CTR_DRBG<21, cipher::block::TDEA3>::MaxStrength), 112);
        EXPECT_EQ ((NIST::CTR_DRBG<16, cipher::block::AES>::MaxStrength), 128);
        EXPECT_EQ ((NIST::CTR_DRBG<24, cipher::block::AES>::MaxStrength), 192);
        EXPECT_EQ ((NIST::CTR_DRBG<32, cipher::block::AES>::MaxStrength), 256);

        EXPECT_EQ ((NIST::CTR_DRBG<21, cipher::block::TDEA3>::SeedLength), 232/8);
        EXPECT_EQ ((NIST::CTR_DRBG<16, cipher::block::AES>::SeedLength), 256/8);
        EXPECT_EQ ((NIST::CTR_DRBG<24, cipher::block::AES>::SeedLength), 320/8);
        EXPECT_EQ ((NIST::CTR_DRBG<32, cipher::block::AES>::SeedLength), 384/8);

        test_instantiate_and_reseed<NIST::Hash_DRBG<hash::SHA1>> ();
        test_instantiate_and_reseed<NIST::Hash_DRBG<hash::SHA2_224>> ();
        test_instantiate_and_reseed<NIST::Hash_DRBG<hash::SHA2_256>> ();
        test_instantiate_and_reseed<NIST::Hash_DRBG<hash::SHA2_384>> ();
        test_instantiate_and_reseed<NIST::Hash_DRBG<hash::SHA2_512>> ();

        test_instantiate_and_reseed<NIST::HMAC_DRBG<hash::SHA1>> ();
        test_instantiate_and_reseed<NIST::HMAC_DRBG<hash::SHA2_224>> ();
        test_instantiate_and_reseed<NIST::HMAC_DRBG<hash::SHA2_256>> ();
        test_instantiate_and_reseed<NIST::HMAC_DRBG<hash::SHA2_384>> ();
        test_instantiate_and_reseed<NIST::HMAC_DRBG<hash::SHA2_512>> ();

        test_instantiate_and_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3>> ();
        test_instantiate_and_reseed<NIST::CTR_DRBG<16, cipher::block::AES>> ();
        test_instantiate_and_reseed<NIST::CTR_DRBG<24, cipher::block::AES>> ();
        test_instantiate_and_reseed<NIST::CTR_DRBG<32, cipher::block::AES>> ();
    }

    template <typename DRBG> struct NIST_test_case_no_reseed {
        hex_string EntropyInput;
        hex_string Nonce;
        hex_string Personalization;
        hex_string AdditionalInput1;
        hex_string AdditionalInput2;
        hex_string Returned;

        void run () {
            DRBG drbg {bytes (EntropyInput), bytes (Nonce), bytes (Personalization)};

            bytes returned (Returned);
            bytes generated;
            generated.resize (returned.size ());

            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput1));
            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput2));

            EXPECT_EQ (generated, returned) << "Expected to generate " << returned << " but got " << generated ;
        }
    };

    template <typename DRBG> struct NIST_test_case_no_pr {
        hex_string EntropyInput;
        hex_string Nonce;
        hex_string Personalization;
        hex_string EntropyReseed;
        hex_string AdditionalReseed;
        hex_string AdditionalInput1;
        hex_string AdditionalInput2;
        hex_string Returned;

        void run () {
            DRBG drbg {bytes (EntropyInput), bytes (Nonce), bytes (Personalization)};

            drbg.reseed (bytes (EntropyReseed), bytes (AdditionalReseed));

            bytes returned (Returned);
            bytes generated;
            generated.resize (returned.size ());

            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput1));
            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput2));

            EXPECT_EQ (generated, returned) << "Expected to generate " << returned << " but got " << generated ;
        }
    };

    TEST (NIST_DRBG, Hash_SHA1) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA1>> {
            hex_string {"136cf1c174e5a09f66b962d994396525"},
            hex_string {"fff1c6645f19231f"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"0e28130fa5ca11edd3293ca26fdb8ae1810611f78715082ed3841e7486f16677b28e33ffe0b93d98"
                "ba57ba358c1343ab2a26b4eb7940f5bc639384641ee80a25140331076268bd1ce702ad534dda0ed8"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA1>> {
            hex_string {"f7e316b13117dcc18c4407b6a5cdc5d8"},
            hex_string {"b80ddee75cf39a48"},
            hex_string {"816cb137ef64f9df71a3b3a0b3aaf9b1"}, hex_string {}, hex_string {},
            hex_string {"be888585d95d95269f002abd8b1e33b2cb667a96d3be6d20d784b1bbc6639347837d01d4b95eed8137cf29"
                "fe724cfedf8b23f9258480be350c3407973c59a9d7f3a6585b3c0e36a36c6234d68852acaf"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA1>> {
            hex_string {"c3ef82ce241f02e4298b118ca4f16225"},
            hex_string {"15e32abbae6b7433"}, hex_string {},
            hex_string {"2b790052f09b364d4a8267a0a7de63b8"},
            hex_string {"2ee0819a671d07b5085cc46aa0e61b56"},
            hex_string {"5825fa1d1dc33c64cdc8690682eff06039e79508c3af48e880f8227d5f9aaa14b3bc76baee47"
                "7ebbb5c45547134179223257525e8f3afefb78b59da032f1006d74c9831375a677eab3239c94ebe3f7fa"}
        }.run ();

        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA1>> {
            hex_string {"6466e1799a68012379631b3aae41f59b"},
            hex_string {"6b0c61269f67c576"},
            hex_string {"cc936b87c8c8c1ab85dde0ad2e9242b4"},
            hex_string {"d1033ac553ef08f22fd38f12b49b45bc"},
            hex_string {"f004ba01f51455430d84362e376eb775"},
            hex_string {"5d675d1e92490952703c194194e1b061b6ec4e219dc2e1edaa891ef2d1b7ed050a06342d3c09501"
                "1eb339f198519779b01ab1a580bd2e34d6cf4e47c1befe0c7dc37b4aafb31128fa396267f3732095a"}
        }.run ();

        // reseed before generate calls.
        NIST_test_case_no_pr<NIST::Hash_DRBG<hash::SHA1>> {
            hex_string {"1610b828ccd27de08ceea032a20e9208"},
            hex_string {"492cf1709242f6b5"}, hex_string {},
            hex_string {"72d28c908edaf9a4d1e526d8f2ded544"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"56f33d4fdbb9a5b64d26234497e9dcb87798c68d08f7c41199d4bddf97ebbf6cb5550e5d149f"
                "f4d5bd0f05f25a6988c17436396227184af84a564335658e2f8572bea333eee2abff22ffa6de3e22aca2"}
        }.run ();

        NIST_test_case_no_pr<NIST::Hash_DRBG<hash::SHA1>> {
            hex_string {"d9bab5cedca96f6178d64509a0dfdc5e"},
            hex_string {"dad8989414450e01"}, hex_string {},
            hex_string {"c6bad074c5906786f5e1f32099f5b491"},
            hex_string {"3e6bf46f4daa3825d7194e694e7752f7"},
            hex_string {"04fa2895aa5a6f8c5743343b805e5ea4"},
            hex_string {"df5dc459dff02aa2f052d721ec607230"},
            hex_string {"c48b89f9da3f748245555d5d033b693dd71a4df5690205cefcd720113cc24e098936ff5e77b54"
                "1535870b339468cdd8d6faf8c56163a700a75b23e599b5aecf16f3baf6d5f2419971f24f446720feabe"}
        }.run ();

        // prediction resistance, meaning we reseed after every generate call.
    }

    TEST (NIST_DRBG, HMAC_SHA1) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA1>> {
            hex_string {"e91b63309e93d1d08e30e8d556906875"},
            hex_string {"f59747c468b0d0da"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"b7928f9503a417110788f9d0c2585f8aee6fb73b220a626b3ab9825b7a9facc79723d7e1ba92"
                "55e40e65c249b6082a7bc5e3f129d3d8f69b04ed1183419d6c4f2a13b304d2c5743f41c8b0ee73225347"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA1>> {
            hex_string {"49058e6773ed2b7ab309c0949fdf9c9e"},
            hex_string {"a457cb8ec0e7fd01"},
            hex_string {"dc477641d89c7fc4a30f1430197dd159"}, hex_string {}, hex_string {},
            hex_string {"4e891f4e281100453b70788929ec743a3c5edd9b81dc798bc93771368c39b612037b6f42f60c5d8924b6468"
                "48151b0c295be491d4a28d1927deed523fd04d3d2dda95ed42166312e5c3392d22893b0dc"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA1>> {
            hex_string {"32c1ca125223de8de569697f92a37c67"},
            hex_string {"72d4cc4f0544d409"}, hex_string {},
            hex_string {"9e98cc8e0f8eb84d1911c1775a5703bb"},
            hex_string {"593aa3a300e5c907a011dd5a3dcd77e2"},
            hex_string {"942909a9d380aa5d4e3af69093a8fa513ee545b9bf9e1b81c5f30966db3e5cb52f8b1b6fe440d592"
                "e5fe4a972c36aa498035e2442f82910c5cd095c7f4b4c7e7555c4669cca481cdfbfda167b5d6f8d5"}
        }.run ();

        // reseed before generate calls.
        NIST_test_case_no_pr<NIST::HMAC_DRBG<hash::SHA1>> {
            hex_string {"79349bbf7cdda5799557866621c91383"},
            hex_string {"1146733abf8c35c8"}, hex_string {},
            hex_string {"c7215b5b96c48e9b338c74e3e99dfedf"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"c6a16ab8d420706f0f34ab7fec5adca9d8ca3a133e159ca6ac43c6f8a2be22834a4c0a0affb1"
                "0d7194f1c1a5cf7322ec1ae0964ed4bf122746e087fdb5b3e91b3493d5bb98faed49e85f130fc8a459b7"}
        }.run ();

        NIST_test_case_no_pr<NIST::HMAC_DRBG<hash::SHA1>> {
            hex_string {"11c0a7e1472cec70fa8c1ca15759ac5b"},
            hex_string {"b1c73c22db39cd7b"},
            hex_string {"b24e392cb1f3c18af2cb50feac733e32"},
            hex_string {"c6ab59ff708a5c1f598e75df060e1981"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"070e603cd48d56430a5ab461a751ec2a4a6aa6fb6ee52efe9a41e4611eafdfc957184b47bbb01"
                "7e484ac34c7de56cd7813feb301b5befce573ad0a254e6cfe35b77c30be6b7cb5e7efa72813c7546ba5"}
        }.run ();

        NIST_test_case_no_pr<NIST::HMAC_DRBG<hash::SHA1>> {
            hex_string {"03e7b41c95818eb0b667bfa8a175a824"},
            hex_string {"66a1e417a9b6b92f"},
            hex_string {"126dded5eb0bc81be37c10bcd9d5f793"},
            hex_string {"d17e98c2e50ee0db00d25c3364451e95"},
            hex_string {"dc596d188e2343802240bc7f5cc60516"},
            hex_string {"14c8ec10f5bdde6b9e75898d7f9f03d0"},
            hex_string {"31aa842afcc1daa94098241a87d6ddfc"},
            hex_string {"4739b1bcf87404a2290829bd7a61f0b391a794c71c055c7cc513b28dcb5fdc88645bc9cb490f41fab"
                "134c6b33ce9336571762754343961de671b02a47960b4b4e23c5bfb87dcc19b260b3bcb921ae325"}
        }.run ();

    }

    TEST (NIST_DRBG, Hash_SHA2_224) {
        // reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_224>> {
            hex_string {"39af3b5438eef3242073aa3aa7a3cf1b713cd38e230fc782"},
            hex_string {"45fadab7fd239df2b9a4783f"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"d86792dca80f167184fad6eba188a08160b14c324c5e279d98e5de1047402e3de71fe92aefdfe5c8aecb416757a67dcc"
                "e3d0523a50ecdc381a98da93a511ac95f4a6b7a9c3192ebe21bbc27fe005bc413c1c10a1995b24078dd05727d55da602ba93a392769349993552a157f5699f16"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_224>> {
            hex_string {"9e7e20ab14485f3638d7c21057ba5014b64bc6b555c0c07f"},
            hex_string {"a5123d9e723137f2f735d16f"},
            hex_string {"fab61ef10db3d14a61ee7c4923fd1dce1b0de0c74f3b1b5a"}, hex_string {}, hex_string {},
            hex_string {"1d5adbf9a0dcbc7754b669d49c4b36ad5cdf12b8a1ed0854797d11636f9c901a4be0fb295c459"
                "cfc36293fabd40e90419b6908f41fbacc4d8569731f376c35d78634530827173dd7db7eccf61678357613"
                "3d6019b869ebae4f21524092fffd9b0909dec7e49fa3e465d07ae5103f0892"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_224>> {
            hex_string {"57ec3fe9533e71f46cb381364c7e7bb3be15c16b8a484a80"},
            hex_string {"0588d36a9b96207b3c169500"}, hex_string {},
            hex_string {"e557d72e18467da1256c35ff5f04ad049ace3a71b8149b55"},
            hex_string {"ff296d65a9733440c2970f2eb0f98b52500d5d87819200e4"},
            hex_string {"b8110a77cf2a6d5bff616f13d203c58b1c3fc982c2e6d1717cda266be36bfbddc4680e6881244a92b2a9"
                "eb2fedde45e70c8236b20ffc507c8b34e6b45147f85505f9237c23d5b6bd9ce2e69ef559fb0a42e26c42bbf64cdf"
                "4d8ae02b9e7e033abe49bde1a49f857565b1b65d66dc8bfe"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, HMAC_SHA2_224) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_224>> {
            hex_string {"a76e77a969ab92645181f0157802523746c34bf321867641"},
            hex_string {"051ed6ba39368033adc93d4e"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"8925987db5566e60520f09bdddab488292bed92cd385e5b6fc223e1919640b4e34e34"
                "575033e56c0a8f608be21d3d221c67d39abec98d81312f3a2653d55ffbf44c337c82bed314c"
                "211be23ec394399ba351c4687dce649e7c2a1ba7b0b5dab125671b1bcf9008da65cad612d95ddc92"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_224>> {
            hex_string {"f6e68bb0585c84d7b9f17579ad9b9a8aa2666abf4e8b44a3"},
            hex_string {"a43311d57842ef096b66fa5e"},
            hex_string {"2f507e12d68a880fa70d6e5e54391538173297814e06d7fd"}, hex_string {}, hex_string {},
            hex_string {"10c2f93ca99a8e8ecf225400c804a7b368d93cee3bfa6f445920a6a912d268d691f1788baf013fb168501"
                "ca156b571ba047d8d029dc1c1ee07fca50af699c5bc2f790acf278041518141e7dc9164c3e571b265fb8954261d92"
                "dbf20ae02fc2b780c018b6b54b4320f2b89d343307fbb2"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_224>> {
            hex_string {"c5c89c26ac4ca8b1106ba90a8ef4d6d687dfd88743caa5fb"},
            hex_string {"afa4745d9c1f8371120b10c8"}, hex_string {},
            hex_string {"d3483ae5f9ed97efd3f852e4a6f20f25c947a03f39a4b75c"},
            hex_string {"2cd523c5958cdf403caa61abe5c4739cdb9d40152f0e769a"},
            hex_string {"1fef4e6abc2778d1c3e3ce00fdb5eae1ebebdd5cff0a7087644c8565d1e8b876b2c05264ca81498468851"
                "fc7b9e5a2163a06f377d2ed754c095adc59dc015a77edd69e4eecbe48d9dc127eedfff5cc73ae38127ae3a518fe7f"
                "a5abd1a9c53eeaf144420873341e2efa3d81493c69b04e"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, Hash_SHA2_256) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_256>> {
            hex_string {"a65ad0f345db4e0effe875c3a2e71f42c7129d620ff5c119a9ef55f05185e0fb"},
            hex_string {"8581f9317517276e06e9607ddbcbcc2e"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"d3e160c35b99f340b2628264d1751060e0045da383ff57a57d73a673d2b8d80daaf6a6c35a91bb45"
                "79d73fd0c8fed111b0391306828adfed528f018121b3febdc343e797b87dbb63db1333ded9d1ece177cfa6b7"
                "1fe8ab1da46624ed6415e51ccde2c7ca86e283990eeaeb91120415528b2295910281b02dd431f4c9f70427df"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_256>> {
            hex_string {"dfeabab904bfe93a37bb5b1ea4a696f881ab5ab4be87ffbf2d4e8cdfaabb37fd"},
            hex_string {"a2d458b475053a0346b57fc518849ba1"},
            hex_string {"d15d5d9a4a3a41877b4ea98dbda5079ee393f6ab24105dbd70f5bf145772b15c"}, hex_string {}, hex_string {},
            hex_string {"86d8c63ed4a8a19f3429b4dd57ede5ca573e861712e631400645ceee763c37cf950bdcc4d9c886ead3f0f1bf46a63bf"
                "22bd2eb39b2dac61d2e8c8f29e26045b3db56b2265adc8152d4f736c09ee90364a1e265eb5e77b0c5988c8fa52717fd33b6da76"
                "0e78f2a7c27065227c47ac2134b95b7dadbf96e4ea2dad78ef200e174b"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_256>> {
            hex_string {"9b6d88373841458da926cc51f83922d363f0f80f90a2f5505c04033824ef7385"},
            hex_string {"82b21ff47bb5e1b33288b22f3856886b"}, hex_string {},
            hex_string {"45d21d94ae1ea460857b50b5b240d943d42160e4c12377e0f817b79e92530bc1"},
            hex_string {"ea432e31cc94c20d66fb13d1ef42a5f62b024134fc635aa1279a6179204731ca"},
            hex_string {"3d23d0fc03936766a1e1330393e8ff6211149f3d0758db038da1c833ca8e5265c2a9ff6c8e0836904c5fcd3e61b"
                "1c77d613dc6bdaf6437573a618e3e75e455338a7f9a41300da8fd2da408cf095ff7eae1686d60ce9c2f547d0515da916002"
                "01c8374b7af8a5f49a6381aaca394c65d451341a0ae1546cd57e0d9167a6b5397d"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, HMAC_SHA2_256) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_256>> {
            hex_string {"ca851911349384bffe89de1cbdc46e6831e44d34a4fb935ee285dd14b71a7488"},
            hex_string {"659ba96c601dc69fc902940805ec0ca8"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"e528e9abf2dece54d47c7e75e5fe302149f817ea9fb4bee6f4199697d04d5b89d54fbb978a15b5c443c"
                "9ec21036d2460b6f73ebad0dc2aba6e624abf07745bc107694bb7547bb0995f70de25d6b29e2d3011bb19d27676"
                "c07162c8b5ccde0668961df86803482cb37ed6d5c0bb8d50cf1f50d476aa0458bdaba806f48be9dcb8"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_256>> {
            hex_string {"5cacc68165a2e2ee20812f35ec73a79dbf30fd475476ac0c44fc6174cdac2b55"},
            hex_string {"6f885496c1e63af620becd9e71ecb824"},
            hex_string {"e72dd8590d4ed5295515c35ed6199e9d211b8f069b3058caa6670b96ef1208d0"}, hex_string {}, hex_string {},
            hex_string {"f1012cf543f94533df27fedfbf58e5b79a3dc517a9c402bdbfc9a0c0f721f9d53faf4aafdc4b8"
                "f7a1b580fcaa52338d4bd95f58966a243cdcd3f446ed4bc546d9f607b190dd69954450d16cd0e2d643706"
                "7d8b44d19a6af7a7cfa8794e5fbd728e8fb2f2e8db5dd4ff1aa275f35886098e80ff844886060da8b1e7137846b23b"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_256>> {
            hex_string {"d3cc4d1acf3dde0c4bd2290d262337042dc632948223d3a2eaab87da44295fbd"},
            hex_string {"0109b0e729f457328aa18569a9224921"}, hex_string {},
            hex_string {"3c311848183c9a212a26f27f8c6647e40375e466a0857cc39c4e47575d53f1f6"},
            hex_string {"fcb9abd19ccfbccef88c9c39bfb3dd7b1c12266c9808992e305bc3cff566e4e4"},
            hex_string {"9c7b758b212cd0fcecd5daa489821712e3cdea4467b560ef5ddc24ab47749a1f1ffdbbb118f4e62fcfca33"
                "71b8fbfc5b0646b83e06bfbbab5fac30ea09ea2bc76f1ea568c9be0444b2cc90517b20ca825f2d0eccd88e7175538b"
                "85d90ab390183ca6395535d34473af6b5a5b88f5a59ee7561573337ea819da0dcc3573a22974"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, Hash_SHA2_384) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_384>> {
            hex_string {"9ef0b00381d6c8c54d08fcadc6f5ef331134bb986373f65c6a14f553bcb6c55d"},
            hex_string {"9fce26ada7b1de39590312bd9d81c4f5"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"663ffb625e62c4eb67d7177a6abb808a9f68c2d5840f19992c11ea3a635d05b537fae1f1746c1314e1a75e141c"
                "2e094187d17b9daae1442e41d3a0d1fea94d8ef9d840111379a52e6c7ffafa7ee83b244ced129613d5b8bb089e7ea25de1"
                "c29897735cf95695043a648a2ef6fd4aa74ce8328a5550da8ddb51f98adcdc108e455603f6f18f5a50016f3e8ebcb244a1"
                "6bc6b6e554a7546153c12f522c75ca5f1017e01da36650e6203f30ed5c3da3b6078736465eecb400eeaaa2c876e37564d8"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_384>> {
            hex_string {"d1b31487d6de11389313b9fcb6d47a80068e27e7dd4f5d19bcdbb5ba7737e433"},
            hex_string {"97c18a1c1bd7ef912be91521be7309dd"},
            hex_string {"a7a2a558852c4b63be3502e8e636ee918d6d4689a647c91d50e3bf0b219f71ce"}, hex_string {}, hex_string {},
            hex_string {"e28462f6504456bfb2e6b96dd6a6a4d07feecb3eeb2ae93f6f12e0edc476caa9407d0ea113553038ee14"
                "077d368a589a20328a9ca3900c5b7b55fcbbfee5955e66aab957ecda339bc06372e152f7a150069404c20ac3829a"
                "c83559eec8c47e4cd297d091611b9c836d143c228726a8a9db34183c5a88ce18ea0f26c0aa166aa3f0d20e261221"
                "7d6d1088b28e09e4560970206a381bfcb1fcb2d623b0adc6e4090b3f85631281a37e755cb8b0f03eb9e0b6436034885919933d24cf8987b03d1c"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_384>> {
            hex_string {"a84fb1a153b2fb6dfd6c8c01050028ed6ffcfda7438fa463dfcb6088424031ee"},
            hex_string {"e8ab86143a21f103d2379ad2afdabaa7"},
            hex_string {"a061161d44625d4bc00c369eac66fd3690c7c6468c3584ec272db8e54fb09229"},
            hex_string {"b0f2fc219591ad6f67d3c43e04de22740fd63ce367a2685b1c591574b53b1811"},
            hex_string {"08c623db0ca9d92d493d306aa9c3c952ce2e15540c8921e9747aa3a36e5548f3"},
            hex_string {"df8dd6d322da1b88432dce878f10445f86efc23afb3c5200b392f5175f9d474cfd38005da15a2f701fe6fc8a7c9"
                "d69c9e3aa08e59e6c9fa414708bf0d40bad1917f1352915669f1a2ed8fd1af2c9edc75ddb66b568a8b8b44acefdcdad5318"
                "32f452bc7e38259f7ad8f7dff1121ad0aaf97921c6b5dd8281ff21066513dd82718e8643fcca8ac51e3afd643d1f964a32b"
                "ef160e44696de970022000a56d7154c0243dfc4c93ae79e17e3ddde4d885414d38def6dc16a4effe71167b3c1e4c1a5"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, HMAC_SHA2_384) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_384>> {
            hex_string {"a1dc2dfeda4f3a1124e0e75ebfbe5f98cac11018221dda3fdcf8f9125d68447a"},
            hex_string {"bae5ea27166540515268a493a96b5187"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"228293e59b1e4545a4ff9f232616fc5108a1128debd0f7c20ace837ca105cbf24c0dac1f9847dafd0d05"
                "00721ffad3c684a992d110a549a264d14a8911c50be8cd6a7e8fac783ad95b24f64fd8cc4c8b649eac2b15b363e3"
                "0df79541a6b8a1caac238949b46643694c85e1d5fcbcd9aaae6260acee660b8a79bea48e079ceb6a5eaf4993a82c"
                "3f1b758d7c53e3094eeac63dc255be6dcdcc2b51e5ca45d2b20684a5a8fa5806b96f8461ebf51bc515a7dd8c5475c0e70f2fd0faf7869a99ab6c"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_384>> {
            hex_string {"2cd968bacda2bc314d2fb41fe43354fb761134eb19eec60431e2f36755b85126"},
            hex_string {"e3dedf2af9382a1e652143e952212d39"},
            hex_string {"59fa8235108821accbd3c14eaf76856d6a07f43383db4cc6038040b18810d53c"}, hex_string {}, hex_string {},
            hex_string {"06051ce6b2f1c34378e08caf8fe836201ff7ec2db8fc5a2519add2524d90470194b247af3a34a673298e5"
                "7070b256f59fd098632768e2d55137d6c17b1a53fe45d6ed0e31d49e64820db145014e2f038b69b7220e042a8efc9"
                "8985706ab9635451230a128aee801d4e3718ff59511c3f3ff1b20f109774a8ddc1fadf41afcc13d40096d99794885"
                "7a894d0ef8b3235c3213ba85c50c2f3d61b0d104eccfcf36c35fe5e49e7602cb1533de12f0bec613a0ed9633821957e5b7cb32f60b7c02fa4"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_384>> {
            hex_string {"5e919d353357671566d2c6ab6e1acd46f47d0c878fe36114d7fea9fecb88a3a2"},
            hex_string {"7efca9e3d1e1b09d7f16832f3af75141"}, hex_string {},
            hex_string {"442f17cb3cb1482a19729bfd58f46f6ef16285554892c01b0718968d6e011082"},
            hex_string {"f9557c93eb841bfd7b5d4b71da928efcbe3f55e1870493ef90d16eb238380d65"},
            hex_string {"36902134f1989cfe7eb518a56c06aada98997d9bacd04aee21f879a57b515ca3b5e0c2d5fed05ca1a8b054e8c4"
                "6b389d9d9186feb0abe8e2e60b3a267281cc5b4b7341116ced35a0e07bc2b0330bbfd8b07f07248fa6d8fc5c9df1344532"
                "4162bdfa22a91ba71453ab123c92f91c70b8bd540b3b180b11ab45ae2c59e57c7c43dab7576594959a96eb502d182267c8"
                "6576b1846ccee1a694cabdfb42e0c8214192efb502926fa3c27eed020b7cc8866a5af9d838a57e78bf7acd230e1f4d8361"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, Hash_SHA2_512) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_512>> {
            hex_string {"6b50a7d8f8a55d7a3df8bb40bcc3b722d8708de67fda010b03c4c84d72096f8c"},
            hex_string {"3ec649cc6256d9fa31db7a2904aaf025"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"95b7f17e9802d3577392c6a9c08083b67dd1292265b5f42d237f1c55bb9b10bfcfd82c77a378b8266a00991"
                "43b3c2d64611eeeb69acdc055957c139e8b190c7a06955f2c797c2778de940396a501f40e91396acf8d7e45ebdbb53b"
                "bf8c975230d2f0ff9106c76119ae498e7fbc03d90f8e4c51627aed5c8d4263d5d2b978873a0de596ee6dc7f7c29e37e"
                "ee8b34c90dd1cf6a9ddb22b4cbd086b14b35de93da2d5cb1806698cbd7bbb67bfe3d31fd2d1dbd2a1e058a3eb99d7e5"
                "1f1a938eed5e1c1de23a6b4345d3191409f92f39b3670d8dbfb635d8e6a36932d81033d1448d63b403ddf88e121b6e8"
                "19ac381226c1321e4b08644f6727c368c5a9f7a4b3ee2"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_512>> {
            hex_string {"67d492360c69fd41aca0ac52f5e2ba1820a5e73fb5fe5dbcd00bb9ea05af0523"},
            hex_string {"5e7d69e187577b0433eee8eab9f77731"},
            hex_string {"22e4e18124ef50ae514d5146479d83f0be23c5c4df4ba208e5e5b3506d3e104e"}, hex_string {}, hex_string {},
            hex_string {"f7aa49abb823c6c41e99e782d098821b9f4029790c701d015b356a1b7c655ff1553180e20cdda09c8286493"
                "3a079cb81ef033d356ed011ad2777dcca17666127c230198c89f1f372fddb307614d062187f0b4099101617d5c2e1b4"
                "792b1d91bf5eec60ba1dbf20e7b070379c3a097a98a043a583718101c052f29dc281d1f666494e11d5f80cded4e6a93"
                "85143d0bf33e7d687f8204cc97f57d9f0ddfc205a8efe2787e8f49575ebf83fabf585840212dc6fce1c54df92608b01"
                "e7d36538d9ef2b8a6b8910daa3c8ccc72f284cc2f2573412085d232e29eabbe61b27eaa2ed485059198d0ae57bb35a7"
                "fa66492c12e782c5774e1abeb202e0744e9d766f2f133"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::Hash_DRBG<hash::SHA2_512>> {
            hex_string {"9c96a34f68689b8aa8d9c1f6cd0fa7c6f96071caf1bf5556f45bdbf48c6cf0c6"},
            hex_string {"885c2539046afb1401eb7a5c84dbd9c2"}, hex_string {},
            hex_string {"cb61c4f75c01b578aa233a0bae4881c0a11527c22fe7b34fb6ae62eebcfe6085"},
            hex_string {"c066fd2eb8e4aea2e7145eda0cfc8bef5eedcc367b1cb4de7eb2c2759fa75bf7"},
            hex_string {"782c208ed58044e78b5bbbd8772a3caf25b47d36afeb0d3493c43e01cc66a0ca2fac"
                "ed2ab186bc46825d989cf8ee7c95f8c0b0d2b76e6c8590e72834d4c52445aeceeb7bf5f5d9ac"
                "44a12cbd3fa7f4462f856452dc4a929182d2388aa7635b9698a912585df7f560adc5080d53b8"
                "2bbd7e9e480b00d1da5bb2d480cae2ba8c67d4bf3bfd146a91d6aab39faae1600af2ce3204ca"
                "bf4c1caee4cfd5e6f8db1902033f7f8d33bc6e0e5d32a320ba735d091f30867b7cb7880c2e3c"
                "e6aada79664191df360d35fe9ae7babca41485b06ab49dff528782fbe6f2b0e74996e9ce9272"
                "d1ef392be5c17cc62c74be504e6a8731dd9548b0db27e0b7db4886f537883623"}
        }.run ();

        // reseed before generate calls.

    }

    TEST (NIST_DRBG, HMAC_SHA2_512) {
        // no reseed, no additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_512>> {
            hex_string {"35049f389a33c0ecb1293238fd951f8ffd517dfde06041d32945b3e26914ba15"},
            hex_string {"f7328760be6168e6aa9fb54784989a11"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"e76491b0260aacfded01ad39fbf1a66a88284caa5123368a2ad9330ee48335e3c9c9ba90e6cbc9429962d60c1a666"
                "1edcfaa31d972b8264b9d4562cf18494128a092c17a8da6f3113e8a7edfcd4427082bd390675e9662408144971717303d8dc3"
                "52c9e8b95e7f35fa2ac9f549b292bc7c4bc7f01ee0a577859ef6e82d79ef23892d167c140d22aac32b64ccdfeee2730528a38"
                "763b24227f91ac3ffe47fb11538e435307e77481802b0f613f370ffb0dbeab774fe1efbb1a80d01154a9459e73ad361108bbc"
                "86b0914f095136cbe634555ce0bb263618dc5c367291ce0825518987154fe9ecb052b3f0a256fcc30cc14572531c9628973639beda456f2bddf6"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_512>> {
            hex_string {"73529bba71a3d4b4fcf9a7edeed269dbdc3748b90df68c0d00e245de54698c77"},
            hex_string {"22e2d6e24501212b6f058e7c54138007"},
            hex_string {"e2cc19e31595d0e4de9e8bd3b236dec2d4b032c3dd5bf9891c284cd1bac67bdb"}, hex_string {}, hex_string {},
            hex_string {"1a73d58b7342c3c933e3ba15eedd8270988691c3794b45aa358570391571881c0d9c4289e5b198db55"
                "34c3cb8466ab48250fa67f24cb19b7038e46af56687bab7e5de3c82fa7312f54dc0f1dc93f5b03fcaa6003cae2"
                "8d3d4707368c144a7aa46091822da292f97f32caf90ae3dd3e48e808ae12e633aa0410106e1ab56bc0a0d80f43"
                "8e9b3492e4a3bc88d73a3904f7dd060c48ae8d7b12bf89a19551b53b3f55a511d2820e941640c845a8a0466432"
                "c5850c5b61bec5272602521125addf677e949b96782bc01a904491df08089bed004ad56e12f8ea1a200883ad72"
                "b3b9fae12b4eb65d5c2bacb3ce46c7c48464c9c29142fb35e7bc267ce852296ac042f9"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::HMAC_DRBG<hash::SHA2_512>> {
            hex_string {"a3da06bc88e2f2ea5181292c194a10b3db38a11d02ac2f9c65951d0c71f63e36"},
            hex_string {"c74e5e3d7ba0193bcd6839e9ae93d70d"}, hex_string {},
            hex_string {"dbb7270760d8d262557807ce746ff314fd06598143611ab69bfc7e10ca5784b3"},
            hex_string {"8cdea882f894e5fdc5f0a0b16b7d9ac8cde35ed17bcaf2665564d4ee74059e29"},
            hex_string {"cb706b90e88380e5c1864458454027821b571dfeba0da83f712efb107b8752099514ef87b4488fbfa3508a"
                "00954bb03090766d2bbd399e71c86c7967a4e8ded57095a29d4cfa01f8d28c97e81a4cd4fc5be7fb32a0d6c230cb87"
                "60e656b74fa7e18e2063ebee5787958b272fc5de93f0d6837e55f0c360dc593c88fff30a428cae37ded52f825646e0"
                "4133a19790c304e4b1f040e10439c5edf454e6f71b23eeb43cdbe7b0634b8e283a97806073f7f28a43de2d0d969b3e"
                "da380c185b785b9101dc905025c9cdb499e594de0f0d3eb41922c20994fe2c403dd5bf01e4b2c3ee6654d6ab9cca7d"
                "4d5ae59525a796119547eae6a3cbf8ad0e9b1de3c4d5a804e4"}
        }.run ();

        // reseed before generate calls.
    }

    // TODO 512/224 and 512/256

    template <size_t key_size, typename C> struct NIST_test_case_no_reseed<NIST::CTR_DRBG<key_size, C, false>> {
        using DRBG = NIST::CTR_DRBG<key_size, C, false>;

        hex_string EntropyInput;
        hex_string Personalization;
        hex_string AdditionalInput1;
        hex_string AdditionalInput2;
        hex_string Returned;

        void run () {
            DRBG drbg {bytes {EntropyInput}, bytes (Personalization)};

            bytes returned (Returned);
            bytes generated;
            generated.resize (returned.size ());

            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput1));
            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput2));

            EXPECT_EQ (generated, returned) << "Expected to generate " << returned << " but got " << generated ;
        }
    };

    template <size_t key_size, typename C> struct NIST_test_case_no_pr<NIST::CTR_DRBG<key_size, C, false>> {
        using DRBG = NIST::CTR_DRBG<key_size, C, false>;

        hex_string EntropyInput;
        hex_string Personalization;
        hex_string EntropyReseed;
        hex_string AdditionalReseed;
        hex_string AdditionalInput1;
        hex_string AdditionalInput2;
        hex_string Returned;

        void run () {
            DRBG drbg {bytes (EntropyInput), bytes (Personalization)};

            drbg.reseed (bytes (EntropyReseed), bytes (AdditionalReseed));

            bytes returned (Returned);
            bytes generated;
            generated.resize (returned.size ());

            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput1));
            drbg.generate (generated.data (), generated.size (), bytes (AdditionalInput2));

            EXPECT_EQ (generated, returned) << "Expected to generate " << returned << " but got " << generated ;
        }
    };

    TEST (NIST_DRBG, CTR_TDEA3_No_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3, false>> {
            hex_string {"4cd97f1701716d1a22f90b55c569c8f2b91aa53322653dcae809abc5c6"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"4353dd937ec55e6733cf7a5d2cea557ce8e3fcc6cdb18e44395e4b1c4669c9d1"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3, false>> {
            hex_string {"f72cd598ea873b39dc86470559a9fbce69200ee4ab949a044d3a9c0771"},
            hex_string {"732472934b1f3d94faae30c64f5544ac2c0a920ae4c03044ec90b16f0b"},
            hex_string {}, hex_string {},
            hex_string {"eb30427f847d50c89f39ab72907c257a264a149c8c19c60f59bf7ed697d2807e"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3, false>> {
            hex_string {"c9fffbe2f700f4a3a9467794077051e6ef4a7d0374e91509fd8b6fdb23"},
            hex_string {"da169b75d05a6f7c164b565d35c974a7ef3ba3df60ceca9f74877b6926"},
            hex_string {"3295d7d6e10d3fe1ea37a725c7b7b1d2ca9dbc95622021fa433d599ffc"},
            hex_string {"90f7d0e53f0f88d2bd6fe0e6e55321e61a502929f8650229d9f17c296d"},
            hex_string {"7c06cc65404e55c7af738774bf2f14fbaa6ba763393f325d57be7d693f9f08e6"}
        }.run ();

        // reseed before generate calls.
        NIST_test_case_no_pr<NIST::CTR_DRBG<21, cipher::block::TDEA3, false>> {
            hex_string {"840642214f7630391c8f16bff05a93b3e00410864a7423f41b2e9fbede"},
            hex_string {},
            hex_string {"6ec01f6c30b116b8598d8d7d06833cba6a305c4212a212614b464e1b61"},
            hex_string {"5e32b3f81d47f4b7faf51a28fdf7ddd3e0d9a4c54f6cc044ad414894d9"},
            hex_string {"dd425759aad9059888f22b8fa28d313110951f313ca412347a38edcbd4"},
            hex_string {"73b9b852d06d438c9a77d347cb8c551e9800e9c3eca4f5fe135afbb618"},
            hex_string {"b29b76cce0f010dcbcdf99d196f5073ae5f19a5c87b86112940948f648edb242"}
        }.run ();

        NIST_test_case_no_pr<NIST::CTR_DRBG<21, cipher::block::TDEA3, false>> {
            hex_string {"c201dcf55f6eaca02f74a2e090109cdf2abdf79f0cb20e89978efdb765"},
            hex_string {"20d0261bbe9f928ff940127a887ec80240b86693e0c66e2fe8bc5bc2a6"},
            hex_string {"34c31fcbe9980ce7d441555c7e7f0ce7e96bb717dae7c7a7d6b2c8af83"},
            hex_string {"67fa5c249243d6eb82ff78183c34d8d78038185eee5488961dc0d36fba"},
            hex_string {"fee8046d22c71ead3c5bbb3a23c3cdfcce169de7519c0afd9069cf605b"},
            hex_string {"488e18ea304eabcd1848424b92b8049cab631d958012cb57c19a70a3dd"},
            hex_string {"732a4c29bf785028811f08745d9442f4432189fde862d9e9b94b500d08405ce2"}
        }.run ();
    }

    TEST (NIST_DRBG, CTR_AES128_No_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES, false>> {
            hex_string {"ce50f33da5d4c1d3d4004eb35244b7f2cd7f2e5076fbf6780a7ff634b249a5fc"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"6545c0529d372443b392ceb3ae3a99a30f963eaf313280f1d1a1e87f9db3"
                "73d361e75d18018266499cccd64d9bbb8de0185f213383080faddec46bae1f784e5a"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES, false>> {
            hex_string {"cee23de86a69c7ef57f6e1e12bd16e35e51624226fa19597bf93ec476a44b0f2"},
            hex_string {"a2ef16f226ea324f23abd59d5e3c660561c25e73638fe21c87566e86a9e04c3e"}, hex_string {}, hex_string {},
            hex_string {"2a76d71b329f449c98dc08fff1d205a2fbd9e4ade120c7611c225c984eac8531288dd3049"
                "f3dc3bb3671501ab8fbf9ad49c86cce307653bd8caf29cb0cf07764"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES, false>> {
            hex_string {"6bd4f2ae649fc99350951ff0c5d460c1a9214154e7384975ee54b34b7cae0704"},
            hex_string {},
            hex_string {"ecd4893b979ac92db1894ae3724518a2f78cf2dbe2f6bbc6fda596df87c7a4ae"},
            hex_string {"b23e9188687c88768b26738862c4791fa52f92502e1f94bf66af017c4228a0dc"},
            hex_string {"5b2bf7a5c60d8ab6591110cbd61cd387b02de19784f496d1a109123d8b3562a5de2dd6d5"
                "d1aef957a6c4f371cecd93c15799d82e34d6a0dba7e915a27d8e65f3"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, CTR_AES192_No_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES, false>> {
            hex_string {"f1ef7eb311c850e189be229df7e6d68f1795aa8e21d93504e75abe78f041395873540386812a9a2a"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"6bb0aa5b4b97ee83765736ad0e9068dfef0ccfc93b71c1d3425302ef7ba4635ffc0998"
                "1d262177e208a7ec90a557b6d76112d56c40893892c3034835036d7a69"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES, false>> {
            hex_string {"d76697495add9baac8cfdf90e0a6f3381a52b2889f840a24cfc9606f97c49446202e70583fde09dc"},
            hex_string {"d64e546523fb082b5e87bbe6b66a8e6a7940bb06ed17fe833c4e9bbb211396c227f83671c924240e"},
            hex_string {}, hex_string {},
            hex_string {"5693f68ec7a8d2631dee9d371503e05cbd5fb872506f040050b1d441a94"
                "bd74ec06f24c1cc4363ece1d8b2295e74389b6dd8b1e5438bceab2f2bc2af5a9d8da8"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES, false>> {
            hex_string {"b895b891f039052821fbb4a889fced861b96c37e36a5f4f7aa208a2bf33a896fe7e29f3f6cf0041f"},
            hex_string {},
            hex_string {"8711df3931a9035905ebdf510b3ea3f344923b2f20a561709c0def03b9570be267e9765719a25d8a"},
            hex_string {"03908b9fd56bad5a1645b688a4f9c2342bc0f81fed6f7449af14e92960d60127dda4e27487e77491"},
            hex_string {"1385397b1a245bd06bbb4bcf651a52b2d30867d3e2f98e9c7a9ee959d2e1bbd63a10054f"
                "c081cd22a7f3b65ed2f0b3a3deb389d2f336e50b359a6c3e83667fb1"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, CTR_AES256_No_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<32, cipher::block::AES, false>> {
            hex_string {"df5d73faa468649edda33b5cca79b0b05600419ccb7a879ddfec9db32ee494e5531b51de16a30f769262474c73bec010"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"d1c07cd95af8a7f11012c84ce48bb8cb87189e99d40fccb1771c619bdf82ab2280b1dc2"
                "f2581f39164f7ac0c510494b3a43c41b7db17514c87b107ae793e01c5"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<32, cipher::block::AES, false>> {
            hex_string {"df5d73faa468649edda33b5cca79b0b05600419ccb7a879ddfec9db32ee494e5531b51de16a30f769262474c73bec010"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"d1c07cd95af8a7f11012c84ce48bb8cb87189e99d40fccb1771c619bdf82ab2280b1dc2"
                "f2581f39164f7ac0c510494b3a43c41b7db17514c87b107ae793e01c5"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<32, cipher::block::AES, false>> {
            hex_string {"f45e9d040c1456f1c7f26e7f146469fbe3973007fe037239ad57623046e7ec52221b22eec208b22ac4cf4ca8d6253874"},
            hex_string {},
            hex_string {"28819bc79b92fc8790ebdc99812cdcea5c96e6feab32801ec1851b9f46e80eb6800028e61fbccb6ccbe42b06bf5a0864"},
            hex_string {"418ca848027e1b3c84d66717e6f31bf89684d5db94cd2d579233f716ac70ab66cc7b01a6f9ab8c7665fcc37dba4af1ad"},
            hex_string {"4f11406bd303c104243441a8f828bf0293cb20ac39392061429c3f56c1f4262"
                "39f8f0c687b69897a2c7c8c2b4fb520b62741ffdd29f038b7c82a9d00a890a3ed"}
        }.run ();

        NIST_test_case_no_reseed<NIST::CTR_DRBG<32, cipher::block::AES, false>> {
            hex_string {"0dd4d80062ecc0f359efbe7723020be9b88b550fe74088094069e74428395856f63eed4f5b0e7d1e006f0eaff74f638c"},
            hex_string {"d2aa2ccd4bc6537e51f6550ab6d6294547bef3e971a7f128e4436f957de9982c93ee22110b0e40ab33a7d3dfa22f599d"},
            hex_string {"0b081bab6c74d86b4a010e2ded99d14e0c9838f7c3d69afd64f1b66377d95cdcb7f6ec5358e3516034c3339ced7e1638"},
            hex_string {"ca818f938ae0c7f4f507e4cfec10e7baf51fe34b89a502f754d2d2be7395120fe1fb013c67ac2500b3d17b735da09a6e"},
            hex_string {"6808268b13e236f642c06deba2494496e7003c937ebf6f7cb7c9210"
                "4ea090f18484aa075560d7844a06eb559948c93b26ae40f2db98ecb53ad593eb4c78f82b1"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, CTR_AES128_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES>> {
            hex_string {"890eb067acf7382eff80b0c73bc872c6"},
            hex_string {"aad471ef3ef1d203"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"a5514ed7095f64f3d0d3a5760394ab42062f373a25072a6ea6bcfd8489e9"
                "4af6cf18659fea22ed1ca0a9e33f718b115ee536b12809c31b72b08ddd8be1910fa3"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES>> {
            hex_string {"e10bc28a0bfddfe93e7f5186e0ca0b3b"},
            hex_string {"9ff477c18673840d"},
            hex_string {"c980dedf9882ed4464a674967868f143"}, hex_string {}, hex_string {},
            hex_string {"35b00df6269b6641fd4ccb354d56d851de7a77527e034d60c9e1a9e1525"
                "a30ed361fded89d3dccb978d4e7a9e100ebf63062735b52831c6f0a1d3e1bdc5ebc72"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES>> {
            hex_string {"b408cefb5bc7157d3f26cb95a8b1d7ac"},
            hex_string {"026c768fd577b92a"}, hex_string {},
            hex_string {"5737ef81dee365b6dadb3feebf5d1084"},
            hex_string {"3368a516b3431a3daaa60dc8743c8297"},
            hex_string {"4e909ebb24147a0004063a5e47ee044fead610d62324bd0f963f756fb91361e8b87e3a"
                "76a398143fe88130fe1b547b661a6480c711b739f18a9df3ae51d41bc9"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, CTR_TDEA3_DF) {

        NIST_test_case_no_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3>> {
            hex_string {"b54d0bbaa78adf0915d3dd83ed3f"},
            hex_string {"11d849d2de4b58"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"88b166404866fd6b1168c4472163e3cb3d5c9cca2e5abfa2faf929025f21fed1"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3>> {
            hex_string {"6f4ba8e51f9cd050d0d53b874ff4"},
            hex_string {"02ce963abfd52d"},
            hex_string {"ca85a0c6f15b5927e3e77a6857f3"}, hex_string {}, hex_string {},
            hex_string {"f47af2daee77cc3dbfffb98dc0373eb8e858a84b9e231383c01b02642ece4c96"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<21, cipher::block::TDEA3>> {
            hex_string {"8daeccde428e64782799d123ccf0"},
            hex_string {"584d044e74ec1f"},
            hex_string {"cdcc5d5f5803f488d40d8f12353a"},
            hex_string {"dd98133a88d7c1d14d4b78b67e18"},
            hex_string {"672626d5473832f10209453f2038"},
            hex_string {"fd4db76cd179bed2c28b47c18bc52e1ca5508c09afcf94cd9f43580e02d0e841"}
        }.run ();

        // reseed before generate calls.
        NIST_test_case_no_pr<NIST::CTR_DRBG<21, cipher::block::TDEA3>> {
            hex_string {"dedfcf34617ac04ff579b87ca18f"},
            hex_string {"40bce1b72deaea"}, hex_string {},
            hex_string {"876beead5bdf5c206ace3bfa05d9"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"f87cfa1529fdaf57ee1542844e556f00ff7fc70e3267372b56dce6141e124f85"}
        }.run ();
    }

    TEST (NIST_DRBG, CTR_AES192_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES>> {
            hex_string {"c35c2fa2a89d52a11fa32aa96c95b8f1c9a8f9cb245a8b40"},
            hex_string {"f3a6e5a7fbd9d3c68e277ba9ac9bbb00"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"8c2e72abfd9bb8284db79e17a43a3146cd7694e35249fc3383914a7117f41"
                "368e6d4f148ff49bf29076b5015c59f457945662e3d3503843f4aa5a3df9a9df10d"}
        }.run ();

        // with a personalization string
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES>> {
            hex_string {"eb4553f7141bc10882f0a3741780a42df8bc38a7121d3b7e"},
            hex_string {"6f347f9c1de84fd5341625ae8d6bf50c"},
            hex_string {"5e2e73b86ca2f3150d53d23d590acbeedaaf91638bdc3f9d588e945af4bb6ea2"}, hex_string {}, hex_string {},
            hex_string {"a1deb9a5aad608a587d61ce5e0d7c7dd449b8c87898354ad1add6e05"
                "853873d279ebe4132fc236a42d8ff0dc3ace95d2cdf9d0b057117cb119ee7550ce03085c"}
        }.run ();

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES>> {
            hex_string {"eb4553f7141bc10882f0a3741780a42df8bc38a7121d3b7e"},
            hex_string {"6f347f9c1de84fd5341625ae8d6bf50c"},
            hex_string {"5e2e73b86ca2f3150d53d23d590acbeedaaf91638bdc3f9d588e945af4bb6ea2"}, hex_string {}, hex_string {},
            hex_string {"a1deb9a5aad608a587d61ce5e0d7c7dd449b8c87898354ad1add6e05"
                "853873d279ebe4132fc236a42d8ff0dc3ace95d2cdf9d0b057117cb119ee7550ce03085c"}
        }.run ();

        // reseed before generate calls.
    }

    TEST (NIST_DRBG, CTR_AES256_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<32, cipher::block::AES>> {
            hex_string {"36401940fa8b1fba91a1661f211d78a0b9389a74e5bccfece8d766af1a6d3b14"},
            hex_string {"496f25b0f1301b4f501be30380a137eb"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"5862eb38bd558dd978a696e6df164782ddd887e7e9a6c9f3f1fbafb78941b535a"
                "64912dfd224c6dc7454e5250b3d97165e16260c2faf1cc7735cb75fb4f07e1d"}
        }.run ();

        // with a personalization string

        // no reseed with additional input on generate.
        NIST_test_case_no_reseed<NIST::CTR_DRBG<32, cipher::block::AES>> {
            hex_string {"6b536f8f4c5616a0c4e681825585724d522d37fc02a05564030872e0d6c3922f"},
            hex_string {"0184417527ffdd3edddbb070957826c0"},
            hex_string {"91e62a609b4db50c5e7ad7d09dc387dae9da6d2585bd3530389411cea7d2a40e"},
            hex_string {"42f398bf2229976f9d97b0a5fc47d5c64b70fa5631abf28f2c6f91f78b7278d9"},
            hex_string {"c624291eb039ad1724c9b0ba20b98421a7f0032f6c8c00f64794018ce5a5ed96"},
            hex_string {"507e0b4f12c408d87052b79eb4879c925a918b0fcd812bbedc720a3d8be656"
                "e40de900257f7a270dd6d8e7da50cdc20d744e94978d707b53f382aeb16488b122"}
        }.run ();

        // reseed before generate calls.
        NIST_test_case_no_pr<NIST::CTR_DRBG<32, cipher::block::AES>> {
            hex_string {"5bb14bec3a2e435acab8b891f075107df387902cb2cd996021b1a1245d4ea2b5"},
            hex_string {"12ac7f444e247f770d2f4d0a65fdab4e"},
            hex_string {"2e957d53cba5a6b9b8a2ce4369bb885c0931788015b9fe5ac3c01a7ec5eacd70"},
            hex_string {"19f30c84f6dbf1caf68cbec3d4bb90e5e8f5716eae8c1bbadaba99a2a2bd4eb2"},
            hex_string {}, hex_string {},
            hex_string {"b7dd8ac2c5eaa97c779fe46cc793b9b1e7b940c318d3b531744b42856f298264e45f9a0a"
                "ca5da93e7f34f0ebc0ed0ea32c009e3e03cf01320c9a839807575405"}
        }.run ();
    }
}
