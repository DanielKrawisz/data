// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    void test_dec_to_hex (const string &x) {

        N nx {x};

        EXPECT_EQ (x, encoding::decimal::write (nx));

        N_bytes_little nlx = *encoding::decimal::read<endian::little> (x);
        N_bytes_big nbx = *encoding::decimal::read<endian::big> (x);

        N_bytes_little nxl (nx);
        N_bytes_big nxb (nx);
        EXPECT_EQ (nlx, nxl) << std::hex << "expected " << nlx << " to equal " << nxl;
        EXPECT_EQ (nbx, nxb) << std::hex << "expected " << nbx << " to equal " << nxb;

        EXPECT_EQ (nx, N (nlx));
        EXPECT_EQ (nx, N (nbx));

        string nlxx = encoding::decimal::write (nlx);
        string nbxx = encoding::decimal::write (nbx);
        EXPECT_EQ (x, nlxx) << std::hex << "expected " << x << " to equal " << nbxx;
        EXPECT_EQ (x, nbxx) << std::hex << "expected " << x << " to equal " << nbxx;

        EXPECT_EQ (nlx, N_bytes_little (nbx));
        EXPECT_EQ (nbx, N_bytes_big (nlx));

        auto nxh = encoding::hexidecimal::write<hex_case::lower> (nx);

        EXPECT_EQ (nxh, encoding::hexidecimal::write<hex_case::lower> (nlx));
        EXPECT_EQ (nxh, encoding::hexidecimal::write<hex_case::lower> (nbx));

        EXPECT_EQ (nx, N (nxh));
        EXPECT_EQ (nlx, N_bytes_little (nxh));
        EXPECT_EQ (nbx, N_bytes_big (nxh));

    }

    TEST (DecimalTest, TestDecToHex) {

        test_dec_to_hex ("129");
        test_dec_to_hex ("7493");
        test_dec_to_hex ("749384");
        test_dec_to_hex ("483749384");
        test_dec_to_hex ("7206483749384");
        test_dec_to_hex ("24397842987206483749384");
        test_dec_to_hex ("98980987676898761029390303474536547398");
        test_dec_to_hex ("98980987676898761029390303474536547399");
        test_dec_to_hex ("98980987676898761029390303474536547400");
    }
    
    void test_decrement_signed (const string &given, const string &expected) {
        
        dec_int g (given);
        dec_int e (expected);
        
        EXPECT_EQ (decrement (g), e);
        EXPECT_EQ (decrement (Z::read (g)), Z::read (e));
        EXPECT_EQ (decrement (Z_bytes_little::read (g)), Z_bytes_little::read (e));
        EXPECT_EQ (decrement (Z_bytes_big::read (g)), Z_bytes_big::read (e));

    }
    
    void test_decrement_unsigned (const string &given, const string &expected) {

        dec_uint g (given);
        dec_uint e (expected);
        EXPECT_EQ (decrement (g), e);

        auto Ng = N {g};
        auto Ne = N {e};
        auto Nd = decrement (Ng);
        EXPECT_EQ (Nd, Ne) << "expected " << Ng << " to decrement to " << Ne << " but got " << Nd;

        auto Nblg = N_bytes_little::read (g);
        auto Nble = N_bytes_little::read (e);
        auto Nbld = decrement (Nblg);
        EXPECT_EQ (Nbld, Nble) << "expected " << std::hex << Nblg << " to decrement to " << Nble << " but got " << Nbld;

        auto Nbbg = N_bytes_big::read (g);
        auto Nbbe = N_bytes_big::read (e);
        auto Nbbd = decrement (Nbbg);
        EXPECT_EQ (Nbbd, Nbbe) << "expected " << std::hex << Nbbg << " to decrement to " << Nbbe << " but got " << Nbbd;

        auto gg = base58_uint::read (g);
        auto ee = base58_uint::read (e);
        auto ggd = decrement (gg);

        EXPECT_EQ (ggd, ee);
        EXPECT_EQ (decrement (hex_uint::read (g)), hex_uint::read (e));
        
    }
    
    void test_increment_signed_final (const dec_int &g, const dec_int &e) {
        
        EXPECT_EQ (increment (g), e);
        EXPECT_EQ (increment (Z::read (g)), Z::read (e));
        EXPECT_EQ (increment (Z_bytes_little::read (g)), Z_bytes_little::read (e));
        EXPECT_EQ (increment (Z_bytes_big::read (g)), Z_bytes_big::read (e));
        
    }
    
    void test_increment_signed (const string &given, const string &expected) {
        
        dec_int g (given);
        dec_int e (expected);
        
        test_increment_signed_final (g, e);
        test_decrement_signed (e, g);
        
        test_increment_signed_final (-e, -g);
        test_decrement_signed (-g, -e);
    }
    
    void test_increment_unsigned (const string &given, const string &expected) {

        dec_uint g (given);
        dec_uint e (expected);
        
        EXPECT_EQ (increment (g), e);
        EXPECT_EQ (increment (N (g)), N (e));
        EXPECT_EQ (increment (N_bytes_little::read (g)), N_bytes_little::read (e));
        EXPECT_EQ (increment (N_bytes_big::read (g)), N_bytes_big::read (e));
        
        auto b58g = base58_uint::read (g);
        auto b58e = base58_uint::read (e);
        auto b58i = increment (b58g);

        EXPECT_EQ (b58i, b58e);
        EXPECT_EQ (increment (hex_uint::read (g)), hex_uint::read (e));
        
        test_decrement_unsigned (expected, given);
        test_increment_signed (given, expected);
    }

    TEST (DecimalTest, TestDecimalIncrement) {

        test_decrement_unsigned ("0", "0");
        
        test_increment_unsigned ("0", "1");
        test_increment_unsigned ("1", "2");
        test_increment_unsigned ("9", "10");
        test_increment_unsigned ("10", "11");
        test_increment_unsigned ("99999999999", "100000000000");
        test_increment_unsigned ("102939030347", "102939030348");
        test_increment_unsigned ("98980987676898761029390303474536547398", "98980987676898761029390303474536547399");
        test_increment_unsigned ("98980987676898761029390303474536547399", "98980987676898761029390303474536547400");
        
    }

    TEST (DecimalTest, TestDecimalUInt) {

        EXPECT_EQ (dec_int {0}, dec_int {"0"});
        EXPECT_EQ (dec_uint {"4"} & dec_uint {"1"}, dec_uint {"0"});

    }

    void test_add_signed (const string &left, const string &right, const string &expected) {

        dec_int l (left);
        dec_int r (right);
        dec_int e (expected);
        
        EXPECT_EQ (l + r, e);

        EXPECT_EQ (Z::read (l) + Z::read (r), Z::read (e));

        EXPECT_EQ (Z_bytes_little::read (l) + Z_bytes_little::read (r), Z_bytes_little::read (e));
        EXPECT_EQ (Z_bytes_big::read (l) + Z_bytes_big::read (r), Z_bytes_big::read (e));

        EXPECT_EQ (Z_bytes_twos_little::read (l) + Z_bytes_twos_little::read (r), Z_bytes_twos_little::read (e));
        EXPECT_EQ (Z_bytes_twos_big::read (l) + Z_bytes_twos_big::read (r), Z_bytes_twos_big::read (e));
        
    }
    
    void test_add_unsigned (const string &left, const string &right, const string &expected) {
        
        dec_uint l (left);
        dec_uint r (right);
        dec_uint e (expected);
        
        EXPECT_EQ (l + r, e);
        EXPECT_EQ (N (l) + N (r), N (e));

        EXPECT_EQ (N_bytes_little::read (l) + N_bytes_little::read (r), N_bytes_little::read (e));
        EXPECT_EQ (N_bytes_big::read (l) + N_bytes_big::read (r), N_bytes_big::read (e));
        
        test_add_signed (left, right, expected);
    }
    
    TEST (DecimalTest, TestDecimalAdd) {

        test_add_unsigned ("0", "0", "0");
        test_add_unsigned ("0", "1", "1");
        test_add_unsigned ("1", "0", "1");
        test_add_unsigned ("1", "1", "2");
        test_add_unsigned ("231938875480", "397027301409876", "397259240285356");
        test_add_unsigned ("23173210900987658780938875480", "39702733535456767789001409876", "62875944436444426569940285356");

        test_add_signed ("-1", "0", "-1");
        test_add_signed ("0", "-1", "-1");
        test_add_signed ("-1", "-1", "-2");
        test_add_signed ("-1", "2", "1");
        test_add_signed ("2", "-1", "1");
        test_add_signed ("1", "-2", "-1");
        test_add_signed ("-2", "1", "-1");
        test_add_signed ("-23173210900987658780938875480", "23173210900987658780938875480", "0");
        test_add_signed ("23173210900987658780938875480", "-23173210900987658780938875480", "0");
        test_add_signed ("-23173210900987658780938875481", "23173210900987658780938875480", "-1");
        test_add_signed ("23173210900987658780938875480", "-23173210900987658780938875481", "-1");
        test_add_signed ("23173210900987658780938875481", "-23173210900987658780938875480", "1");
        test_add_signed ("-23173210900987658780938875480", "23173210900987658780938875481", "1");
    }

    void test_subtract_unsigned (const string &left, const string &right, const string &expected) {
        
        dec_uint l (left);
        dec_uint r (right);
        dec_uint e (expected);
        
        EXPECT_EQ (l - r, e);
        EXPECT_EQ (N (l) - N (r), N (e));

        EXPECT_EQ (N_bytes_little::read (l) - N_bytes_little::read (r), N_bytes_little::read (e));
        EXPECT_EQ (N_bytes_big::read (l) - N_bytes_big::read (r), N_bytes_big::read (e));
    }
    
    void test_subtract_signed (const string &left, const string &right, const string &expected) {

        dec_int l (left);
        dec_int r (right);
        dec_int e (expected);
        
        EXPECT_EQ (l - r, e);
        EXPECT_EQ (Z::read (l) - Z::read (r), Z::read (e));

        EXPECT_EQ (Z_bytes_little::read (l) - Z_bytes_little::read (r), Z_bytes_little::read (e));
        EXPECT_EQ (Z_bytes_big::read (l) - Z_bytes_big::read (r), Z_bytes_big::read (e));

        EXPECT_EQ (Z_bytes_twos_little::read (l) - Z_bytes_twos_little::read (r), Z_bytes_twos_little::read (e));
        EXPECT_EQ (Z_bytes_twos_big::read (l) - Z_bytes_twos_big::read (r), Z_bytes_twos_big::read (e));

    }
    
    TEST (DecimalTest, TestDecimalSubtract) {

        test_subtract_unsigned ("0", "0", "0");
        test_subtract_signed ("0", "0", "0");
        
        test_subtract_unsigned ("0", "0", "0");
        test_subtract_unsigned ("1", "0", "1");
        test_subtract_unsigned ("1", "1", "0");
        
        test_subtract_unsigned ("37", "12", "25");
        test_subtract_unsigned ("4502938948920982780930898389", "4502938948920982780930898389", "0");
        test_subtract_unsigned ("4502938948920982780930898389", "4502938948920982780930898390", "0");

        test_subtract_signed ("0", "0", "0");
        test_subtract_signed ("1", "0", "1");
        test_subtract_signed ("1", "1", "0");
        
        test_subtract_signed ("37", "12", "25");
        test_subtract_signed ("4502938948920982780930898389", "4502938948920982780930898389", "0");
        
        test_subtract_signed ("4502938948920982780930898389", "4502938948920982780930898390", "-1");
        
    }
    
    void test_multiply_signed (const string &left, const string &right, const string &expected) {

        dec_int l (left);
        dec_int r (right);
        dec_int e (expected);
        
        EXPECT_EQ (l * r, e);
        EXPECT_EQ (Z::read (l) * Z::read (r), Z::read (e));

        EXPECT_EQ (Z_bytes_little::read (l) * Z_bytes_little::read (r), Z_bytes_little::read (e));
        EXPECT_EQ (Z_bytes_big::read (l) * Z_bytes_big::read (r), Z_bytes_big::read (e));

        EXPECT_EQ (Z_bytes_twos_little::read (l) * Z_bytes_twos_little::read (r), Z_bytes_twos_little::read (e));
        EXPECT_EQ (Z_bytes_twos_big::read (l) * Z_bytes_twos_big::read (r), Z_bytes_twos_big::read (e));

    }
    
    void test_multiply_unsigned (const string &left, const string &right, const string &expected) {
        
        dec_uint l (left);
        dec_uint r (right);
        dec_uint e (expected);
        EXPECT_EQ (l * r, e);
        EXPECT_EQ (N (l) * N (r), N (e));

        EXPECT_EQ (N_bytes_little::read (l) * N_bytes_little::read (r), N_bytes_little::read (e));
        EXPECT_EQ (N_bytes_big::read (l) * N_bytes_big::read (r), N_bytes_big::read (e));
        
        test_multiply_signed (left, right, expected);
    }
    
    TEST (DecimalTest, TestDecimalMultiply) {

        test_multiply_unsigned ("0", "0", "0");
        test_multiply_unsigned ("0", "1", "0");
        test_multiply_unsigned ("1", "1", "1");
        test_multiply_unsigned ("1", "2", "2");
        test_multiply_unsigned ("23", "45", "1035");
        test_multiply_unsigned ("4802", "6107", "29325814");
        test_multiply_unsigned ("48028", "96107", "4615826996");
        test_multiply_unsigned ("248028", "961073", "238373014044");
        test_multiply_unsigned ("4334098237", "357843409854", "1550928491770289827398");
        test_multiply_unsigned (
            "23173210900987658780938875480",
            "39702733535456767789001409876",
            "920039817562855061210426612476533348173557348698006240480");
        
    }
    
}
