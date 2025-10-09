// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    void test_dec_to_hex (const string &x) {

        N nx {x};

        EXPECT_EQ (x, encoding::decimal::write (nx));

        N_bytes_little nlx = *encoding::decimal::read<endian::little, byte> (x);
        N_bytes_big nbx = *encoding::decimal::read<endian::big, byte> (x);

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

    TEST (Decimal, DecToHex) {

        test_dec_to_hex ("0");
        test_dec_to_hex ("1");
        test_dec_to_hex ("9");
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

    template <std::unsigned_integral word> using Nl = math::number::N_bytes<endian::little, word>;
    template <std::unsigned_integral word> using Zl1 = math::number::Z_bytes<endian::little, math::negativity::twos, word>;
    template <std::unsigned_integral word> using Zl2 = math::number::Z_bytes<endian::little, math::negativity::BC, word>;
    template <std::unsigned_integral word> using Nb = math::number::N_bytes<endian::big, word>;
    template <std::unsigned_integral word> using Zb1 = math::number::Z_bytes<endian::big, math::negativity::twos, word>;
    template <std::unsigned_integral word> using Zb2 = math::number::Z_bytes<endian::big, math::negativity::BC, word>;

    void test_decrement_signed (const string &given, const string &expected) {
        
        dec_int g (given);
        dec_int e (expected);

        EXPECT_EQ (decrement (g), e);
        EXPECT_EQ (decrement (Z::read (g)), Z::read (e));

        EXPECT_EQ (decrement (Zl1<byte>::read (g)), Zl1<byte>::read (e));
        EXPECT_EQ (decrement (Zb1<byte>::read (g)), Zb1<byte>::read (e));

        EXPECT_EQ (decrement (Zl1<unsigned short>::read (g)), Zl1<unsigned short>::read (e));
        EXPECT_EQ (decrement (Zb1<unsigned short>::read (g)), Zb1<unsigned short>::read (e));

        EXPECT_EQ (decrement (Zl1<unsigned>::read (g)), Zl1<unsigned>::read (e));
        EXPECT_EQ (decrement (Zb1<unsigned>::read (g)), Zb1<unsigned>::read (e));

        EXPECT_EQ (decrement (Zl1<unsigned long>::read (g)), Zl1<unsigned long>::read (e));
        EXPECT_EQ (decrement (Zb1<unsigned long>::read (g)), Zb1<unsigned long>::read (e));

        EXPECT_EQ (decrement (Zl1<unsigned long long>::read (g)), Zl1<unsigned long long>::read (e));
        EXPECT_EQ (decrement (Zb1<unsigned long long>::read (g)), Zb1<unsigned long long>::read (e));

        EXPECT_EQ (decrement (Zl2<byte>::read (g)), Zl2<byte>::read (e));
        EXPECT_EQ (decrement (Zb2<byte>::read (g)), Zb2<byte>::read (e));

        EXPECT_EQ (decrement (Zl2<unsigned short>::read (g)), Zl2<unsigned short>::read (e));
        EXPECT_EQ (decrement (Zb2<unsigned short>::read (g)), Zb2<unsigned short>::read (e));

        EXPECT_EQ (decrement (Zl2<unsigned>::read (g)), Zl2<unsigned>::read (e));
        EXPECT_EQ (decrement (Zb2<unsigned>::read (g)), Zb2<unsigned>::read (e));

        EXPECT_EQ (decrement (Zl2<unsigned long>::read (g)), Zl2<unsigned long>::read (e));
        EXPECT_EQ (decrement (Zb2<unsigned long>::read (g)), Zb2<unsigned long>::read (e));

        EXPECT_EQ (decrement (Zl2<unsigned long long>::read (g)), Zl2<unsigned long long>::read (e));
        EXPECT_EQ (decrement (Zb2<unsigned long long>::read (g)), Zb2<unsigned long long>::read (e));

        // TODO test bounded numbers here.

    }

    template <std::unsigned_integral word>
    void test_decrement_N_b (const dec_uint &g, const dec_uint &e) {

        Nl<word> Nblg = Nl<word>::read (g);
        Nl<word> Nble = Nl<word>::read (e);
        Nl<word> Nbld = decrement (Nblg);
        EXPECT_EQ (Nbld, Nble) << "expected " << std::hex << Nblg << " to decrement to " << Nble << " but got " << Nbld;

        Nb<word> Nbbg = Nb<word>::read (g);
        Nb<word> Nbbe = Nb<word>::read (e);
        Nb<word> Nbbd = decrement (Nbbg);
        EXPECT_EQ (Nbbd, Nbbe) << "expected " << std::hex << Nbbg << " to decrement to " << Nbbe << " but got " << Nbbd;

    }
    
    void test_decrement_unsigned (const string &given, const string &expected) {

        dec_uint g (given);
        dec_uint e (expected);
        EXPECT_EQ (decrement (g), e);

        auto Ng = N {g};
        auto Ne = N {e};
        auto Nd = decrement (Ng);
        EXPECT_EQ (Nd, Ne) << "expected " << Ng << " to decrement to " << Ne << " but got " << Nd;

        auto gg = base58_uint::read (g);
        auto ee = base58_uint::read (e);
        auto ggd = decrement (gg);

        EXPECT_EQ (ggd, ee);
        EXPECT_EQ (decrement (hex_uint::read (g)), hex_uint::read (e));

        test_decrement_N_b<byte> (g, e);
        test_decrement_N_b<unsigned short> (g, e);
        test_decrement_N_b<unsigned> (g, e);
        test_decrement_N_b<unsigned long> (g, e);
        test_decrement_N_b<unsigned long long> (g, e);
        
    }
    
    void test_increment_signed_final (const dec_int &g, const dec_int &e) {

        EXPECT_EQ (increment (g), e);
        EXPECT_EQ (increment (Z::read (g)), Z::read (e));

        EXPECT_EQ (increment (Zl1<byte>::read (g)), Zl1<byte>::read (e));
        EXPECT_EQ (increment (Zb1<byte>::read (g)), Zb1<byte>::read (e));

        EXPECT_EQ (increment (Zl2<byte>::read (g)), Zl2<byte>::read (e));
        EXPECT_EQ (increment (Zb2<byte>::read (g)), Zb2<byte>::read (e));

        EXPECT_EQ (increment (Zl1<unsigned short>::read (g)), Zl1<unsigned short>::read (e));
        EXPECT_EQ (increment (Zb1<unsigned short>::read (g)), Zb1<unsigned short>::read (e));

        EXPECT_EQ (increment (Zl2<unsigned short>::read (g)), Zl2<unsigned short>::read (e));
        EXPECT_EQ (increment (Zb2<unsigned short>::read (g)), Zb2<unsigned short>::read (e));

        EXPECT_EQ (increment (Zl1<unsigned>::read (g)), Zl1<unsigned>::read (e));
        EXPECT_EQ (increment (Zb1<unsigned>::read (g)), Zb1<unsigned>::read (e));

        EXPECT_EQ (increment (Zl2<unsigned>::read (g)), Zl2<unsigned>::read (e));
        EXPECT_EQ (increment (Zb2<unsigned>::read (g)), Zb2<unsigned>::read (e));

        EXPECT_EQ (increment (Zl1<unsigned long>::read (g)), Zl1<unsigned long>::read (e));
        EXPECT_EQ (increment (Zb1<unsigned long>::read (g)), Zb1<unsigned long>::read (e));

        EXPECT_EQ (increment (Zl2<unsigned long>::read (g)), Zl2<unsigned long>::read (e));
        EXPECT_EQ (increment (Zb2<unsigned long>::read (g)), Zb2<unsigned long>::read (e));

        EXPECT_EQ (increment (Zl1<unsigned long long>::read (g)), Zl1<unsigned long long>::read (e));
        EXPECT_EQ (increment (Zb1<unsigned long long>::read (g)), Zb1<unsigned long long>::read (e));

        EXPECT_EQ (increment (Zl2<unsigned long long>::read (g)), Zl2<unsigned long long>::read (e));
        EXPECT_EQ (increment (Zb2<unsigned long long>::read (g)), Zb2<unsigned long long>::read (e));

        // TODO test bounded numbers here.
        
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

        test_decrement_unsigned (expected, given);
        test_increment_signed (given, expected);

        dec_uint g (given);
        dec_uint e (expected);
        
        EXPECT_EQ (increment (g), e);
        EXPECT_EQ (increment (N (g)), N (e));
        
        auto b58g = base58_uint::read (g);
        auto b58e = base58_uint::read (e);
        auto b58i = increment (b58g);

        EXPECT_EQ (b58i, b58e);
        EXPECT_EQ (increment (hex_uint::read (g)), hex_uint::read (e));

        EXPECT_EQ (increment (Nl<byte>::read (g)), Nl<byte>::read (e));
        EXPECT_EQ (increment (Nb<byte>::read (g)), Nb<byte>::read (e));

        EXPECT_EQ (increment (Nl<unsigned short>::read (g)), Nl<unsigned short>::read (e));
        EXPECT_EQ (increment (Nb<unsigned short>::read (g)), Nb<unsigned short>::read (e));

        EXPECT_EQ (increment (Nl<unsigned>::read (g)), Nl<unsigned>::read (e));
        EXPECT_EQ (increment (Nb<unsigned>::read (g)), Nb<unsigned>::read (e));

        EXPECT_EQ (increment (Nl<unsigned long>::read (g)), Nl<unsigned long>::read (e));
        EXPECT_EQ (increment (Nb<unsigned long>::read (g)), Nb<unsigned long>::read (e));

        EXPECT_EQ (increment (Nl<unsigned long long>::read (g)), Nl<unsigned long long>::read (e));
        EXPECT_EQ (increment (Nb<unsigned long long>::read (g)), Nb<unsigned long long>::read (e));

        // TODO bounded numbers here.
    }

    TEST (Decimal, DecimalIncrement) {

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

    TEST (Decimal, DecimalUInt) {

        EXPECT_EQ (dec_int {0}, dec_int {"0"});
        EXPECT_EQ (dec_uint {"4"} & dec_uint {"1"}, dec_uint {"0"});

    }

    void test_add_signed (const string &left, const string &right, const string &expected) {

        dec_int l (left);
        dec_int r (right);
        dec_int e (expected);
        
        EXPECT_EQ (l + r, e);

        EXPECT_EQ (Z::read (l) + Z::read (r), Z::read (e));

        EXPECT_EQ (Zl1<byte>::read (l) + Zl1<byte>::read (r), Zl1<byte>::read (e));
        EXPECT_EQ (Zb1<byte>::read (l) + Zb1<byte>::read (r), Zb1<byte>::read (e));

        EXPECT_EQ (Zl2<byte>::read (l) + Zl2<byte>::read (r), Zl2<byte>::read (e));
        EXPECT_EQ (Zb2<byte>::read (l) + Zb2<byte>::read (r), Zb2<byte>::read (e));

        EXPECT_EQ (Zl1<unsigned short>::read (l) + Zl1<unsigned short>::read (r), Zl1<unsigned short>::read (e));
        EXPECT_EQ (Zb1<unsigned short>::read (l) + Zb1<unsigned short>::read (r), Zb1<unsigned short>::read (e));

        EXPECT_EQ (Zl2<unsigned short>::read (l) + Zl2<unsigned short>::read (r), Zl2<unsigned short>::read (e));
        EXPECT_EQ (Zb2<unsigned short>::read (l) + Zb2<unsigned short>::read (r), Zb2<unsigned short>::read (e));

        EXPECT_EQ (Zl1<unsigned>::read (l) + Zl1<unsigned>::read (r), Zl1<unsigned>::read (e));
        EXPECT_EQ (Zb1<unsigned>::read (l) + Zb1<unsigned>::read (r), Zb1<unsigned>::read (e));

        EXPECT_EQ (Zl2<unsigned>::read (l) + Zl2<unsigned>::read (r), Zl2<unsigned>::read (e));
        EXPECT_EQ (Zb2<unsigned>::read (l) + Zb2<unsigned>::read (r), Zb2<unsigned>::read (e));

        EXPECT_EQ (Zl1<unsigned long>::read (l) + Zl1<unsigned long>::read (r), Zl1<unsigned long>::read (e));
        EXPECT_EQ (Zb1<unsigned long>::read (l) + Zb1<unsigned long>::read (r), Zb1<unsigned long>::read (e));

        EXPECT_EQ (Zl2<unsigned long>::read (l) + Zl2<unsigned long>::read (r), Zl2<unsigned long>::read (e));
        EXPECT_EQ (Zb2<unsigned long>::read (l) + Zb2<unsigned long>::read (r), Zb2<unsigned long>::read (e));

        EXPECT_EQ (Zl1<unsigned long long>::read (l) + Zl1<unsigned long long>::read (r), Zl1<unsigned long long>::read (e));
        EXPECT_EQ (Zb1<unsigned long long>::read (l) + Zb1<unsigned long long>::read (r), Zb1<unsigned long long>::read (e));

        EXPECT_EQ (Zl2<unsigned long long>::read (l) + Zl2<unsigned long long>::read (r), Zl2<unsigned long long>::read (e));
        EXPECT_EQ (Zb2<unsigned long long>::read (l) + Zb2<unsigned long long>::read (r), Zb2<unsigned long long>::read (e));

        // TODO test bounded numbers
        
    }
    
    void test_add_unsigned (const string &left, const string &right, const string &expected) {
        
        dec_uint l (left);
        dec_uint r (right);
        dec_uint e (expected);
        
        EXPECT_EQ (l + r, e);
        EXPECT_EQ (N (l) + N (r), N (e));

        // TODO
        EXPECT_EQ (Nl<byte>::read (l) + Nl<byte>::read (r), Nl<byte>::read (e));
        EXPECT_EQ (Nb<byte>::read (l) + Nb<byte>::read (r), Nb<byte>::read (e));

        EXPECT_EQ (Nl<unsigned short>::read (l) + Nl<unsigned short>::read (r), Nl<unsigned short>::read (e));
        EXPECT_EQ (Nb<unsigned short>::read (l) + Nb<unsigned short>::read (r), Nb<unsigned short>::read (e));

        EXPECT_EQ (Nl<unsigned>::read (l) + Nl<unsigned>::read (r), Nl<unsigned>::read (e));
        EXPECT_EQ (Nb<unsigned>::read (l) + Nb<unsigned>::read (r), Nb<unsigned>::read (e));

        EXPECT_EQ (Nl<unsigned long>::read (l) + Nl<unsigned long>::read (r), Nl<unsigned long>::read (e));
        EXPECT_EQ (Nb<unsigned long>::read (l) + Nb<unsigned long>::read (r), Nb<unsigned long>::read (e));

        EXPECT_EQ (Nl<unsigned long long>::read (l) + Nl<unsigned long long>::read (r), Nl<unsigned long long>::read (e));
        EXPECT_EQ (Nb<unsigned long long>::read (l) + Nb<unsigned long long>::read (r), Nb<unsigned long long>::read (e));

        test_add_signed (left, right, expected);

        // TODO test bounded numbers
    }
    
    TEST (Decimal, DecimalAdd) {

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

        // TODO
        EXPECT_EQ (Nb<byte>::read (l) - Nb<byte>::read (r), Nb<byte>::read (e));
        EXPECT_EQ (Nb<byte>::read (l) - Nb<byte>::read (r), Nb<byte>::read (e));

        EXPECT_EQ (Nb<unsigned short>::read (l) - Nb<unsigned short>::read (r), Nb<unsigned short>::read (e));
        EXPECT_EQ (Nb<unsigned short>::read (l) - Nb<unsigned short>::read (r), Nb<unsigned short>::read (e));

        EXPECT_EQ (Nb<unsigned>::read (l) - Nb<unsigned>::read (r), Nb<unsigned>::read (e));
        EXPECT_EQ (Nb<unsigned>::read (l) - Nb<unsigned>::read (r), Nb<unsigned>::read (e));

        EXPECT_EQ (Nb<unsigned long>::read (l) - Nb<unsigned long>::read (r), Nb<unsigned long>::read (e));
        EXPECT_EQ (Nb<unsigned long>::read (l) - Nb<unsigned long>::read (r), Nb<unsigned long>::read (e));

        EXPECT_EQ (Nb<unsigned long long>::read (l) - Nb<unsigned long long>::read (r), Nb<unsigned long long>::read (e));
        EXPECT_EQ (Nb<unsigned long long>::read (l) - Nb<unsigned long long>::read (r), Nb<unsigned long long>::read (e));

        // TODO test bounded numbers
    }
    
    void test_subtract_signed (const string &left, const string &right, const string &expected) {

        dec_int l (left);
        dec_int r (right);
        dec_int e (expected);
        
        EXPECT_EQ (l - r, e);
        EXPECT_EQ (Z::read (l) - Z::read (r), Z::read (e));

        EXPECT_EQ (Zl1<byte>::read (l) - Zl1<byte>::read (r), Zl1<byte>::read (e));
        EXPECT_EQ (Zb1<byte>::read (l) - Zb1<byte>::read (r), Zb1<byte>::read (e));

        EXPECT_EQ (Zl2<byte>::read (l) - Zl2<byte>::read (r), Zl2<byte>::read (e));
        EXPECT_EQ (Zb2<byte>::read (l) - Zb2<byte>::read (r), Zb2<byte>::read (e));

        EXPECT_EQ (Zl1<unsigned short>::read (l) - Zl1<unsigned short>::read (r), Zl1<unsigned short>::read (e));
        EXPECT_EQ (Zb1<unsigned short>::read (l) - Zb1<unsigned short>::read (r), Zb1<unsigned short>::read (e));

        EXPECT_EQ (Zl2<unsigned short>::read (l) - Zl2<unsigned short>::read (r), Zl2<unsigned short>::read (e));
        EXPECT_EQ (Zb2<unsigned short>::read (l) - Zb2<unsigned short>::read (r), Zb2<unsigned short>::read (e));

        EXPECT_EQ (Zl1<unsigned>::read (l) - Zl1<unsigned>::read (r), Zl1<unsigned>::read (e));
        EXPECT_EQ (Zb1<unsigned>::read (l) - Zb1<unsigned>::read (r), Zb1<unsigned>::read (e));

        EXPECT_EQ (Zl2<unsigned>::read (l) - Zl2<unsigned>::read (r), Zl2<unsigned>::read (e));
        EXPECT_EQ (Zb2<unsigned>::read (l) - Zb2<unsigned>::read (r), Zb2<unsigned>::read (e));

        EXPECT_EQ (Zl1<unsigned long>::read (l) - Zl1<unsigned long>::read (r), Zl1<unsigned long>::read (e));
        EXPECT_EQ (Zb1<unsigned long>::read (l) - Zb1<unsigned long>::read (r), Zb1<unsigned long>::read (e));

        EXPECT_EQ (Zl2<unsigned long>::read (l) - Zl2<unsigned long>::read (r), Zl2<unsigned long>::read (e));
        EXPECT_EQ (Zb2<unsigned long>::read (l) - Zb2<unsigned long>::read (r), Zb2<unsigned long>::read (e));

        EXPECT_EQ (Zl1<unsigned long long>::read (l) - Zl1<unsigned long long>::read (r), Zl1<unsigned long long>::read (e));
        EXPECT_EQ (Zb1<unsigned long long>::read (l) - Zb1<unsigned long long>::read (r), Zb1<unsigned long long>::read (e));

        EXPECT_EQ (Zl2<unsigned long long>::read (l) - Zl2<unsigned long long>::read (r), Zl2<unsigned long long>::read (e));
        EXPECT_EQ (Zb2<unsigned long long>::read (l) - Zb2<unsigned long long>::read (r), Zb2<unsigned long long>::read (e));

        // TODO test bounded numbers.

    }
    
    TEST (Decimal, DecimalSubtract) {

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

        EXPECT_EQ (Zl1<byte>::read (l) * Zl1<byte>::read (r), Zl1<byte>::read (e));
        EXPECT_EQ (Zb1<byte>::read (l) * Zb1<byte>::read (r), Zb1<byte>::read (e));

        EXPECT_EQ (Zl2<byte>::read (l) * Zl2<byte>::read (r), Zl2<byte>::read (e));
        EXPECT_EQ (Zb2<byte>::read (l) * Zb2<byte>::read (r), Zb2<byte>::read (e));

        EXPECT_EQ (Zl1<unsigned short>::read (l) * Zl1<unsigned short>::read (r), Zl1<unsigned short>::read (e));
        EXPECT_EQ (Zb1<unsigned short>::read (l) * Zb1<unsigned short>::read (r), Zb1<unsigned short>::read (e));

        EXPECT_EQ (Zl2<unsigned short>::read (l) * Zl2<unsigned short>::read (r), Zl2<unsigned short>::read (e));
        EXPECT_EQ (Zb2<unsigned short>::read (l) * Zb2<unsigned short>::read (r), Zb2<unsigned short>::read (e));

        EXPECT_EQ (Zl1<unsigned>::read (l) * Zl1<unsigned>::read (r), Zl1<unsigned>::read (e));
        EXPECT_EQ (Zb1<unsigned>::read (l) * Zb1<unsigned>::read (r), Zb1<unsigned>::read (e));

        EXPECT_EQ (Zl2<unsigned>::read (l) * Zl2<unsigned>::read (r), Zl2<unsigned>::read (e));
        EXPECT_EQ (Zb2<unsigned>::read (l) * Zb2<unsigned>::read (r), Zb2<unsigned>::read (e));

        EXPECT_EQ (Zl1<unsigned long>::read (l) * Zl1<unsigned long>::read (r), Zl1<unsigned long>::read (e));
        EXPECT_EQ (Zb1<unsigned long>::read (l) * Zb1<unsigned long>::read (r), Zb1<unsigned long>::read (e));

        EXPECT_EQ (Zl2<unsigned long>::read (l) * Zl2<unsigned long>::read (r), Zl2<unsigned long>::read (e));
        EXPECT_EQ (Zb2<unsigned long>::read (l) * Zb2<unsigned long>::read (r), Zb2<unsigned long>::read (e));

        EXPECT_EQ (Zl1<unsigned long long>::read (l) * Zl1<unsigned long long>::read (r), Zl1<unsigned long long>::read (e));
        EXPECT_EQ (Zb1<unsigned long long>::read (l) * Zb1<unsigned long long>::read (r), Zb1<unsigned long long>::read (e));

        EXPECT_EQ (Zl2<unsigned long long>::read (l) * Zl2<unsigned long long>::read (r), Zl2<unsigned long long>::read (e));
        EXPECT_EQ (Zb2<unsigned long long>::read (l) * Zb2<unsigned long long>::read (r), Zb2<unsigned long long>::read (e));

        // TODO test bounded numbers

    }
    
    void test_multiply_unsigned (const string &left, const string &right, const string &expected) {
        
        dec_uint l (left);
        dec_uint r (right);
        dec_uint e (expected);
        EXPECT_EQ (l * r, e);
        EXPECT_EQ (N (l) * N (r), N (e));

        EXPECT_EQ (Nl<byte>::read (l) * Nl<byte>::read (r), Nl<byte>::read (e));
        EXPECT_EQ (Nb<byte>::read (l) * Nb<byte>::read (r), Nb<byte>::read (e));

        EXPECT_EQ (Nl<unsigned short>::read (l) * Nl<unsigned short>::read (r), Nl<unsigned short>::read (e));
        EXPECT_EQ (Nb<unsigned short>::read (l) * Nb<unsigned short>::read (r), Nb<unsigned short>::read (e));

        EXPECT_EQ (Nl<unsigned>::read (l) * Nl<unsigned>::read (r), Nl<unsigned>::read (e));
        EXPECT_EQ (Nb<unsigned>::read (l) * Nb<unsigned>::read (r), Nb<unsigned>::read (e));

        EXPECT_EQ (Nl<unsigned long>::read (l) * Nl<unsigned long>::read (r), Nl<unsigned long>::read (e));
        EXPECT_EQ (Nb<unsigned long>::read (l) * Nb<unsigned long>::read (r), Nb<unsigned long>::read (e));

        EXPECT_EQ (Nl<unsigned long long>::read (l) * Nl<unsigned long long>::read (r), Nl<unsigned long long>::read (e));
        EXPECT_EQ (Nb<unsigned long long>::read (l) * Nb<unsigned long long>::read (r), Nb<unsigned long long>::read (e));
        
        test_multiply_signed (left, right, expected);

        // TODO test bounded numbers
    }
    
    TEST (Decimal, DecimalMultiply) {

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
