// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>

#include <gtest/gtest.h>

namespace data {
    
    TEST (N, StringToN) {
        
        EXPECT_THROW (N::read (""), exception);
        EXPECT_THROW (N::read ("a"), exception);
        EXPECT_THROW (N::read ("-"), exception);
        EXPECT_THROW (N::read ("-1"), exception);
        EXPECT_THROW (N::read ("01"), exception);
        EXPECT_THROW (N::read ("0x1"), exception);

        EXPECT_NO_THROW (N {0});
        EXPECT_NO_THROW (N::read ("0"));
        EXPECT_NO_THROW (N::read ("0x"));
        
        EXPECT_EQ (N {}, N::read ("0"));
        
        EXPECT_EQ (N {0}, N::read ("0"));
        EXPECT_EQ (N {0}, N::read ("0x"));
        
        EXPECT_NE (N {1}, N::read ("0"));
        EXPECT_NE (N {0}, N::read ("1"));
        
        EXPECT_EQ (N {1}, N::read ("1"));
        EXPECT_EQ (N {1}, N::read ("0x01"));
        EXPECT_EQ (N {1}, N::read ("0x0001"));
        EXPECT_EQ (N {255}, N::read ("0xff"));
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {0}), "0x");
        EXPECT_EQ (encoding::decimal::write (N {0}), "0");
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {1}), "0x01");
        EXPECT_EQ (encoding::decimal::write (N {1}), "1");
        
    }

    TEST (N, NToHexString) {
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N::read ("128")), std::string {"0x80"});
        
    }
    
    TEST (N, NNegativeHex) {
        
        EXPECT_NE (N::read ("0x8000000000000000000000000000000000000000000000000000000000000000"), N (0));
        EXPECT_NE (N::read ("0x80000000000000000000000000000000000000000000000000000000000000"), N (0));
        EXPECT_NE (N::read ("0x80000000000000000000000000000000000000000000000000000000"), N (0));
        EXPECT_NE (N::read ("0x8000000000000000000000000000000000000000000000"), N (0));
        EXPECT_NE (N::read ("0x8000000000000000000000000000"), N (0));
        EXPECT_NE (N::read ("0x8000000000"), N (0));
        EXPECT_NE (N::read ("0x8000"), N (0));
        EXPECT_NE (N::read ("0x80"), N (0));
        
    }

    template<endian r, std::unsigned_integral word>
    N N_Bytes_to_N_stupid (const math::number::N_bytes<r, word> &n) {
        N x {0};

        for (const word &b : n.words ().reverse ()) {
            x <<= (sizeof (word) * 8);
            x += b;
        }

        return x;
    }

    template<endian r, std::unsigned_integral word>
    math::number::N_bytes<r, word> inline N_to_N_Bytes_stupid (const N &n) {
        auto hex_string = encoding::hexidecimal::write<hex_case::lower> (n);
        size_t bytes_encoded = (hex_string.size () - 2) / 2;

        size_t bytes_extended = bytes_encoded % sizeof (word) == 0 ? bytes_encoded :
            ((bytes_encoded / sizeof (word)) + 1) * sizeof (word);

        auto hex_resized = encoding::hexidecimal::extend<negativity::nones, hex_case::lower> (hex_string, bytes_extended * 2 + 2);

        return math::number::N_bytes<r, word>::read (hex_resized);
    }

    template <std::unsigned_integral word> using Nl = math::number::N_bytes<endian::little, word>;
    template <std::unsigned_integral word> using Zl1 = math::number::Z_bytes<endian::little, negativity::twos, word>;
    template <std::unsigned_integral word> using Zl2 = math::number::Z_bytes<endian::little, negativity::BC, word>;
    template <std::unsigned_integral word> using Nb = math::number::N_bytes<endian::big, word>;
    template <std::unsigned_integral word> using Zb1 = math::number::Z_bytes<endian::big, negativity::twos, word>;
    template <std::unsigned_integral word> using Zb2 = math::number::Z_bytes<endian::big, negativity::BC, word>;

    template <typename in, std::unsigned_integral word> void N_Bytes_to_N_by_word (in x) {

        N n = N::read (x);

        Nb<word> big {x};
        Nl<word> little {x};

        Nb<word> stupid_big = N_to_N_Bytes_stupid<endian::big, word> (n);
        Nl<word> stupid_little = N_to_N_Bytes_stupid<endian::little, word> (n);

        EXPECT_EQ (stupid_big, big) << "expected " << std::hex << stupid_big << " to equal " << big << "; input = " << x;
        EXPECT_EQ (stupid_little, little) << "expected " << std::hex << stupid_little << " to equal " << little << "; input = " << x;

        N N_big = N (big);
        N N_little = N (little);

        N N_big_stupid = N_Bytes_to_N_stupid<endian::big, word> (big);
        N N_little_stupid = N_Bytes_to_N_stupid<endian::little, word> (little);

        EXPECT_EQ (N_big_stupid, N_big) << "expected " << std::hex << N_big_stupid << " to equal " << N_big << "; input = " << x;
        EXPECT_EQ (N_little_stupid, N_little) << "expected " << std::hex << N_little_stupid << " to equal " << N_little << "; input = " << x;

        EXPECT_EQ (N_big, n);
        EXPECT_EQ (N_little, n);

    }

    template <typename in> void N_Bytes_to_N (in x) {

        N_Bytes_to_N_by_word<in, byte> (x);
        N_Bytes_to_N_by_word<in, unsigned short> (x);
        N_Bytes_to_N_by_word<in, unsigned> (x);
        N_Bytes_to_N_by_word<in, unsigned long> (x);
        N_Bytes_to_N_by_word<in, unsigned long long> (x);

    }

    TEST (N, NBytesToN) {

        N_Bytes_to_N<string> ("0");
        N_Bytes_to_N<string> ("1");
        N_Bytes_to_N<string> ("3");
        N_Bytes_to_N<string> ("767");
        N_Bytes_to_N<string> ("7439");
        N_Bytes_to_N<string> ("10920960978709");

        N_Bytes_to_N<string> (
            "0x0fabcdef123456789012323454567600000a00aabbccddeeffffffffffffffff"
            "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff"
            "0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"
            "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcd");

        N_Bytes_to_N<string> (
            "0xf0abcdef123456789012323454567600000a00aabbccddeeffffffffffffffff"
            "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff"
            "0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"
            "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcd");

    }

    void test_N_to_N_bytes (string x) {
        EXPECT_EQ ((math::N_bytes<endian::big, byte> {N::read (x)}), (math::N_bytes<endian::big, byte>::read (x)));
        EXPECT_EQ ((math::N_bytes<endian::little, byte> {N::read (x)}), (math::N_bytes<endian::little, byte>::read (x)));
    }

    TEST (N, NToNBytes) {

        test_N_to_N_bytes ("0");
        test_N_to_N_bytes ("1");
        test_N_to_N_bytes ("23");
        test_N_to_N_bytes ("5704566599993321");
        test_N_to_N_bytes ("98980987676898761029390303474536547398");
        test_N_to_N_bytes ("98980987676898761029390303474536547399");
        test_N_to_N_bytes ("98980987676898761029390303474536547400");

    }
    
    TEST (N, Multiply) {
        
        EXPECT_TRUE (N {0} * N {0} == N {0});
        EXPECT_TRUE (N {1} * N {0} == N {0});
        EXPECT_TRUE (N {0} * N {1} == N {0});
        EXPECT_TRUE (N {1} * N {1} == N {1});
        
        N n {0};
        
        EXPECT_TRUE (n == N {0});
        
        n++;
        
        EXPECT_TRUE (n == N {1});
        
        n += 1;
        
        EXPECT_TRUE (n == N {2});
        
    }
    
    TEST (N, Shift) {
        
        EXPECT_TRUE (N {1} >>  1 == N::read ("0x00000000000000000000"));
        EXPECT_TRUE (N {1} <<  1 == N::read ("0x00000000000000000002"));
        EXPECT_TRUE (N {1} <<  3 == N::read ("0x00000000000000000008"));
        EXPECT_TRUE (N {1} <<  7 == N::read ("0x00000000000000000080"));
        EXPECT_TRUE (N {1} << 79 == N::read ("0x80000000000000000000"));
        
    }
        
    template<endian r> using N_bytes = math::number::N_bytes<r, byte>;
    
    TEST (N, N) {
        
        // parameters from https://en.bitcoin.it/wiki/Secp256k1
        
        N p = (N {1u} << 256) - (N {1u} << 32) - (N {1u} << 9) - (N {1u} << 8) - (N {1u} << 7) - (N {1u} << 6) - (N {1u} << 4) - 1u;
        
        std::string p_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
        std::string p_dec = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
        
        std::string n_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
        std::string n_dec = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
        
        N n = N::read (n_hex);
        
        EXPECT_TRUE (p == p);
        EXPECT_FALSE (p == n);
        
        EXPECT_TRUE (n == n);
        EXPECT_FALSE (n == p);
        
        EXPECT_TRUE (p == N::read (p_hex));
        EXPECT_FALSE (p == N::read (n_hex));
        EXPECT_FALSE (n == N::read (p_hex));
        
        EXPECT_TRUE (p == N::read (p_dec));
        EXPECT_TRUE (n == N::read (n_dec));
        
        EXPECT_FALSE (p == N::read (n_dec));
        EXPECT_FALSE (n == N::read (p_dec));

        auto p_bytes = N_bytes<endian::big>::read (p_hex);
        auto n_bytes = N_bytes<endian::big>::read (n_hex);
        
        N p_from_big (p_bytes);
        N n_from_big (n_bytes);

        N_bytes<endian::little> p_little_from_N (p_from_big);
        N_bytes<endian::little> n_little_from_N (n_from_big);
        
        N p_from_little {p_little_from_N};
        N n_from_little {n_little_from_N};
        
        EXPECT_EQ (p_bytes, N_bytes<endian::big> (p_from_big));
        EXPECT_EQ (n_bytes, N_bytes<endian::big> (n_from_big));
        
        EXPECT_EQ (p_from_big, p_from_little);
        EXPECT_EQ (n_from_big, n_from_little);
        
    }
    
    TEST (N, NRemainder) {
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") % 1, N (0));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") % 10, N (5));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") % 16, N (15));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") % 58, N (29));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") / 1u,
            N::read ("26959946667150639794667015087019630673637144422540572481103610249215"));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") / 10u,
            N::read ("2695994666715063979466701508701963067363714442254057248110361024921"));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") / 16u,
            N::read ("1684996666696914987166688442938726917102321526408785780068975640575"));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") / 58u,
            N::read ("464826666675011030942534742879648804717881800388630560019027762917"));
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N::read ("26959946667150639794667015087019630673637144422540572481103610249215"), N {1});
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N::read ("2695994666715063979466701508701963067363714442254057248110361024921"), N {10});
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N::read ("1684996666696914987166688442938726917102321526408785780068975640575"), N {16});
        
        EXPECT_EQ (N::read ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N::read ("464826666675011030942534742879648804717881800388630560019027762917"), N {58});
    }
    
    TEST (N, AKS) {
        math::number::AKS<N> aks {};
        
        EXPECT_FALSE (aks.is_prime (N::read ("0")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("1")).valid ());
        EXPECT_TRUE (aks.is_prime (N::read ("2")).valid ());
        EXPECT_TRUE (aks.is_prime (N::read ("3")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("4")).valid ());
        EXPECT_TRUE (aks.is_prime (N::read ("5")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("6")).valid ());
        EXPECT_TRUE (aks.is_prime (N::read ("7")).valid ());
        
        EXPECT_TRUE (aks.is_prime (N::read ("23")).valid ());
        EXPECT_TRUE (aks.is_prime (N::read ("101")).valid ());
        
        EXPECT_FALSE (aks.is_prime (N::read ("519")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("3439")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("988320843")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("2904873984723454089")).valid ());
        EXPECT_FALSE (aks.is_prime (N::read ("4095842309824958234058934985234958304985083")).valid ());
        
        // These tests are commented out because they run too slow for practical use. 
        /*EXPECT_TRUE (aks.is_prime (N {"523"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"3449"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"988320847"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"2904873984723454103"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"4095842309824958234058934985234958304985117"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}).valid ());*/
        
    }
    
}
