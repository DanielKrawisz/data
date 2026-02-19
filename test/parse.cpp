// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/parse/URL.hpp>
#include <data/parse/XML.hpp>
#include <data/parse/JSON.hpp>
#include "gtest/gtest.h"
#include <regex>

namespace data::parse {

    TEST (Parse, Basics) {

        EXPECT_FALSE (accept<invalid> (""));
        EXPECT_FALSE (accept<invalid> ("a"));
        EXPECT_FALSE (accept<invalid> ("anything"));

        EXPECT_TRUE  (accept<any> (""));
        EXPECT_TRUE  (accept<any> ("a"));
        EXPECT_TRUE  (accept<any> ("abc"));
        EXPECT_TRUE  (accept<any> (" "));

        EXPECT_TRUE  ((accept<max_size<any, 0>> ("")));
        EXPECT_FALSE ((accept<max_size<any, 0>> ("a")));

        EXPECT_TRUE  ((accept<max_size<any, 3>> ("")));
        EXPECT_TRUE  ((accept<max_size<any, 3>> ("a")));
        EXPECT_TRUE  ((accept<max_size<any, 3>> ("abc")));
        EXPECT_FALSE ((accept<max_size<any, 3>> ("abcd")));
    }

    TEST (Parse, Optional) {

        EXPECT_TRUE  (accept<optional<invalid>> (""));
        EXPECT_FALSE (accept<optional<invalid>> (" "));
        EXPECT_FALSE (accept<optional<invalid>> ("a"));

        EXPECT_TRUE  (accept<optional<any>> (""));
        EXPECT_TRUE  (accept<optional<any>> ("a"));
        EXPECT_TRUE  (accept<optional<any>> ("abc"));

        using opt_a = optional<one<'a'>>;

        EXPECT_TRUE  (accept<opt_a> (""));      // zero case
        EXPECT_TRUE  (accept<opt_a> ("a"));     // one case
        EXPECT_FALSE (accept<opt_a> ("aa"));    // too many
        EXPECT_FALSE (accept<opt_a> ("b"));     // invalid char

        EXPECT_FALSE (accept<opt_a> ("ab"));
        EXPECT_FALSE (accept<opt_a> ("ba"));

        using opt_opt_a = optional<optional<one<'a'>>>;

        EXPECT_TRUE  (accept<opt_opt_a> (""));
        EXPECT_TRUE  (accept<opt_opt_a> ("a"));
        EXPECT_FALSE (accept<opt_opt_a> ("aa"));

    }

    TEST (Parse, SequenceAB) {
        {
            sequence<one<'a'>, one<'b'>> r {};
            EXPECT_FALSE ((r.valid ()));
            EXPECT_TRUE ((r.possible ()));

            r.step (string_view {}, 'b');
            EXPECT_FALSE ((r.valid ()));
            EXPECT_FALSE ((r.possible ()));
        }
        {
            sequence<one<'a'>, one<'b'>> r {};
            char str[] = "aa";

            r.step (string_view {str, 0}, 'a');
            EXPECT_FALSE ((r.valid ()));
            EXPECT_TRUE ((r.possible ()));
            r.step (string_view {str, 1}, 'a');
            EXPECT_FALSE ((r.valid ()));
            EXPECT_FALSE ((r.possible ()));
        }
        {
            sequence<one<'a'>, one<'b'>> r {};
            char str[] = "abc";

            r.step (string_view {str, 0}, 'a');
            EXPECT_FALSE ((r.valid ()));
            EXPECT_TRUE ((r.possible ()));
            r.step (string_view {str, 1}, 'b');
            EXPECT_TRUE ((r.valid ()));
            EXPECT_FALSE ((r.possible ()));
            r.step (string_view {str, 2}, 'c');
            EXPECT_FALSE ((r.valid ()));
            EXPECT_FALSE ((r.possible ()));
        }



    }

    TEST (Parse, Sequence) {

        EXPECT_TRUE  ((accept<sequence<any, any>> ("")));
        EXPECT_TRUE  ((accept<sequence<any, any>> ("a")));
        EXPECT_TRUE  ((accept<sequence<any, any>> ("abc")));

        EXPECT_FALSE ((accept<sequence<one<'a'>, one<'b'>>> ("")));
        EXPECT_FALSE ((accept<sequence<one<'a'>, one<'b'>>> ("a")));
        EXPECT_TRUE  ((accept<sequence<one<'a'>, one<'b'>>> ("ab")));
        EXPECT_FALSE ((accept<sequence<one<'a'>, one<'b'>>> ("aa")));
        EXPECT_FALSE ((accept<sequence<one<'a'>, one<'b'>>> ("abc")));
        EXPECT_FALSE ((accept<sequence<one<'a'>, one<'b'>>> ("b")));

        using P = sequence<exactly<>, exactly<'a'>>;

        EXPECT_FALSE ((accept<P> ("")));
        EXPECT_TRUE  ((accept<P> ("a")));
        EXPECT_FALSE ((accept<P> ("aa")));

        using seq_ab_c = sequence<
            exactly<'a','b'>,
            exactly<'c'>>;

        EXPECT_FALSE (accept<seq_ab_c> (""));
        EXPECT_TRUE  (accept<seq_ab_c> ("abc"));
        EXPECT_FALSE (accept<seq_ab_c> ("ab"));
        EXPECT_FALSE (accept<seq_ab_c> ("a"));
        EXPECT_FALSE (accept<seq_ab_c> ("abcd"));
        EXPECT_FALSE (accept<seq_ab_c> ("xbc"));
        EXPECT_FALSE (accept<seq_ab_c> ("abd"));

        using seq_a_bc = sequence<
            exactly<'a'>,
            exactly<'b','c'>>;

        EXPECT_FALSE (accept<seq_a_bc> (""));
        EXPECT_TRUE  (accept<seq_a_bc> ("abc"));
        EXPECT_FALSE (accept<seq_a_bc> ("ab"));
        EXPECT_FALSE (accept<seq_a_bc> ("a"));
        EXPECT_FALSE (accept<seq_a_bc> ("abcd"));
        EXPECT_FALSE (accept<seq_a_bc> ("xbc"));
        EXPECT_FALSE (accept<seq_a_bc> ("abd"));

        using seq_ab_cd = sequence<
            exactly<'a','b'>,
            exactly<'c','d'>
        >;

        EXPECT_TRUE  (accept<seq_ab_cd> ("abcd"));
        EXPECT_FALSE (accept<seq_ab_cd> ("abc"));
        EXPECT_FALSE (accept<seq_ab_cd> ("ab"));
        EXPECT_FALSE (accept<seq_ab_cd> ("abcdx"));
        EXPECT_FALSE (accept<seq_ab_cd> ("abxd"));
        EXPECT_FALSE (accept<seq_ab_cd> ("xbcd"));
        EXPECT_FALSE (accept<seq_ab_cd> ("aecd"));

        using seq_abc = sequence<
            exactly<'a'>,
            exactly<'b'>,
            exactly<'c'>
        >;

        EXPECT_TRUE  (accept<seq_abc> ("abc"));
        EXPECT_FALSE (accept<seq_abc> ("ab"));
        EXPECT_FALSE (accept<seq_abc> ("abcd"));
        EXPECT_FALSE (accept<seq_abc> ("a"));
        EXPECT_FALSE (accept<seq_abc> (""));
        EXPECT_FALSE (accept<seq_abc> ("xbc"));
        EXPECT_FALSE (accept<seq_abc> ("abd"));

        EXPECT_FALSE ((accept<sequence<optional<exactly<'a'>>, exactly<'b'>>> ("")));
        EXPECT_TRUE  ((accept<sequence<optional<exactly<'a'>>, exactly<'b'>>> ("b")));
        EXPECT_TRUE  ((accept<sequence<optional<exactly<'a'>>, exactly<'b'>>> ("ab")));
        EXPECT_FALSE ((accept<sequence<optional<exactly<'a'>>, exactly<'b'>>> ("a")));
        EXPECT_FALSE ((accept<sequence<optional<exactly<'a'>>, exactly<'b'>>> ("bb")));
    }

    TEST (Parse, OptionalSequence) {
        using test_case = sequence<
            optional<one<'a'>>,
            optional<one<'b'>>,
            optional<one<'c'>>>;

            EXPECT_TRUE ((accept<test_case> ("")));
            EXPECT_TRUE ((accept<test_case> ("a")));
            EXPECT_TRUE ((accept<test_case> ("b")));
            EXPECT_TRUE ((accept<test_case> ("c")));
            EXPECT_TRUE ((accept<test_case> ("ab")));
            EXPECT_TRUE ((accept<test_case> ("ac")));
            EXPECT_TRUE ((accept<test_case> ("bc")));
            EXPECT_TRUE ((accept<test_case> ("abc")));
    }

    TEST (Parse, Alternatives) {

        EXPECT_FALSE ((accept<alternatives<invalid, invalid>> ("")));
        EXPECT_FALSE ((accept<alternatives<invalid, invalid>> ("a")));

        EXPECT_FALSE ((accept<alternatives<one<'a'>, one<'b'>>> ("")));
        EXPECT_TRUE  ((accept<alternatives<one<'a'>, one<'b'>>> ("a")));
        EXPECT_TRUE  ((accept<alternatives<one<'a'>, one<'b'>>> ("b")));
        EXPECT_FALSE ((accept<alternatives<one<'a'>, one<'b'>>> ("ab")));
        EXPECT_FALSE ((accept<alternatives<one<'a'>, one<'b'>>> ("c")));

        using O = alternatives<
            exactly<'a','b','c'>,
            exactly<'a','b','d'>
        >;

        EXPECT_TRUE  (accept<O> ("abd"));
        EXPECT_TRUE  (accept<O> ("abc"));
        EXPECT_FALSE (accept<O> ("abe"));

        // this case won't work now but we need to get it to work.
        using P = sequence<
            alternatives<
                exactly<'a','b','c'>,
                exactly<'a','b'>
            >,
            exactly<'d'>
        >;

        EXPECT_TRUE  (accept<P> ("abcd"));
        EXPECT_TRUE  (accept<P> ("abd"));
        EXPECT_FALSE (accept<P> ("abc"));

        using prefix_alt_a = alternatives<exactly<'a', 'b'>, exactly<'a', 'b', 'c'>>;
        using prefix_alt_b = alternatives<exactly<'a', 'b', 'c'>, exactly<'a', 'b'>>;

        EXPECT_TRUE  ((accept<prefix_alt_a> ("ab")));
        EXPECT_TRUE  ((accept<prefix_alt_a> ("abc")));
        EXPECT_FALSE ((accept<prefix_alt_a> ("abcd")));
        EXPECT_FALSE ((accept<prefix_alt_a> ("a")));
        EXPECT_FALSE ((accept<prefix_alt_a> ("")));

        EXPECT_TRUE  ((accept<prefix_alt_b> ("ab")));
        EXPECT_TRUE  ((accept<prefix_alt_b> ("abc")));
        EXPECT_FALSE ((accept<prefix_alt_b> ("abcd")));
        EXPECT_FALSE ((accept<prefix_alt_b> ("a")));
        EXPECT_FALSE ((accept<prefix_alt_b> ("")));

        using empty_alt = alternatives<exactly<'a'>, exactly<>>;

        EXPECT_TRUE  ((accept<empty_alt>("")));
        EXPECT_TRUE  ((accept<empty_alt>("a")));
        EXPECT_FALSE ((accept<empty_alt>("aa")));
        EXPECT_FALSE ((accept<empty_alt>("b")));

        using optional_overlap = alternatives<exactly<'a', 'b', 'c'>, optional<exactly<'a', 'b', 'c'>>>;

        EXPECT_TRUE  ((accept<optional_overlap> ("")));
        EXPECT_TRUE  ((accept<optional_overlap> ("abc")));
        EXPECT_FALSE ((accept<optional_overlap> ("abcabc")));
        EXPECT_FALSE ((accept<optional_overlap> ("ab")));

        using shared_prefix = alternatives<exactly<'a', 'b'>, exactly<'a', 'c'>>;

        EXPECT_TRUE  ((accept<shared_prefix>("ab")));
        EXPECT_TRUE  ((accept<shared_prefix>("ac")));
        EXPECT_FALSE ((accept<shared_prefix>("a")));
        EXPECT_FALSE ((accept<shared_prefix>("ad")));
    }

    TEST (Parse, Repeated) {

        EXPECT_TRUE  (accept<star<any>> (""));
        EXPECT_TRUE  (accept<star<any>> ("a"));
        EXPECT_TRUE  (accept<star<any>> ("abc"));

        EXPECT_TRUE  (accept<star<one<'a'>>> (""));
        EXPECT_TRUE  (accept<star<one<'a'>>> ("a"));
        EXPECT_TRUE  (accept<star<one<'a'>>> ("aaaa"));
        EXPECT_FALSE (accept<star<one<'a'>>> ("b"));
        EXPECT_FALSE (accept<star<one<'a'>>> ("aaab"));
        EXPECT_FALSE (accept<star<one<'a'>>> ("baaa"));

        EXPECT_TRUE  ((accept<star<exactly<>>> ("")));
        EXPECT_TRUE  ((accept<plus<exactly<>>> ("")));
        EXPECT_TRUE  ((accept<repeated<exactly<>, 10>> ("")));

        EXPECT_TRUE  (accept<repeated<invalid>> (""));
        EXPECT_FALSE ((accept<plus<invalid>> ("")));
        EXPECT_FALSE (accept<repeated<invalid>> (" "));
        EXPECT_FALSE (accept<repeated<invalid>> ("a"));

        EXPECT_FALSE ((accept<plus<invalid>> ("")));
        EXPECT_FALSE ((accept<plus<invalid>> ("a")));

        using M = sequence<
            repeated<exactly<'a'>>,
            exactly<'b'>>;

        EXPECT_TRUE  (accept<M> ("b"));
        EXPECT_TRUE  (accept<M> ("ab"));
        EXPECT_TRUE  (accept<M> ("aaaaab"));
        EXPECT_FALSE (accept<M> ("a"));
        EXPECT_FALSE (accept<M> ("aaaaa"));

        using N = repeated<exactly<'a'>, 2, 4>;

        EXPECT_FALSE (accept<N> (""));
        EXPECT_FALSE (accept<N> ("a"));
        EXPECT_TRUE  (accept<N> ("aa"));
        EXPECT_TRUE  (accept<N> ("aaa"));
        EXPECT_TRUE  (accept<N> ("aaaa"));
        EXPECT_FALSE (accept<N> ("aaaaa"));
        EXPECT_FALSE (accept<N> ("b"));
        EXPECT_FALSE (accept<N> ("aab"));
        EXPECT_FALSE (accept<N> ("baa"));

        using O = repeated<exactly<'a'>, 1>;

        EXPECT_FALSE (accept<O> (""));
        EXPECT_TRUE  (accept<O> ("a"));
        EXPECT_TRUE  (accept<O> ("aaaa"));
        EXPECT_FALSE (accept<O> ("b"));
        EXPECT_FALSE (accept<O> ("aaab"));

        using P = repeated<exactly<'a'>, 0, 3>;

        EXPECT_TRUE  (accept<P> (""));
        EXPECT_TRUE  (accept<P> ("a"));
        EXPECT_TRUE  (accept<P> ("aaa"));
        EXPECT_FALSE (accept<P> ("aaaa"));
        EXPECT_FALSE (accept<P> ("b"));

        using Q = sequence<
            repeated<exactly<'a'>, 0, 3>,
            exactly<'b'>
        >;

        EXPECT_TRUE  (accept<Q> ("b"));
        EXPECT_TRUE  (accept<Q> ("ab"));
        EXPECT_TRUE  (accept<Q> ("aaab"));
        EXPECT_FALSE (accept<Q> ("aaaab"));
        EXPECT_FALSE (accept<Q> ("a"));

        using S = repeated<exactly<'a'>, 3, 3>;

        EXPECT_FALSE (accept<S> (""));
        EXPECT_FALSE (accept<S> ("aa"));
        EXPECT_TRUE  (accept<S> ("aaa"));
        EXPECT_FALSE (accept<S> ("aaaa"));

        using U = repeated<exactly<'a'>, 5, 10>;

        EXPECT_FALSE (accept<U> (""));
        EXPECT_FALSE (accept<U> ("aaaa"));
        EXPECT_TRUE  (accept<U> ("aaaaa"));
    }

    TEST (Parse, Backtrack) {

        using M = sequence<
            repeated<exactly<'a','b','c','d'>>,
            exactly<'a','b','d'>>;

        EXPECT_TRUE (accept<M> ("abcdabd"));

        EXPECT_TRUE  (accept<M> ("abcdabcdabcdabd"));
        EXPECT_FALSE (accept<M> ("abcdabcdabcdab"));
        EXPECT_FALSE (accept<M> ("abcdabcdabcdabx"));

        using Q = sequence<
            repeated<exactly<'a','b'>>,
            exactly<'b'>
        >;

        EXPECT_TRUE  (accept<Q> ("abb"));
        EXPECT_TRUE  (accept<Q> ("abababb"));
        EXPECT_FALSE (accept<Q> ("ababab"));

    }

    TEST (Parse, Exactly) {

        EXPECT_TRUE  ((accept<exactly<>> ("")));
        EXPECT_FALSE ((accept<exactly<>> ("a")));

        EXPECT_FALSE (accept<exactly<'a'>> (""));
        EXPECT_TRUE  (accept<exactly<'a'>> ("a"));
        EXPECT_FALSE (accept<exactly<'a'>> ("aa"));
        EXPECT_FALSE (accept<exactly<'a'>> ("b"));

        EXPECT_TRUE  ((accept<exactly<'a','b','c'>> ("abc")));
        EXPECT_FALSE ((accept<exactly<'a','b','c'>> ("ab")));
        EXPECT_FALSE ((accept<exactly<'a','b','c'>> ("abcd")));

        EXPECT_TRUE  ((accept<exactly<u8'a'>> ("a")));
        EXPECT_TRUE  ((accept<exactly<U'a'>> ("a")));

        EXPECT_TRUE  ((accept<exactly<'a', u8'b', U'c'>> ("abc")));

        EXPECT_TRUE  ((accept<exactly<U'é'>> (u8"é")));
        EXPECT_FALSE ((accept<exactly<U'é'>> ("e")));

        EXPECT_TRUE  ((accept<exactly<U'𐍈'>> (u8"𐍈")));
        EXPECT_FALSE ((accept<exactly<U'𐍈'>> (u8"x")));

        EXPECT_TRUE  ((accept<exactly<'a', U'é', 'z'>> (u8"aéz")));
        EXPECT_FALSE ((accept<exactly<'a', U'é', 'z'>> (u8"aez")));

        EXPECT_TRUE  ((accept<exactly<U'é', U'ø'>> (u8"éø")));
        EXPECT_FALSE ((accept<exactly<U'é', U'ø'>> (u8"éa")));
    }

    TEST (Parse, MaxValue) {

        EXPECT_TRUE ((accept<IP::V4_number> ("0")));
        EXPECT_TRUE ((accept<IP::V4_number> ("1")));
        EXPECT_TRUE ((accept<IP::V4_number> ("9")));
        EXPECT_TRUE ((accept<IP::V4_number> ("10")));
        EXPECT_TRUE ((accept<IP::V4_number> ("99")));
        EXPECT_TRUE ((accept<IP::V4_number> ("100")));
        EXPECT_TRUE ((accept<IP::V4_number> ("255")));

        EXPECT_FALSE ((accept<IP::V4_number> ("")));
        EXPECT_FALSE ((accept<IP::V4_number> ("00")));
        EXPECT_FALSE ((accept<IP::V4_number> ("01")));
        EXPECT_FALSE ((accept<IP::V4_number> ("001")));
        EXPECT_FALSE ((accept<IP::V4_number> ("000")));
        EXPECT_FALSE ((accept<IP::V4_number> ("256")));
        EXPECT_FALSE ((accept<IP::V4_number> ("300")));
        EXPECT_FALSE ((accept<IP::V4_number> ("999")));
        EXPECT_FALSE ((accept<IP::V4_number> ("a")));
        EXPECT_FALSE ((accept<IP::V4_number> ("1a")));
        EXPECT_FALSE ((accept<IP::V4_number> ("a1")));
        EXPECT_FALSE ((accept<IP::V4_number> ("-1")));

        EXPECT_TRUE ((accept<IP::port> ("0")));
        EXPECT_TRUE ((accept<IP::port> ("1")));
        EXPECT_TRUE ((accept<IP::port> ("80")));
        EXPECT_TRUE ((accept<IP::port> ("443")));
        EXPECT_TRUE ((accept<IP::port> ("65535")));

        EXPECT_FALSE ((accept<IP::port> ("65536")));
        EXPECT_FALSE ((accept<IP::port> ("70000")));
        EXPECT_FALSE ((accept<IP::port> ("99999")));
        EXPECT_FALSE ((accept<IP::port> ("00")));
        EXPECT_FALSE ((accept<IP::port> ("01")));
        EXPECT_FALSE ((accept<IP::port> ("0001")));

        EXPECT_FALSE ((accept<IP::port> ("4294967296"))); // large overflow path
        EXPECT_FALSE ((accept<IP::port> ("18446744073709551616")));
    }

    TEST (Parse, Complement) {
        // ----- Basic literal exclusion -----

        using XML      = exactly<'x','m','l'>;
        using XML_only = complement<XML, XML>;

        EXPECT_FALSE ((accept<XML_only> ("xml")));   // forbidden exact match
        EXPECT_FALSE ((accept<XML_only> ("xm")));    // X invalid
        EXPECT_FALSE ((accept<XML_only> ("xmlx")));  // X invalid


        // ----- Name but not "xml" -----

        using Name   = plus<ASCII::alpha>;   // assumes alpha predicate exists
        using NotXML = complement<Name, XML>;

        EXPECT_FALSE ((accept<NotXML> ("xml")));     // excluded
        EXPECT_TRUE  ((accept<NotXML> ("abcd")));    // allowed
        EXPECT_TRUE  ((accept<NotXML> ("xmll")));    // allowed
        EXPECT_TRUE  ((accept<NotXML> ("xmlx")));    // allowed
        EXPECT_FALSE ((accept<NotXML> ("123")));     // Name invalid


        // ----- Y never matches fully -----

        using ZZZ    = exactly<'z','z','z'>;
        using NotZZZ = complement<Name, ZZZ>;

        EXPECT_TRUE  ((accept<NotZZZ> ("xml")));     // Y never valid
        EXPECT_TRUE  ((accept<NotZZZ> ("zzzq")));    // Y invalid at EOF
        EXPECT_FALSE ((accept<NotZZZ> ("zzz")));     // Y valid → reject


        // ----- Different length exclusion -----

        using Hello      = exactly<'h','e','l','l','o'>;
        using HelloBlock = complement<Hello, Hello>;

        EXPECT_FALSE ((accept<HelloBlock>("hello")));
        EXPECT_FALSE ((accept<HelloBlock>("hell")));
        EXPECT_FALSE ((accept<HelloBlock>("helloo")));


        // ----- Y longer than X (should never reject) -----

        using Short  = exactly<'a'>;
        using Longer = exactly<'a','b'>;
        using Ctest  = complement<Short, Longer>;

        EXPECT_TRUE  ((accept<Ctest>("a")));        // Y can never be valid
        EXPECT_FALSE ((accept<Ctest>("ab")));       // X invalid

    }

    TEST (Parse, Until) {
        // ----- Basic sentinel detection -----

        using U1 = until<repeated<one<'a'>>, 'x','y','z'>;

        EXPECT_TRUE  ((accept<U1> ("aaaxyz")));   // normal case
        EXPECT_TRUE  ((accept<U1> ("xyz")));      // terminator immediately
        EXPECT_TRUE  ((accept<U1> ("aaaaxyz")));  // longer prefix

        EXPECT_FALSE ((accept<U1> ("aaaa")));     // no terminator
        EXPECT_FALSE ((accept<U1> ("aaaxy")));    // partial terminator


        // ----- Multiple terminators (stop at first) -----

        EXPECT_FALSE  ((accept<U1> ("aaaxyzxyz")));
        EXPECT_FALSE  ((accept<U1> ("xyzxyz")));


        // ----- Terminator at beginning -----

        EXPECT_FALSE  ((accept<U1> ("xyzabc")));


        // ----- Overlapping prefix handling -----

        using U2 = until<repeated<one<'a'>>, 'a','a','b'>;

        EXPECT_TRUE  ((accept<U2> ("aaab")));
        EXPECT_TRUE  ((accept<U2> ("aaaab")));   // overlap case
        EXPECT_FALSE ((accept<U2> ("aaaaa")));   // never completes "aab")


        // ----- Single-character terminator -----

        using U3 = until<repeated<one<'a'>>, 'z'>;

        EXPECT_TRUE  ((accept<U3> ("aaaz")));
        EXPECT_TRUE  ((accept<U3> ("z")));
        EXPECT_FALSE ((accept<U3> ("aaaa")));


        // ----- Terminator longer pattern -----

        using U4 = until<repeated<one<'b'>>, '1','2','3','4'>;

        EXPECT_TRUE  ((accept<U4> ("bbb1234")));
        EXPECT_FALSE ((accept<U4> ("bbb123")));
        EXPECT_FALSE ((accept<U4> ("bbbb")));

        // Terminator is also fully valid alpha characters

        using U5 = until<repeated<ASCII::alpha>, 'a','b','c'>;


        // ----- Simple detection -----

        EXPECT_TRUE  ((accept<U5> ("abc")));
        EXPECT_TRUE  ((accept<U5> ("xabc")));
        EXPECT_TRUE  ((accept<U5> ("xyzabc")));
        EXPECT_TRUE  ((accept<U5> ("aaaaabc")));


        // ----- No terminator -----

        EXPECT_FALSE ((accept<U5> ("")));
        EXPECT_FALSE ((accept<U5> ("a")));
        EXPECT_FALSE ((accept<U5> ("ab")));
        EXPECT_FALSE ((accept<U5> ("abcd")));     // "abc" must appear as substring
        EXPECT_FALSE ((accept<U5> ("xyz")));      // never saw "abc")


        // ----- Terminator in the middle -----

        EXPECT_FALSE ((accept<U5> ("zzabczz")));
        EXPECT_FALSE ((accept<U5> ("aabcxabc")));   // first abc should terminate


        // ----- Overlapping prefix cases -----

        EXPECT_TRUE  ((accept<U5> ("aabc")));       // overlap: first 'a' restarts prefix
        EXPECT_TRUE  ((accept<U5> ("aaabc")));      // deeper overlap
        EXPECT_TRUE  ((accept<U5> ("abababc")));    // repeated prefix fragments

        EXPECT_FALSE ((accept<U5> ("aaab")));       // never completes full "abc"
        EXPECT_FALSE ((accept<U5> ("ababab")));     // prefix keeps restarting


        // ----- Partial prefix at end -----

        EXPECT_FALSE ((accept<U5> ("za")));
        EXPECT_FALSE ((accept<U5> ("zab")));
        EXPECT_FALSE ((accept<U5> ("zzab")));


        // ----- Multiple occurrences -----

        EXPECT_FALSE ((accept<U5> ("abcabc")));
        EXPECT_FALSE ((accept<U5> ("zzabcabc")));
        EXPECT_TRUE  ((accept<U5> ("ababc")));   // first full "abc" ends it

        // ----- Prefix broken and restarted -----

        EXPECT_TRUE  ((accept<U5> ("abxabc")));   // first attempt fails, second succeeds
        EXPECT_FALSE ((accept<U5> ("abxab")));    // never completes


        // ----- Terminator at very end -----

        EXPECT_TRUE  ((accept<U5> ("zzzabc")));
        EXPECT_FALSE ((accept<U5> ("zzzab")));


    }

    TEST (Parse, IPV4) {

        EXPECT_TRUE  ((accept<IP::V4> ("0.0.0.0")));
        EXPECT_TRUE  ((accept<IP::V4> ("1.2.3.4")));
        EXPECT_TRUE  ((accept<IP::V4> ("127.0.0.1")));
        EXPECT_TRUE  ((accept<IP::V4> ("192.168.0.1")));
        EXPECT_TRUE  ((accept<IP::V4> ("255.255.255.255")));
        EXPECT_TRUE  ((accept<IP::V4> ("10.20.30.40")));

        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.4.5")));
        EXPECT_FALSE ((accept<IP::V4> (".1.2.3")));
        EXPECT_FALSE ((accept<IP::V4> ("1..2.3")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.")));
        EXPECT_FALSE ((accept<IP::V4> ("01.2.3.4")));
        EXPECT_FALSE ((accept<IP::V4> ("1.02.3.4")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.003.4")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.04")));

        EXPECT_FALSE ((accept<IP::V4> ("256.0.0.1")));
        EXPECT_FALSE ((accept<IP::V4> ("1.256.0.1")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.256.1")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.256")));
        EXPECT_FALSE ((accept<IP::V4> ("999.1.1.1")));
        EXPECT_FALSE ((accept<IP::V4> ("a.b.c.d")));
        EXPECT_FALSE ((accept<IP::V4> ("1.a.3.4")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.a.4")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.a")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.4a")));
        EXPECT_FALSE ((accept<IP::V4> ("00.0.0.0")));
        EXPECT_FALSE ((accept<IP::V4> ("0.00.0.0")));
        EXPECT_FALSE ((accept<IP::V4> ("0.0.00.0")));
        EXPECT_FALSE ((accept<IP::V4> ("0.0.0.00")));
        EXPECT_FALSE ((accept<IP::V4> ("1.2.3.4.")));
        EXPECT_FALSE ((accept<IP::V4> (".1.2.3.4")));
    }

    TEST (Parse, IPV6) {

        EXPECT_TRUE  ((accept<IP::V6> ("::")));
        EXPECT_TRUE  ((accept<IP::V6> ("::1")));
        EXPECT_TRUE  ((accept<IP::V6> ("::ffff:192.168.0.1")));
        EXPECT_TRUE  ((accept<IP::V6> ("0:0:0:0:0:ffff:192.168.0.1")));

        EXPECT_TRUE  ((accept<IP::V6> ("2001:db8::192.0.2.33")));
        EXPECT_TRUE  ((accept<IP::V6> ("::192.168.0.1")));
        EXPECT_TRUE  ((accept<IP::V6> ("::ffff:0:192.168.0.1")));

        EXPECT_TRUE  ((accept<IP::V6> ("1:2:3:4:5:6:192.168.0.1")));

        EXPECT_FALSE ((accept<IP::V6> ("1:2:3:4:5:6:7:192.168.0.1")));

        EXPECT_FALSE ((accept<IP::V6> ("1:2:3:4:5:192.168.0.1")));

        EXPECT_FALSE ((accept<IP::V6> ("::ffff:256.168.0.1")));
        EXPECT_FALSE ((accept<IP::V6> ("::ffff:192.168.0")));
        EXPECT_FALSE ((accept<IP::V6> ("::ffff:192.168.0.01")));

        EXPECT_FALSE ((accept<IP::V6> ("192.168.0.1::")));
        EXPECT_FALSE ((accept<IP::V6> ("1:2:192.168.0.1:3")));

        EXPECT_FALSE ((accept<IP::V6> ("::ffff::192.168.0.1")));

        EXPECT_FALSE ((accept<IP::V6> ("AB:02:3008:8CFD::02:3008:8CFD:02")));
        EXPECT_FALSE ((accept<IP::V6> (":0002:3008:8CFD:00AB:0002:3008:8CFD")));
    }

    TEST (Parse, JSON) {
        // ===== JSON::value Tests =====

        // ----- Literals -----
        EXPECT_TRUE  ( accept<JSON::value> ("null") );
        EXPECT_TRUE  ( accept<JSON::value> ("true") );
        EXPECT_TRUE  ( accept<JSON::value> ("false") );

        EXPECT_FALSE ( accept<JSON::value> ("nul") );
        EXPECT_FALSE ( accept<JSON::value> ("True") );
        EXPECT_FALSE ( accept<JSON::value> ("FALSE") );


        // ----- Numbers -----
        EXPECT_TRUE  ( accept<JSON::value> ("0") );
        EXPECT_TRUE  ( accept<JSON::value> ("-0") );
        EXPECT_TRUE  ( accept<JSON::value> ("123") );
        EXPECT_TRUE  ( accept<JSON::value> ("-123") );
        EXPECT_TRUE  ( accept<JSON::value> ("3.14") );
        EXPECT_TRUE  ( accept<JSON::value> ("-3.14") );
        EXPECT_TRUE  ( accept<JSON::value> ("1e10") );
        EXPECT_TRUE  ( accept<JSON::value> ("1E10") );
        EXPECT_TRUE  ( accept<JSON::value> ("-1e-10") );
        EXPECT_TRUE  ( accept<JSON::value> ("1.5e+8") );

        EXPECT_FALSE ( accept<JSON::value> ("01") );        // leading zero
        EXPECT_FALSE ( accept<JSON::value> ("--1") );
        EXPECT_FALSE ( accept<JSON::value> ("1.") );
        EXPECT_FALSE ( accept<JSON::value> (".5") );
        EXPECT_FALSE ( accept<JSON::value> ("1e") );
        EXPECT_FALSE ( accept<JSON::value> ("e10") );


        // ----- Strings -----
        EXPECT_TRUE  ( accept<JSON::value> ("\"\"") );
        EXPECT_TRUE  ( accept<JSON::value> ("\"hello\"") );
        EXPECT_TRUE  ( accept<JSON::value> ("\"hello world\"") );
        EXPECT_TRUE  ( accept<JSON::value> ("\"with \\\"escape\\\"\"") );
        EXPECT_TRUE  ( accept<JSON::value> ("\"line\\nbreak\"") );
        EXPECT_TRUE  ( accept<JSON::value> ("\"unicode \\u1234\"") );

        EXPECT_FALSE ( accept<JSON::value> ("\"unterminated") );
        EXPECT_FALSE ( accept<JSON::value> ("unterminated\"") );
        EXPECT_FALSE ( accept<JSON::value> ("\"bad \\escape\"") );
        EXPECT_FALSE ( accept<JSON::value> ("\"bad \\u12\"") );


        // ----- Arrays -----
        EXPECT_TRUE  ( accept<JSON::value> ("[]") );
        EXPECT_TRUE  ( accept<JSON::value> ("[1]") );
        EXPECT_TRUE  ( accept<JSON::value> ("[1,2,3]") );
        EXPECT_TRUE  ( accept<JSON::value> ("[true,false,null]") );
        EXPECT_TRUE  ( accept<JSON::value> ("[\"a\",\"b\",\"c\"]") );
        EXPECT_TRUE  ( accept<JSON::value> ("[[1,2],[3,4]]") );
        EXPECT_TRUE  ( accept<JSON::value> ("[{\"a\":1},{\"b\":2}]") );

        EXPECT_FALSE ( accept<JSON::value> ("[") );
        EXPECT_FALSE ( accept<JSON::value> ("]") );
        EXPECT_FALSE ( accept<JSON::value> ("[1,]") );      // trailing comma
        EXPECT_FALSE ( accept<JSON::value> ("[,1]") );
        EXPECT_FALSE ( accept<JSON::value> ("[1 2]") );


        // ----- Objects -----
        EXPECT_TRUE  ( accept<JSON::value> ("{}") );
        EXPECT_TRUE  ( accept<JSON::value> ("{\"a\":1}") );
        EXPECT_TRUE  ( accept<JSON::value> ("{\"a\":1,\"b\":2}") );
        EXPECT_TRUE  ( accept<JSON::value> ("{\"nested\":{\"x\":10}}") );
        EXPECT_TRUE  ( accept<JSON::value> ("{\"arr\":[1,2,3]}") );
        EXPECT_TRUE  ( accept<JSON::value> ("{\"mix\":{\"a\":[true,false,null]}}") );

        EXPECT_FALSE ( accept<JSON::value> ("{") );
        EXPECT_FALSE ( accept<JSON::value> ("}") );
        EXPECT_FALSE ( accept<JSON::value> ("{\"a\"}") );           // missing colon
        EXPECT_FALSE ( accept<JSON::value> ("{\"a\":}") );          // missing value
        EXPECT_FALSE ( accept<JSON::value> ("{a:1}") );             // key not string
        EXPECT_FALSE ( accept<JSON::value> ("{\"a\":1,}") );        // trailing comma
        EXPECT_FALSE ( accept<JSON::value> ("{,\"a\":1}") );


        // ----- Deep Recursion Smoke Tests -----
        EXPECT_TRUE  ( accept<JSON::value> (
            "{\"a\":[{\"b\":[{\"c\":[{\"d\":null}]}]}]}"
        ));

        EXPECT_TRUE  ( accept<JSON::value> (
            "[[[[[{\"x\":0}]]]]]"
        ));

        EXPECT_FALSE ( accept<JSON::value> (
            "{\"a\":[{\"b\":[{\"c\":[{\"d\":null}]}]}]"  // missing closing brace
        ));

    }

    TEST (Parse, XML) {
        // ===== XML::document Tests =====

        // ----- Minimal Documents -----
        EXPECT_TRUE  ( accept<XML::document> ("<a></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a/>") );

        EXPECT_FALSE ( accept<XML::document> ("") );
        EXPECT_FALSE ( accept<XML::document> ("<a>") );
        EXPECT_FALSE ( accept<XML::document> ("</a>") );


        // ----- Simple Text Content -----
        EXPECT_TRUE  ( accept<XML::document> ("<a>hello</a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a>123</a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a>hello world</a>") );

        EXPECT_FALSE ( accept<XML::document> ("<a><</a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a>></a>") );


        // ----- Nested Elements -----
        EXPECT_TRUE  ( accept<XML::document> ("<a><b></b></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><b><c></c></b></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<root><a/><b/><c/></root>") );


        // ----- Mismatched Tags (Allowed by recognizer) -----
        // Since we are NOT verifying matching names, these should pass
        EXPECT_TRUE  ( accept<XML::document> ("<a></b>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><b></c></d>") );


        // ----- Attributes -----
        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"1\"></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x='1'></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x='\"'></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"'\"></a>") );

        EXPECT_FALSE ( accept<XML::document> ("<a x=\"\"\"></a>") );
        EXPECT_FALSE ( accept<XML::document> ("<a x='''></a>") );
        EXPECT_FALSE ( accept<XML::document> ("<a x=\"<\"></a>") );
        EXPECT_FALSE ( accept<XML::document> ("<a x=\"&\"></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"&amp;\"></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"1\" y='2'></a>") );

        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"1\" y=\"2\"></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"value\"/>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a x=\"1\"><b y=\"2\"/></a>") );

        EXPECT_FALSE ( accept<XML::document> ("<a x=1></a>") );          // missing quotes
        EXPECT_FALSE ( accept<XML::document> ("<a x=\"1></a>") );        // unterminated string
        EXPECT_FALSE ( accept<XML::document> ("<a x=></a>") );           // missing value


        // ----- XML Declaration (if supported) -----
        EXPECT_TRUE  ( accept<XML::document> ("<?xml version=\"1.0\"?><a/>") );
        EXPECT_TRUE  ( accept<XML::document> ("<?xml version=\"1.0\" encoding=\"UTF-8\"?><a></a>") );


        // ----- Mixed Content -----
        EXPECT_TRUE  ( accept<XML::document> ("<a>text<b/>more</a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><b/>text<c/></a>") );


        // ----- Deep Nesting -----
        EXPECT_TRUE  ( accept<XML::document> (
            "<a><b><c><d><e></e></d></c></b></a>"
        ));

        EXPECT_TRUE  ( accept<XML::document> (
            "<root><a><b/><c><d/><e/></c></a></root>"
        ));


        // ----- Structural Failures -----
        EXPECT_FALSE ( accept<XML::document> ("<a/><b/>") );     // multiple roots (if you enforce single root)
        EXPECT_FALSE ( accept<XML::document> ("<a></a></b>") );

        EXPECT_FALSE ( accept<XML::document> ("<a><b></b>") );    // missing closing outer
        EXPECT_FALSE ( accept<XML::document> ("<a/></") );        // incomplete tag
        EXPECT_FALSE ( accept<XML::document> ("<>") );
        EXPECT_FALSE ( accept<XML::document> ("<a x=\"1\"") );    // incomplete start tag

        // =========================
        // XML Declaration
        // =========================

        EXPECT_TRUE  ( accept<XML::document> ("<?xml version=\"1.0\"?><a></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<?xml version=\"1.0\" encoding=\"UTF-8\"?><a></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>   <a></a>") );

        EXPECT_FALSE ( accept<XML::document> ("<?xml?><a></a>") );                     // missing required attributes
        EXPECT_FALSE ( accept<XML::document> ("<?xml version=\"1.0\"><a></a>") );      // missing ?>
        EXPECT_FALSE ( accept<XML::document> ("<?xml version=\"1.0\"?>") );            // no root element

        // ----- Comments -----
        EXPECT_TRUE  ( accept<XML::document> ("<!-- comment --><a/>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><!-- inside --></a>") );

        EXPECT_FALSE ( accept<XML::document> ("<!-- unclosed <a/>") );
        EXPECT_FALSE ( accept<XML::document> ("<!-- bad -- comment --><a/>") );

        // =========================
        // Comments – standalone
        // =========================

        EXPECT_TRUE  ( accept<XML::document> ("<!-- comment --><a></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a></a><!-- comment -->") );
        EXPECT_TRUE  ( accept<XML::document> ("<!-- one --><!-- two --><a></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<!-- one --><a></a><!-- two -->") );


        // =========================
        // Comments inside content
        // =========================

        EXPECT_TRUE  ( accept<XML::document> ("<a><!-- comment --></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a>text<!-- comment --></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><!-- one --><!-- two --></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a>text<!-- one -->more<!-- two --></a>") );


        // =========================
        // Invalid comment forms
        // =========================

        EXPECT_FALSE ( accept<XML::document> ("<a><!-- bad -- comment --></a>") );     // contains "--"
        EXPECT_FALSE ( accept<XML::document> ("<a><!-- bad ---></a>") );               // malformed ending
        EXPECT_FALSE ( accept<XML::document> ("<a><!--></a>") );                       // unclosed
        EXPECT_FALSE ( accept<XML::document> ("<a><!-----></a>") );                    // invalid "--" sequence



        // =========================
        // Raw '&' in text (should be invalid XML)
        // =========================

        EXPECT_FALSE ( accept<XML::document> ("<a>&</a>") );
        EXPECT_FALSE ( accept<XML::document> ("<a>Tom & Jerry</a>") );


        // =========================
        // Entity references
        // =========================

        EXPECT_TRUE ( accept<XML::document> ("<a>&amp;</a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a>&lt;</a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a>&gt;</a>") );


        // =========================
        // Numeric character references
        // =========================

        EXPECT_TRUE ( accept<XML::document> ("<a>&#65;</a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a>&#x41;</a>") );

        // =========================
        // CDATA sections
        // =========================

        EXPECT_TRUE  ( accept<XML::document> ("<a><![CDATA[ raw <xml> ]]></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><![CDATA[test]]></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><![CDATA[<b>&stuff</b>]]></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><![CDATA[<b>&stuff</b>]]></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><![CDATA[]]]x]]></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><![CDATA[]]]x]]></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><![CDATA[]]]x]]></a>") );

        EXPECT_FALSE ( accept<XML::document> ("<a><![CDATA[test]]>oops]]></a>") );
        EXPECT_FALSE ( accept<XML::document> ("<a><![CDATA[test]</a>") );
        EXPECT_FALSE ( accept<XML::document> ("<a><![cdata[test]]></a>") );

        // =========================
        // Processing instructions
        // =========================

        EXPECT_TRUE ( accept<XML::document> ("<a><?pi?></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><?pi test?></a>") );
        EXPECT_TRUE ( accept<XML::document> ("<a><?pi   test?></a>") );

        EXPECT_FALSE ( accept<XML::document> ("<a><?xml test?></a>") );   // forbidden target
        EXPECT_TRUE  ( accept<XML::document> ("<a><?xmltest?></a>") );   // ok
        EXPECT_FALSE ( accept<XML::document> ("<a><?pi test></a>") );     // missing ?>

        // =========================
        // Mismatched tags
        // =========================
        // NOTE our recognizer cannot reject these.
        //EXPECT_FALSE ( accept<XML::document> ("<a></b>") );
        //EXPECT_FALSE ( accept<XML::document> ("<a><b></a></b>") );

        // =========================
        // Multiple root elements (should fail)
        // =========================

        EXPECT_FALSE ( accept<XML::document> ("<a></a><b></b>") );


        // =========================
        // Edge whitespace cases
        // =========================

        EXPECT_TRUE  ( accept<XML::document> ("   <a></a>   ") );
        EXPECT_TRUE  ( accept<XML::document> ("\n\t<a></a>\r") );


        // =========================
        // Nested elements
        // =========================

        EXPECT_TRUE  ( accept<XML::document> ("<a><b></b></a>") );
        EXPECT_TRUE  ( accept<XML::document> ("<a><b><c></c></b></a>") );



    }

    TEST (Parse, URL) {

    }

}
