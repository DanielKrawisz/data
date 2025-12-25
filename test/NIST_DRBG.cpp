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
        EXPECT_NO_THROW ((R {bytes (good_bytes), byte_slice {}}));/*
        R r {bytes (good_bytes), byte_slice {}};
        EXPECT_THROW (r.reseed (bytes (bad_bytes)), NIST::insufficient_entropy);
        EXPECT_NO_THROW (r.reseed (bytes (good_bytes)));*/
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

        EXPECT_EQ ((NIST::CTR_DRBG<24, cipher::block::TDA3>::MaxStrength), 112);
        EXPECT_EQ ((NIST::CTR_DRBG<16, cipher::block::AES>::MaxStrength), 128);
        EXPECT_EQ ((NIST::CTR_DRBG<24, cipher::block::AES>::MaxStrength), 192);
        EXPECT_EQ ((NIST::CTR_DRBG<32, cipher::block::AES>::MaxStrength), 256);

        EXPECT_EQ ((NIST::CTR_DRBG<24, cipher::block::TDA3>::SeedLength), 232/8);
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

        test_instantiate_and_reseed<NIST::CTR_DRBG<24, cipher::block::TDA3>> ();
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

        // no reseed with additional input on generate.
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

    TEST (NIST_DRBG, CTR_AES128_No_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES, false>> {
            hex_string {"ce50f33da5d4c1d3d4004eb35244b7f2cd7f2e5076fbf6780a7ff634b249a5fc"},
            hex_string {}, hex_string {}, hex_string {},
            hex_string {"6545c0529d372443b392ceb3ae3a99a30f963eaf313280f1d1a1e87f9db3"
                "73d361e75d18018266499cccd64d9bbb8de0185f213383080faddec46bae1f784e5a"}
        }.run ();

        // with a personalization string

        // no reseed with additional input on generate.
    }
/*
    TEST (NIST_DRBG, CTR_AES128_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<16, cipher::block::AES>> {
            hex_string {"890eb067acf7382eff80b0c73bc872c6"},
            hex_string {"aad471ef3ef1d203"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"a5514ed7095f64f3d0d3a5760394ab42062f373a25072a6ea6bcfd8489e9"
                "4af6cf18659fea22ed1ca0a9e33f718b115ee536b12809c31b72b08ddd8be1910fa3"}
        }.run ();

        // with a personalization string

        // no reseed with additional input on generate.
    }

    TEST (NIST_DRBG, CTR_TDA3_DF) {


        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::TDA3>> {
            hex_string {"b54d0bbaa78adf0915d3dd83ed3f"},
            hex_string {"11d849d2de4b58"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"88b166404866fd6b1168c4472163e3cb3d5c9cca2e5abfa2faf929025f21fed1"}
        }.run ();

        // with a personalization string

        // no reseed with additional input on generate.
    }


    TEST (NIST_DRBG, CTR_AES192_DF) {
        NIST_test_case_no_reseed<NIST::CTR_DRBG<24, cipher::block::AES>> {
            hex_string {"c35c2fa2a89d52a11fa32aa96c95b8f1c9a8f9cb245a8b40"},
            hex_string {"f3a6e5a7fbd9d3c68e277ba9ac9bbb00"}, hex_string {}, hex_string {}, hex_string {},
            hex_string {"8c2e72abfd9bb8284db79e17a43a3146cd7694e35249fc3383914a7117f41"
                "368e6d4f148ff49bf29076b5015c59f457945662e3d3503843f4aa5a3df9a9df10d"}
        }.run ();

        // with a personalization string

        // no reseed with additional input on generate.
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
    }*/
}
