// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/unicode.hpp"
#include "data/tools/map_schema.hpp"
#include "data/math.hpp"
#include "gtest/gtest.h"

namespace data::schema::rule {
    
    static_assert (Same<map<empty>, decltype (*map<empty> {})>);
    static_assert (Same<map<blank<all<value<int>, value<int>>>>, decltype (key<int> ("A") & +key<int> ("B"))>);
    static_assert (Same<map<blank<all<value<int>, value<int>>>>, decltype (+key<int> ("A") & key<int> ("B"))>);
    static_assert (Same<map<blank<all<value<int>, value<int>>>>, decltype (+key<int> ("A") & +key<int> ("B"))>);
   
}

namespace data::schema {

    template <typename map> void test_map_validate () {
        
        map map_empty {};
        auto empty = rule::map<rule::empty> {};

        EXPECT_NO_THROW (validate<> (map_empty, empty));

        map map_A {{"A", "23"}};

        EXPECT_THROW ((validate<> (map_A, empty)), unknown_key);

        auto blank_empty = +empty;

        EXPECT_NO_THROW ((validate<> (map_empty, blank_empty)));
        EXPECT_NO_THROW ((validate<> (map_A, blank_empty)));

        auto one_key_A = key<uint32> ("A");

        EXPECT_THROW ((validate<> (map_empty, one_key_A)), missing_key);
        ASSERT_NO_THROW ((validate<> (map_A, one_key_A)));

        // let's try to read the value from the map. 
        EXPECT_EQ (23, (validate<> (map_A, one_key_A)).Value);

        auto optional_key_A = *one_key_A;
        
        ASSERT_NO_THROW ((validate<> (map_empty, optional_key_A)));
        ASSERT_NO_THROW ((validate<> (map_A, optional_key_A)));

        map map_B {{"B", "567"}};
        EXPECT_THROW ((validate<> (map_B, one_key_A)), missing_key);
        EXPECT_THROW ((validate<> (map_B, optional_key_A)), unknown_key);

        // this one matches any map and it reads a value A if it exists. 
        auto blank_optional_key_A = +optional_key_A;
        EXPECT_NO_THROW (validate<> (map_empty, blank_optional_key_A));
        EXPECT_NO_THROW (validate<> (map_A, blank_optional_key_A));
        EXPECT_NO_THROW (validate<> (map_B, blank_optional_key_A));

        map map_AB {{"A", "23"}, {"B", "567"}};
        ASSERT_THROW ((validate<> (map_AB, one_key_A)), unknown_key);
        ASSERT_THROW ((validate<> (map_AB, optional_key_A)), unknown_key);
        EXPECT_NO_THROW ((validate<> (map_AB, blank_optional_key_A)));

        // this matches any map with A in it. 
        auto blank_key_A = +one_key_A;
        EXPECT_THROW ((validate<> (map_empty, blank_key_A)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_A, blank_key_A)));
        EXPECT_THROW ((validate<> (map_B, blank_key_A)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_AB, blank_key_A)));

        // this matches either an empty map or a map with A in it. 
        rule::map<rule::optional<rule::blank<rule::value<uint32>>>> optional_blank_key_A = *blank_key_A;
        EXPECT_NO_THROW ((validate<> (map_empty, optional_blank_key_A)));
        EXPECT_NO_THROW ((validate<> (map_A, optional_blank_key_A)));
        EXPECT_THROW ((validate<> (map_B, optional_blank_key_A)), unknown_key);
        ASSERT_NO_THROW ((validate<> (map_AB, optional_blank_key_A)));

        auto one_key_B = key<uint32> ("B");
        auto key_A_and_B = one_key_A & one_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_A, key_A_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_B, key_A_and_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_and_B)));

        auto key_A_opt_and_B = optional_key_A & one_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_opt_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_A, key_A_opt_and_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_and_B)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_opt_and_B)));

        auto optional_key_B = *one_key_B;
        auto key_A_opt_and_key_B_opt = optional_key_A & optional_key_B;
        EXPECT_NO_THROW ((validate<> (map_empty, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_A, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_opt_and_key_B_opt)));
        
        auto key_A_or_key_B = one_key_A | one_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_or_key_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_A, key_A_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_or_key_B)));
        EXPECT_THROW ((validate<> (map_AB, key_A_or_key_B)), unknown_key);
        
        auto key_A_opt_or_key_B = optional_key_A | one_key_B;
        EXPECT_NO_THROW ((validate<> (map_empty, key_A_opt_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_A, key_A_opt_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_or_key_B)));
        EXPECT_THROW ((validate<> (map_AB, key_A_opt_or_key_B)), unknown_key);
        
        // any map with A or a map with only B.
        auto key_A_blank_or_key_B = blank_key_A | one_key_B;
        ASSERT_THROW ((validate<> (map_empty, key_A_blank_or_key_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_A, key_A_blank_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_blank_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_blank_or_key_B)));

    }

    TEST (MapSchema, Validate) {

        test_map_validate<data::map<string, string>> ();
        //test_map_validate<std::map<string, string>> ();

    }

    static_assert(Same<std::uint_least32_t, std::uint32_t>);

    TEST (MapSchema, Read) {

        map<UTF8, UTF8> test_map {
            {"A", "23"}, 
            {"B", "129"}, 
            {"C", "-3"},
            {"D", "1255566"}, 
            {"E", "true"}, 
            {"F", "\"true\""}
        }; 

        try {
            
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("A"))).Value, "23");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("B"))).Value, "129");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("C"))).Value, "-3");
            
            EXPECT_THROW ((validate<> (test_map, +key<string> ("E"))).Value, invalid_value);
            EXPECT_EQ ((validate<> (test_map, +key<string> ("F"))).Value, "true");
            
            EXPECT_EQ (true, (validate<> (test_map, +key<bool> ("E"))).Value);
            EXPECT_THROW ((validate<> (test_map, +key<bool> ("B"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<bool> ("C"))), invalid_value);
    
            EXPECT_EQ (23, (validate<> (test_map, +key<byte> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<signed char> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<uint32> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<int32> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<uint32_little> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<int32_little> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<uint128> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<int128> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<N_bytes_little> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<Z_bytes_little> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<N> ("A")).Value));
            EXPECT_EQ (23, (validate<> (test_map, +key<Z> ("A")).Value));

            EXPECT_EQ (129, (validate<> (test_map, +key<byte> ("B")).Value));
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("B"))), invalid_value);
            EXPECT_EQ (129, (validate<> (test_map, +key<uint32> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<int32> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<uint32_little> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<int32_little> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<uint128> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<int128> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<N_bytes_little> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<Z_bytes_little> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<N> ("B")).Value));
            EXPECT_EQ (129, (validate<> (test_map, +key<Z> ("B")).Value));

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<signed char> ("C")).Value));
            EXPECT_THROW ((validate<> (test_map, +key<uint32> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int32> ("C")).Value));
            ASSERT_THROW ((validate<> (test_map, +key<uint32_little> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int32_little> ("C")).Value));
            EXPECT_THROW ((validate<> (test_map, +key<uint128> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int128> ("C")).Value));
            EXPECT_THROW ((validate<> (test_map, +key<N_bytes_little> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<Z_bytes_little> ("C")).Value));
            EXPECT_THROW ((validate<> (test_map, +key<N> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<Z> ("C")).Value));

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("D"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("D"))), invalid_value);
            EXPECT_EQ (1255566, (validate<> (test_map, +key<uint32> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<int32> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<uint32_little> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<int32_little> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<uint128> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<int128> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<N_bytes_little> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<Z_bytes_little> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<N> ("D")).Value));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<Z> ("D")).Value));

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<uint32> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<int32> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<uint32_little> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<int32_little> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<uint128> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<int128> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<N_bytes_little> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<Z_bytes_little> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<N> ("E"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<Z> ("E"))), invalid_value);
        } catch (mismatch) {
            FAIL () << "mismatch caught";
        }

    }
}
