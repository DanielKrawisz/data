// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include <data/math/number/bytes.hpp>
#include "gtest/gtest.h"

namespace data {
    
    TEST (NTest, TestStringToN) {
        
        EXPECT_THROW (N {""}, exception);
        EXPECT_THROW (N {"a"}, exception);
        EXPECT_THROW (N {"-"}, exception);
        EXPECT_THROW (N {"-1"}, exception);
        EXPECT_THROW (N {"01"}, exception);
        EXPECT_THROW (N {"0x1"}, exception);

        EXPECT_NO_THROW (N {0});
        EXPECT_NO_THROW (N {"0"});
        EXPECT_NO_THROW (N {"0x"});
        
        EXPECT_EQ (N {}, N {"0"});
        
        EXPECT_EQ (N {0}, N {"0"});
        EXPECT_EQ (N {0}, N {"0x"});
        
        EXPECT_NE (N {1}, N {"0"});
        EXPECT_NE (N {0}, N {"1"});
        
        EXPECT_EQ (N {1}, N {"1"});
        EXPECT_EQ (N {1}, N {"0x01"});
        EXPECT_EQ (N {1}, N {"0x0001"});
        EXPECT_EQ (N {255}, N {"0xff"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {0}), "0x");
        EXPECT_EQ (encoding::decimal::write (N {0}), "0");
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {1}), "0x01");
        EXPECT_EQ (encoding::decimal::write (N {1}), "1");
        
    }

    TEST (NTest, TestNToHexString) {
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {"0"}), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {"127"}), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N {"128"}), std::string {"0x80"});
        
    }
    
    TEST (NTest, TestNNegativeHex) {
        
        EXPECT_NE (N {"0x8000000000000000000000000000000000000000000000000000000000000000"}, N (0));
        EXPECT_NE (N {"0x80000000000000000000000000000000000000000000000000000000000000"}, N (0));
        EXPECT_NE (N {"0x80000000000000000000000000000000000000000000000000000000"}, N (0));
        EXPECT_NE (N {"0x8000000000000000000000000000000000000000000000"}, N (0));
        EXPECT_NE (N {"0x8000000000000000000000000000"}, N (0));
        EXPECT_NE (N {"0x8000000000"}, N (0));
        EXPECT_NE (N {"0x8000"}, N (0));
        EXPECT_NE (N {"0x80"}, N (0));
        
    }
    
    TEST (NTest, TestMultiply) {
        
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
    
    TEST (NTest, TestShift) {
        
        EXPECT_TRUE (N {1} >>  1 == N {"0x00000000000000000000"});
        EXPECT_TRUE (N {1} <<  1 == N {"0x00000000000000000002"});
        EXPECT_TRUE (N {1} <<  3 == N {"0x00000000000000000008"});
        EXPECT_TRUE (N {1} <<  7 == N {"0x00000000000000000080"});
        EXPECT_TRUE (N {1} << 79 == N {"0x80000000000000000000"});
        
    }
        
    template<endian::order r> using N_bytes = math::number::N_bytes<r>;
    
    TEST (NTest, TestN) {
        
        // parameters from https://en.bitcoin.it/wiki/Secp256k1
        
        N p = (N {1} << 256) - (N {1} << 32) - (N {1} << 9) - (N {1} << 8) - (N {1} << 7) - (N {1} << 6) - (N {1} << 4) - 1;
        
        std::string p_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
        std::string p_dec = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
        
        std::string n_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
        std::string n_dec = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
        
        N n = N {n_hex};
        
        EXPECT_TRUE (p == p);
        EXPECT_FALSE (p == n);
        
        EXPECT_TRUE (n == n);
        EXPECT_FALSE (n == p);
        
        EXPECT_TRUE (p == N {p_hex});
        EXPECT_FALSE (p == N {n_hex});
        EXPECT_FALSE (n == N {p_hex});
        
        EXPECT_TRUE (p == N {p_dec});
        EXPECT_TRUE (n == N {n_dec});
        
        EXPECT_FALSE (p == N {n_dec});
        EXPECT_FALSE (n == N {p_dec});

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
    
    TEST (NTest, TestNRemainder) {
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") % 1, N (0));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") % 10, N (5));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") % 16, N (15));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") % 58, N (29));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") / 1,
            N ("26959946667150639794667015087019630673637144422540572481103610249215"));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") / 10,
            N ("2695994666715063979466701508701963067363714442254057248110361024921"));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") / 16,
            N ("1684996666696914987166688442938726917102321526408785780068975640575"));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") / 58,
            N ("464826666675011030942534742879648804717881800388630560019027762917"));
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N ("26959946667150639794667015087019630673637144422540572481103610249215"), N {1});
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N ("2695994666715063979466701508701963067363714442254057248110361024921"), N {10});
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N ("1684996666696914987166688442938726917102321526408785780068975640575"), N {16});
        
        EXPECT_EQ (N ("26959946667150639794667015087019630673637144422540572481103610249215") /
            N ("464826666675011030942534742879648804717881800388630560019027762917"), N {58});
    }
    
    TEST (NTest, TestAKS) {
        math::number::AKS<N> aks {};
        
        EXPECT_FALSE (aks.is_prime (N {"0"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"1"}).valid ());
        EXPECT_TRUE (aks.is_prime (N {"2"}).valid ());
        EXPECT_TRUE (aks.is_prime (N {"3"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"4"}).valid ());
        EXPECT_TRUE (aks.is_prime (N {"5"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"6"}).valid ());
        EXPECT_TRUE (aks.is_prime (N {"7"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"23"}).valid ());
        EXPECT_TRUE (aks.is_prime (N {"101"}).valid ());
        
        EXPECT_FALSE (aks.is_prime (N {"519"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"3439"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"988320843"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"2904873984723454089"}).valid ());
        EXPECT_FALSE (aks.is_prime (N {"4095842309824958234058934985234958304985083"}).valid ());
        
        // These tests are commented out because they run too slow for practical use. 
        /*EXPECT_TRUE (aks.is_prime (N {"523"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"3449"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"988320847"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"2904873984723454103"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"4095842309824958234058934985234958304985117"}).valid ());
        
        EXPECT_TRUE (aks.is_prime (N {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}).valid ());*/
        
    }
    
}
