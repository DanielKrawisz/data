// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/unicode.hpp"
#include "data/tools/schema.hpp"
#include "data/math.hpp"
#include "data/net/URL.hpp"
#include "data/net/JSON.hpp"
#include "gtest/gtest.h"

namespace data::schema::rule {

    static_assert (Same<map<empty>, decltype (-map<blank> {})>);
    static_assert (Same<map<blank>, decltype (+map<empty> {})>);

    static_assert (Same<map<only<value<int>>>, decltype (schema::map::key<int> ("abcd"))>);
    static_assert (Same<map<value<int>>, decltype (+schema::map::key<int> ("abcd"))>);
    static_assert (Same<map<only<value<int>>>, decltype (-+schema::map::key<int> ("abcd"))>);
    static_assert (Same<map<only<optional<value<int>>>>, decltype (*schema::map::key<int> ("abcd"))>);

    static_assert (Same<map<only<all<value<int>, value<int>>>>, decltype (schema::map::key<int> ("A") && schema::map::key<int> ("B"))>);
    static_assert (Same<map<all<value<int>, value<int>>>, decltype (schema::map::key<int> ("A") && +schema::map::key<int> ("B"))>);
    static_assert (Same<map<only<all<value<int>, optional<value<int>>>>>, decltype (schema::map::key<int> ("A") && *schema::map::key<int> ("B"))>);
    static_assert (Same<map<all<value<int>, value<int>>>, decltype (+(schema::map::key<int> ("A") && schema::map::key<int> ("B")))>);

    static_assert (Same<decltype (schema::map::key<int> ("A") || schema::map::key<int> ("B")),
        map<only<any<value<int>, value<int>>>>>);

    static_assert (Same<map<only<all<value<int>, value<int>, value<int>>>>,
        decltype (schema::map::key<int> ("A") && schema::map::key<int> ("B") && schema::map::key<int> ("C"))>);

    static_assert (Same<map<only<all<value<int>, optional<value<int>>, optional<value<int>>>>>,
        decltype (schema::map::key<int> ("A") && *schema::map::key<int> ("B") && *schema::map::key<int> ("C"))>);

    static_assert (Same<map<only<all<value<int>, optional<all<value<int>, value<int>>>>>>,
        decltype (schema::map::key<int> ("A") && *(schema::map::key<int> ("B") && schema::map::key<int> ("C")))>);

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
        decltype (schema::map::key<int> ("A") && *(schema::map::key<int> ("B") && schema::map::key<int> ("C")) && *schema::map::key<int> ("D"))>);

    // this should mean that the map has a C and either and A or a B and nothing else.
    static_assert (Same<decltype ((schema::map::key<int> ("A") || schema::map::key<int> ("B")) && schema::map::key<int> ("C")),
        map<only<all<any<value<int>, value<int>>, value<int>>>>>);
   
}

namespace data::schema::map {

    TEST (Schema, Valid) {

        EXPECT_TRUE (valid (empty ()));
        EXPECT_TRUE (valid (key<int> ("A")));
        EXPECT_FALSE (valid (key<int> ("A") && key<int> ("A")));
        EXPECT_TRUE (valid (key<int> ("A") && key<int> ("B")));
        EXPECT_FALSE (valid (key<int> ("A") && (key<int> ("B") || key<int> ("A"))));
        EXPECT_TRUE (valid (key<int> ("A") && key<int> ("B") || key<int> ("A") && key<int> ("C")));

    }

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
        auto key_A_and_B = blank_key_A && blank_key_B;
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
        auto key_A_or_key_B = -blank_key_A || -blank_key_B;
        ASSERT_THROW ((validate<> (map_empty, key_A_or_key_B)), missing_key);
        ASSERT_NO_THROW ((validate<> (map_A, key_A_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_or_key_B)));
        EXPECT_THROW ((validate<> (map_AB, key_A_or_key_B)), incomplete_match);

        auto key_A_opt_and_B = optional_key_A && blank_key_B;
        EXPECT_THROW ((validate<> (map_empty, key_A_opt_and_B)), missing_key);
        EXPECT_THROW ((validate<> (map_A, key_A_opt_and_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_and_B)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_opt_and_B)));

        auto one_key_B = key<uint32> ("B");
        auto optional_key_B = *one_key_B;

        auto blank_key_C = rule::map<rule::value<uint32>> ("C");
        auto one_key_C = key<uint32> ("C");
        auto optional_key_C = *one_key_C;

        auto key_A_opt_and_key_B_opt = optional_key_A && optional_key_B;
        EXPECT_NO_THROW ((validate<> (map_empty, key_A_opt_and_key_B_opt)));
        ASSERT_NO_THROW ((validate<> (map_A, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_opt_and_key_B_opt)));
        EXPECT_NO_THROW ((validate<> (map_AB, key_A_opt_and_key_B_opt)));

        // this ought to match a map with one key B, an empty map, or one with key A.
        auto key_A_opt_or_key_B = optional_key_A || one_key_B;
        EXPECT_NO_THROW ((validate<> (map_empty, key_A_opt_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_A, key_A_opt_or_key_B)));
        ASSERT_NO_THROW ((validate<> (map_B, key_A_opt_or_key_B)));
        ASSERT_THROW ((validate<> (map_AB, key_A_opt_or_key_B)), mismatch);

        map map_AC {{"A", "23"}, {"C", "1293554"}};
        map map_BC {{"B", "567"}, {"C", "1293554"}};

        // basically, you either need to distribute any & operations
        // over | or you need to put the | at the end or it won't work.
        auto test_proof_of_design_issue = (one_key_A || one_key_B) && one_key_C;
        auto test_resolution_of_issue = one_key_C && (one_key_A || one_key_B);

        EXPECT_THROW ((validate<> (map_empty, test_proof_of_design_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_A, test_proof_of_design_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_B, test_proof_of_design_issue)), missing_key);
        ASSERT_THROW ((validate<> (map_AB, test_proof_of_design_issue)), incomplete_match);
        EXPECT_NO_THROW ((validate<> (map_AC, test_proof_of_design_issue)));
        EXPECT_NO_THROW ((validate<> (map_BC, test_proof_of_design_issue)));

        EXPECT_THROW ((validate<> (map_empty, test_resolution_of_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_A, test_resolution_of_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_B, test_resolution_of_issue)), missing_key);
        EXPECT_THROW ((validate<> (map_AB, test_resolution_of_issue)), mismatch);
        EXPECT_NO_THROW ((validate<> (map_AC, test_resolution_of_issue)));
        EXPECT_NO_THROW ((validate<> (map_BC, test_resolution_of_issue)));

        // must have all three.
        auto and_A_B_C = one_key_A && one_key_B && one_key_C;
        ASSERT_THROW ((validate<> (map_empty, and_A_B_C)), missing_key);
        EXPECT_THROW ((validate<> (map_A, and_A_B_C)), missing_key);
        EXPECT_THROW ((validate<> (map_AB, and_A_B_C)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_ABC, and_A_B_C)));

        // must have A and B and may have C.
        auto and_A_B_C_opt = one_key_A && one_key_B && optional_key_C;
        ASSERT_THROW ((validate<> (map_empty, and_A_B_C)), missing_key);
        EXPECT_THROW ((validate<> (map_A, and_A_B_C)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_AB, and_A_B_C_opt)));
        EXPECT_NO_THROW ((validate<> (map_ABC, and_A_B_C_opt)));

        // any map with A or a map with only B.
        auto key_A_blank_or_key_B = blank_key_A || one_key_B;
        ASSERT_THROW ((validate<> (map_empty, key_A_blank_or_key_B)), missing_key);
        EXPECT_NO_THROW ((validate<> (map_A, key_A_blank_or_key_B)));
        EXPECT_NO_THROW ((validate<> (map_B, key_A_blank_or_key_B)));
        EXPECT_THROW ((validate<> (map_AB, key_A_blank_or_key_B)), mismatch);
        ASSERT_NO_THROW ((validate<> (map_AC, key_A_blank_or_key_B)));

        // must have key A and may optionally have B and C.
        auto and_A_opt_B_C = one_key_A && *(one_key_B && one_key_C);
        ASSERT_NO_THROW ((validate<> (map_A, and_A_opt_B_C)));
        EXPECT_NO_THROW (validate<> (map_A, and_A_opt_B_C));
        EXPECT_NO_THROW ((validate<> (map_ABC, and_A_opt_B_C)));
        EXPECT_THROW ((validate<> (map_AB, and_A_opt_B_C)), incomplete_match);

        auto or_A_B_C = one_key_A || one_key_B || one_key_C;

    }

    TEST (Schema, ValidateMap) {

        try {
            test_map_validate<data::map<string, string>> ();
            test_map_validate<std::map<string, string>> ();
        } catch (unknown_key err) {
            FAIL () << "Fail with unknown key " << err.Key;
        } catch (missing_key err) {
            FAIL () << "Fail with missing key " << err.Key;
        } catch (invalid_entry err) {
            FAIL () << "Fail with invalid value " << err.Key;
        } catch (incomplete_match err) {
            FAIL () << "Fail with incomplete match " << err.Key;
        }

    }

    TEST (Schema, Optional) {

        data::map<string, string> test_map {{"na", "Y"}, {"ne", "M"}, {"ty", "W"}};

        auto test_schema = key<std::string> ("na") &&
            *(key<std::string> ("ty") &&
                *key<std::string> ("ne") &&
                *key<std::string> ("co"));

        EXPECT_NO_THROW ((validate<> (test_map, test_schema)));

    }

    TEST (Schema, Read) {

        data::map<UTF8, UTF8> test_map {
            {"A", "23"}, 
            {"B", "129"}, 
            {"C", "-3"},
            {"D", "1255566"}, 
            {"E", "true"}, 
            {"F", "\"true\""},
            {"G", "string with whitespace"}
        }; 

        try {
            
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("A"))), "23");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("B"))), "129");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("C"))), "-3");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("F"))), R"("true")");
            EXPECT_EQ ((validate<> (test_map, +key<std::string> ("G"))), "string with whitespace");

            EXPECT_THROW ((validate<> (test_map, +key<string> ("E"))), invalid_entry);
            EXPECT_EQ ((validate<> (test_map, +key<string> ("F"))), "true");

            EXPECT_EQ (true, (validate<> (test_map, +key<bool> ("E"))));
            EXPECT_THROW ((validate<> (test_map, +key<bool> ("B"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<bool> ("C"))), invalid_entry);

            EXPECT_NO_THROW (validate<> (test_map, +key<JSON> ("A")));
            EXPECT_NO_THROW (validate<> (test_map, +key<JSON> ("E")));
            EXPECT_NO_THROW (validate<> (test_map, +key<JSON> ("F")));

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
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("B"))), invalid_entry);
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

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("C"))), invalid_entry);
            EXPECT_EQ (-3, (validate<> (test_map, +key<signed char> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<uint32> ("C"))), invalid_entry);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int32> ("C"))));
            ASSERT_THROW ((validate<> (test_map, +key<uint32_little> ("C"))), invalid_entry);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int32_little> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<uint128> ("C"))), invalid_entry);
            EXPECT_EQ (-3, (validate<> (test_map, +key<int128> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<N_bytes_little> ("C"))), invalid_entry);
            EXPECT_EQ (-3, (validate<> (test_map, +key<Z_bytes_little> ("C"))));
            EXPECT_THROW ((validate<> (test_map, +key<N> ("C"))), invalid_entry);
            EXPECT_EQ (-3, (validate<> (test_map, +key<Z> ("C"))));

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("D"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("D"))), invalid_entry);
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

            EXPECT_THROW ((validate<> (test_map, +key<byte> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<signed char> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<uint32> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<int32> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<uint32_little> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<int32_little> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<uint128> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<int128> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<N_bytes_little> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<Z_bytes_little> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<N> ("E"))), invalid_entry);
            EXPECT_THROW ((validate<> (test_map, +key<Z> ("E"))), invalid_entry);
        } catch (mismatch) {
            FAIL () << "mismatch caught";
        }

    }

    TEST (Schema, Default) {
        EXPECT_EQ ((validate<> (data::map<string, string> {}, key<int> ("zoob", 13))), 13);
        EXPECT_EQ ((validate<> (data::map<string, string> {{"zoob", "92"}}, key<int> ("zoob", 13))), 92);
    }

    // this test has to do with a specific application that didn't work.
    TEST (Schema, Endpoint) {

        auto endpoint_schema = schema::map::key<net::domain_name> ("domain") ||
            schema::map::key<net::IP::TCP::endpoint> ("endpoint") ||
            (schema::map::key<net::IP::address> ("ip_address") && *schema::map::key<uint32> ("port"));

        auto optional_endpoint_schema = *endpoint_schema;

        data::map<UTF8, UTF8> ip_port_input {
            {"ip_address", "123.23.3.2"},
            {"port", "4567"}
        };

        data::map<UTF8, UTF8> ip_input {
            {"ip_address", "123.23.3.2"}
        };

        data::map<UTF8, UTF8> endpoint_input {
            {"endpoint", "123.23.3.2:4567"}
        };

        data::map<UTF8, UTF8> domain_input {
            {"domain", "zoob.com"}
        };

        try {
            EXPECT_NO_THROW (validate (ip_port_input, endpoint_schema));
            EXPECT_NO_THROW (validate (ip_input, endpoint_schema));
            EXPECT_NO_THROW (validate (endpoint_input, endpoint_schema));
            EXPECT_NO_THROW (validate (domain_input, endpoint_schema));
        } catch (mismatch) {
            FAIL () << "mismatch caught";
        }

    }
}

namespace data::schema::list {

    static_assert (requires {
        value<uint32> (1);
        value<uint32> (1) + value<uint32> (2);
        value<uint32> () + value<uint32> (1);
        equal<uint32> (1) + value<uint32> (2);
    });

    template <typename list> void test_list_validate () {

        list empty_list {};
        list list_1 {"1"};

        EXPECT_NO_THROW (validate<> (empty_list, empty ()));

        EXPECT_THROW ((validate<> (list_1, empty ())), end_of_sequence);

        EXPECT_NO_THROW (validate<> (list_1, value<uint32> ()));
        EXPECT_NO_THROW (validate<> (list_1, equal<uint32> (1)));
        EXPECT_THROW (validate<> (list_1, equal<uint32> (2)), invalid_value_at);

        EXPECT_NO_THROW (validate<> (empty_list, value<uint32> (3)));
        EXPECT_NO_THROW (validate<> (empty_list, *value<uint32> ()));
        EXPECT_NO_THROW (validate<> (list_1, value<uint32> (3)));

        EXPECT_THROW (validate<> (list_1, value<string> (data::string {"huuub"})), invalid_value_at);

        list list_2 {"1", "2"};

        EXPECT_THROW ((validate<> (list_2, value<uint32> ())), no_end_of_sequence);
        EXPECT_THROW ((validate<> (list_2, value<uint32> (1))), no_end_of_sequence);
        EXPECT_THROW ((validate<> (list_2, equal<uint32> (1))), no_end_of_sequence);

        EXPECT_THROW ((validate<> (list_1, value<uint32> () + value<uint32> ())), end_of_sequence);
        EXPECT_THROW ((validate<> (list_1, value<uint32> () + equal<uint32> (2))), end_of_sequence);

        EXPECT_NO_THROW ((validate<> (list_1, value<uint32> () + *value<uint32> ())));
        EXPECT_NO_THROW ((validate<> (list_1, value<uint32> () + value<uint32> (2))));

        EXPECT_NO_THROW ((validate<> (list_2, value<uint32> () + value<uint32> ())));
        EXPECT_NO_THROW ((validate<> (list_2, value<uint32> () + equal<uint32> (2))));
        EXPECT_NO_THROW ((validate<> (list_2, value<uint32> () + *value<uint32> ())));

        EXPECT_NO_THROW ((validate<> (empty_list, value<uint32> (3) + value<uint32> (4))));
        EXPECT_NO_THROW ((validate<> (empty_list, *value<uint32> () + *value<uint32> ())));
        EXPECT_NO_THROW ((validate<> (empty_list, value<uint32> (2) + *value<uint32> ())));

        // need tests with blank.

    }

    TEST (Schema, ValidateList) {

        try {

            test_list_validate<data::list<string>> ();
            test_list_validate<data::stack<string>> ();
            test_list_validate<data::cross<string>> ();

        } catch (invalid_value_at err) {
            FAIL () << "Fail with invalid value at " << err.Position;
        } catch (end_of_sequence err) {
            FAIL () << "Fail with premature end of sequence " << err.Position;
        } catch (no_end_of_sequence err) {
            FAIL () << "Fail with no end of sequence " << err.Position;
        } catch (mismatch &) {
            FAIL () << "unexpected mismatch caught...";
        } catch (std::exception &e) {
            FAIL () << "exception caught with msg " << e.what ();
        }

    }
}
