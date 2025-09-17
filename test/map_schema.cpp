// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/unicode.hpp"
#include "data/tools/map_schema.hpp"
#include "data/math.hpp"
#include "gtest/gtest.h"

namespace data::schema::rule {

    static_assert (Same<map<empty>, decltype (-map<blank> {})>);
    static_assert (Same<map<blank>, decltype (+map<empty> {})>);

    static_assert (Same<map<only<value<int>>>, decltype (key<int> ("abcd"))>);
    static_assert (Same<map<value<int>>, decltype (+key<int> ("abcd"))>);
    static_assert (Same<map<only<value<int>>>, decltype (-+key<int> ("abcd"))>);
    static_assert (Same<map<only<optional<value<int>>>>, decltype (*key<int> ("abcd"))>);

    static_assert (Same<map<only<all<value<int>, value<int>>>>, decltype (key<int> ("A") & key<int> ("B"))>);
    static_assert (Same<map<all<value<int>, value<int>>>, decltype (key<int> ("A") & +key<int> ("B"))>);
    static_assert (Same<map<only<all<value<int>, optional<value<int>>>>>, decltype (key<int> ("A") & *key<int> ("B"))>);
    static_assert (Same<map<all<value<int>, value<int>>>, decltype (+(key<int> ("A") & key<int> ("B")))>);

    static_assert (Same<decltype (key<int> ("A") | key<int> ("B")),
        map<only<any<value<int>, value<int>>>>>);

    static_assert (Same<map<only<all<value<int>, value<int>, value<int>>>>,
        decltype (key<int> ("A") & key<int> ("B") & key<int> ("C"))>);

    static_assert (Same<map<only<all<value<int>, optional<value<int>>, optional<value<int>>>>>,
        decltype (key<int> ("A") & *key<int> ("B") & *key<int> ("C"))>);

    static_assert (Same<map<only<all<value<int>, optional<all<value<int>, value<int>>>>>>,
        decltype (key<int> ("A") & *(key<int> ("B") & key<int> ("C")))>);

    static_assert (Same<
        map<
            only<
                all<
                    value<int>,
                    optional<
                        all<value<int>, value<int>>
                    >,
                    optional<value<int>>
                >
            >
        >,
        decltype (key<int> ("A") & *(key<int> ("B") & key<int> ("C")) & *key<int> ("D"))>);

    // this should mean that the map has a C and either and A or a B and nothing else.
    static_assert (Same<decltype ((key<int> ("A") | key<int> ("B")) & key<int> ("C")),
        map<only<all<any<value<int>, value<int>>, value<int>>>>>);
   
}

namespace data::schema {

    template <typename map> void test_map_validate () {
        
        map map_empty {};
        map map_A {{"A", "23"}};
        map map_B {{"B", "567"}};
        map map_AB {{"A", "23"}, {"B", "567"}};
        map map_ABC {{"A", "23"}, {"B", "567"}, {"C", "1293554"}};

        EXPECT_NO_THROW (validate<> (map_empty, empty ()));
        EXPECT_THROW ((validate<> (map_A, empty ())), unknown_key);
        EXPECT_THROW ((validate<> (map_B, empty ())), unknown_key);
        EXPECT_THROW ((validate<> (map_AB, empty ())), unknown_key);

        auto blank = +empty ();

        EXPECT_NO_THROW ((validate<> (map_empty, blank)));
        EXPECT_NO_THROW ((validate<> (map_A, blank)));
        EXPECT_NO_THROW ((validate<> (map_B, blank)));
        EXPECT_NO_THROW ((validate<> (map_AB, blank)));

        // this matches any map with A in it.
        auto blank_key_A = rule::map<rule::value<uint32>> ("A");

        EXPECT_THROW ((validate<> (map_empty, blank_key_A)), missing_key);
        ASSERT_NO_THROW (EXPECT_EQ (23, (validate<> (map_A, blank_key_A))));
        EXPECT_THROW ((validate<> (map_B, blank_key_A)), missing_key);
        EXPECT_NO_THROW (EXPECT_EQ (23, (validate<> (map_AB, blank_key_A))));

        auto blank_key_B = rule::map<rule::value<uint32>> ("B");

        EXPECT_THROW ((validate<> (map_empty, blank_key_B)), missing_key);
        EXPECT_THROW ((validate<> (map_A, blank_key_B)), missing_key);
        ASSERT_NO_THROW (EXPECT_EQ (567, (validate<> (map_B, blank_key_B))));
        EXPECT_NO_THROW (EXPECT_EQ (567, (validate<> (map_AB, blank_key_B))));

        // match any map with exactly an A in it.
        auto one_key_A = -rule::map<rule::value<uint32>> ("A");

        EXPECT_THROW ((validate<> (map_empty, one_key_A)), missing_key);
        ASSERT_NO_THROW (EXPECT_EQ (23, (validate<> (map_A, one_key_A))));
        EXPECT_THROW ((validate<> (map_B, one_key_A)), missing_key);
        EXPECT_THROW ((validate<> (map_AB, one_key_A)), unknown_key);
        auto key_A_and_B = blank_key_A & blank_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_A, key_A_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_B, key_A_and_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_and_B)));
        EXPECT_NO_THROW ((validate<> (map_ABC, key_A_and_B)));
        EXPECT_THROW ((validate<> (map_ABC, -key_A_and_B)), unknown_key);

        // this one matches empty or just A.
        auto optional_key_A = *key<uint32> ("A");
        EXPECT_NO_THROW (EXPECT_EQ (maybe<uint32> {}, (validate<> (map_empty, optional_key_A))));
        EXPECT_NO_THROW (EXPECT_EQ (maybe<uint32> {23}, (validate<> (map_A, optional_key_A))));
        EXPECT_THROW ((validate<> (map_B, optional_key_A)), unknown_key);
        EXPECT_THROW ((validate<> (map_AB, optional_key_A)), unknown_key);

        // this will match any map but will only return a
        // value of type uint32 if key A was present.
        rule::map<rule::optional<rule::value<uint32>>> optional_blank_key_A {blank_key_A};
        EXPECT_NO_THROW (EXPECT_EQ (maybe<uint32> {}, (validate<> (map_empty, optional_blank_key_A))));
        EXPECT_NO_THROW (EXPECT_EQ (maybe<uint32> {23}, (validate<> (map_A, optional_blank_key_A))));
        EXPECT_NO_THROW (EXPECT_EQ (maybe<uint32> {}, (validate<> (map_B, optional_blank_key_A))));
        EXPECT_NO_THROW (EXPECT_EQ (maybe<uint32> {23}, (validate<> (map_AB, optional_blank_key_A))));

        // match a map with A or a map with B.
        auto key_A_or_key_B = -blank_key_A | -blank_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_or_key_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_A, key_A_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_or_key_B)));
        EXPECT_THROW ((validate<> (map_AB, key_A_or_key_B)), unknown_key);

        auto key_A_opt_and_B = optional_key_A & blank_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_opt_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_A, key_A_opt_and_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_and_B)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_opt_and_B)));

        auto one_key_B = key<uint32> ("B");
        auto optional_key_B = *one_key_B;

        auto blank_key_C = rule::map<rule::value<uint32>> ("C");
        auto one_key_C = key<uint32> ("C");
        auto optional_key_C = *one_key_C;

        auto key_A_opt_and_key_B_opt = optional_key_A & optional_key_B;
        EXPECT_NO_THROW ((validate<> (map_empty, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_A, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_opt_and_key_B_opt)));

        // this ought to match a map with one key B, an empty map, or one with key A.
        auto key_A_opt_or_key_B = optional_key_A | one_key_B;
        EXPECT_NO_THROW ((validate<> (map_empty, key_A_opt_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_A, key_A_opt_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_or_key_B)));
        EXPECT_THROW ((validate<> (map_AB, key_A_opt_or_key_B)), unknown_key);

        map map_AC {{"A", "23"}, {"C", "1293554"}};
        map map_BC {{"B", "567"}, {"C", "1293554"}};

        // basically, you either need to distribute any & operations
        // over | or you need to put the | at the end or it won't work.
        auto test_proof_of_design_issue = (one_key_A | one_key_B) & one_key_C;
        auto test_resolution_of_issue = one_key_C & (one_key_A | one_key_B);

        EXPECT_THROW ((validate<> (map_empty, test_proof_of_design_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_A, test_proof_of_design_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_B, test_proof_of_design_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_AB, test_proof_of_design_issue)), unknown_key);
        //EXPECT_NO_THROW ((validate<> (map_AC, test_proof_of_design_issue)));
        //EXPECT_NO_THROW ((validate<> (map_BC, test_proof_of_design_issue)));

        EXPECT_THROW ((validate<> (map_empty, test_resolution_of_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_A, test_resolution_of_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_B, test_resolution_of_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_AB, test_resolution_of_issue)), mismatch);
        EXPECT_NO_THROW ((validate<> (map_AC, test_resolution_of_issue)));
        EXPECT_NO_THROW ((validate<> (map_BC, test_resolution_of_issue)));

        // must have all three.
        auto and_A_B_C = one_key_A & one_key_B & one_key_C;
        ASSERT_THROW ((validate<> (map_empty, and_A_B_C)), missing_key);
        EXPECT_THROW ((validate<> (map_A, and_A_B_C)), missing_key);
        EXPECT_THROW ((validate<> (map_AB, and_A_B_C)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_ABC, and_A_B_C)));

        // must have A and B and may have C.
        auto and_A_B_C_opt = one_key_A & one_key_B & optional_key_C;
        ASSERT_THROW ((validate<> (map_empty, and_A_B_C)), missing_key);
        EXPECT_THROW ((validate<> (map_A, and_A_B_C)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_AB, and_A_B_C_opt)));
        EXPECT_NO_THROW ((validate<> (map_ABC, and_A_B_C_opt)));

        // any map with A or a map with only B.
        auto key_A_blank_or_key_B = blank_key_A | one_key_B;
        ASSERT_THROW ((validate<> (map_empty, key_A_blank_or_key_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_A, key_A_blank_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_blank_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_blank_or_key_B)));

        // must have key A and may optionally have B and C.
        auto and_A_opt_B_C = one_key_A & *(one_key_B & one_key_C);
        ASSERT_NO_THROW ((validate<> (map_A, and_A_opt_B_C)));
        EXPECT_NO_THROW (validate<> (map_A, and_A_opt_B_C));
        EXPECT_NO_THROW ((validate<> (map_ABC, and_A_opt_B_C)));
        EXPECT_THROW ((validate<> (map_AB, and_A_opt_B_C)), mismatch);

    }

    TEST (MapSchema, Validate) {

        try {
            test_map_validate<data::map<string, string>> ();
            //test_map_validate<std::map<string, string>> ();
        } catch (unknown_key err) {
            FAIL () << "Fail with unknown key " << err.Key;
        } catch (missing_key err) {
            FAIL () << "Fail with missing key " << err.Key;
        } catch (invalid_value err) {
            FAIL () << "Fail with invalid value " << err.Key;
        }

    }

    static_assert (Same<std::uint_least32_t, std::uint32_t>);

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
            
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("A"))), "23");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("B"))), "129");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("C"))), "-3");
            
            EXPECT_THROW ((validate<> (test_map, +key<string> ("E"))), invalid_value);
            EXPECT_EQ ((validate<> (test_map, +key<string> ("F"))), "true");
            
            EXPECT_EQ (true, (validate<> (test_map, +key<bool> ("E"))));
            EXPECT_THROW ((validate<> (test_map, +key<bool> ("B"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<bool> ("C"))), invalid_value);
    
            EXPECT_EQ (23, (validate<> (test_map, +key<byte> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<signed char> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<uint32> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<int32> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<uint32_little> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<int32_little> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<uint128> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<int128> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<N_bytes_little> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<Z_bytes_little> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<N> ("A"))));
            EXPECT_EQ (23, (validate<> (test_map, +key<Z> ("A"))));

            EXPECT_EQ (129, (validate<> (test_map, +key<byte> ("B"))));
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("B"))), invalid_value);
            EXPECT_EQ (129, (validate<> (test_map, +key<uint32> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<int32> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<uint32_little> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<int32_little> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<uint128> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<int128> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<N_bytes_little> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<Z_bytes_little> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<N> ("B"))));
            EXPECT_EQ (129, (validate<> (test_map, +key<Z> ("B"))));

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<signed char> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<uint32> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int32> ("C"))));
            ASSERT_THROW ((validate<> (test_map, +key<uint32_little> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int32_little> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<uint128> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int128> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<N_bytes_little> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<Z_bytes_little> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<N> ("C"))), invalid_value);
            EXPECT_EQ (-3, (validate<> (test_map, +key<Z> ("C"))));

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("D"))), invalid_value);
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("D"))), invalid_value);
            EXPECT_EQ (1255566, (validate<> (test_map, +key<uint32> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<int32> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<uint32_little> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<int32_little> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<uint128> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<int128> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<N_bytes_little> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<Z_bytes_little> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<N> ("D"))));
            EXPECT_EQ (1255566, (validate<> (test_map, +key<Z> ("D"))));

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
