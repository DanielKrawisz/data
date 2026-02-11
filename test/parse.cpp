// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools/parse.hpp"
#include "data/encoding/hex.hpp"
#include "gtest/gtest.h"
#include <regex>

namespace data::parse {

    TEST (Parse, Basics) {

        using namespace machine;

        EXPECT_FALSE (accept<invalid> (""));
        EXPECT_FALSE (accept<invalid> ("a"));
        EXPECT_FALSE (accept<invalid> ("anything"));

        EXPECT_TRUE  (accept<any> (""));
        EXPECT_TRUE  (accept<any> ("a"));
        EXPECT_TRUE  (accept<any> ("abc"));
        EXPECT_TRUE  (accept<any> (" "));

        EXPECT_TRUE  (accept<exactly<>> (""));
        EXPECT_FALSE (accept<exactly<>> ("a"));

        EXPECT_FALSE (accept<exactly<'a'>> (""));
        EXPECT_TRUE  (accept<exactly<'a'>> ("a"));
        EXPECT_FALSE (accept<exactly<'a'>> ("aa"));
        EXPECT_FALSE (accept<exactly<'a'>> ("b"));

        EXPECT_TRUE  ((accept<max_size<any, 0>> ("")));
        EXPECT_FALSE ((accept<max_size<any, 0>> ("a")));

        EXPECT_TRUE  ((accept<max_size<any, 3>> ("")));
        EXPECT_TRUE  ((accept<max_size<any, 3>> ("a")));
        EXPECT_TRUE  ((accept<max_size<any, 3>> ("abc")));
        EXPECT_FALSE ((accept<max_size<any, 3>> ("abcd")));
    }

    TEST (Parse, Optional) {

        using namespace machine;

        EXPECT_TRUE  (accept<optional<invalid>> (""));
        EXPECT_FALSE (accept<optional<invalid>> (" "));
        EXPECT_FALSE (accept<optional<invalid>> ("a"));

        EXPECT_TRUE  (accept<optional<any>> (""));
        EXPECT_TRUE  (accept<optional<any>> ("a"));
        EXPECT_TRUE  (accept<optional<any>> ("abc"));

        using opt_a = optional<exactly<'a'>>;

        EXPECT_TRUE  (accept<opt_a> (""));      // zero case
        EXPECT_TRUE  (accept<opt_a> ("a"));     // one case
        EXPECT_FALSE (accept<opt_a> ("aa"));    // too many
        EXPECT_FALSE (accept<opt_a> ("b"));     // invalid char

        EXPECT_FALSE (accept<opt_a> ("ab"));
        EXPECT_FALSE (accept<opt_a> ("ba"));

        using opt_opt_a = optional<optional<exactly<'a'>>>;

        EXPECT_TRUE  (accept<opt_opt_a> (""));
        EXPECT_TRUE  (accept<opt_opt_a> ("a"));
        EXPECT_FALSE (accept<opt_opt_a> ("aa"));
    }

    TEST (Parse, Sequence) {

        using namespace machine;

        EXPECT_TRUE  ((accept<sequence<any, any>> ("")));
        EXPECT_TRUE  ((accept<sequence<any, any>> ("a")));
        EXPECT_TRUE  ((accept<sequence<any, any>> ("abc")));

        EXPECT_FALSE ((accept<sequence<exactly<'a'>, exactly<'b'>>> ("")));
        EXPECT_FALSE ((accept<sequence<exactly<'a'>, exactly<'b'>>> ("a")));
        EXPECT_TRUE  ((accept<sequence<exactly<'a'>, exactly<'b'>>> ("ab")));
        EXPECT_FALSE ((accept<sequence<exactly<'a'>, exactly<'b'>>> ("abc")));
        EXPECT_FALSE ((accept<sequence<exactly<'a'>, exactly<'b'>>> ("b")));

        using P = sequence<exactly<>, exactly<'a'>>;

        EXPECT_FALSE ((accept<P> ("")));
        EXPECT_TRUE  ((accept<P> ("a")));
        EXPECT_FALSE ((accept<P> ("aa")));

        using seq_ab_c = sequence<
            exactly<'a','b'>,
            exactly<'c'>
        >;

        EXPECT_FALSE (accept<seq_ab_c> (""));
        EXPECT_TRUE  (accept<seq_ab_c> ("abc"));
        EXPECT_FALSE (accept<seq_ab_c> ("ab"));
        EXPECT_FALSE (accept<seq_ab_c> ("a"));
        EXPECT_FALSE (accept<seq_ab_c> ("abcd"));
        EXPECT_FALSE (accept<seq_ab_c> ("xbc"));
        EXPECT_FALSE (accept<seq_ab_c> ("abd"));

        using seq_a_bc = sequence<
            exactly<'a'>,
            exactly<'b','c'>
        >;

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

    TEST (Parse, Alternatives) {

        using namespace machine;

        EXPECT_FALSE ((accept<alternatives<invalid, invalid>> ("")));
        EXPECT_FALSE ((accept<alternatives<invalid, invalid>> ("a")));

        EXPECT_FALSE ((accept<alternatives<exactly<'a'>, exactly<'b'>>> ("")));
        EXPECT_TRUE  ((accept<alternatives<exactly<'a'>, exactly<'b'>>> ("a")));
        EXPECT_TRUE  ((accept<alternatives<exactly<'a'>, exactly<'b'>>> ("b")));
        EXPECT_FALSE ((accept<alternatives<exactly<'a'>, exactly<'b'>>> ("ab")));
        EXPECT_FALSE ((accept<alternatives<exactly<'a'>, exactly<'b'>>> ("c")));

        using O = alternatives<
            exactly<'a','b','c'>,
            exactly<'a','b','d'>
        >;

        EXPECT_TRUE  (accept<O> ("abd"));
        EXPECT_TRUE  (accept<O> ("abc"));
        EXPECT_FALSE (accept<O> ("abe"));
/*
        using P = sequence<
            alternatives<
                exactly<'a','b','c'>,
                exactly<'a','b'>
            >,
            exactly<'d'>
        >;

        EXPECT_TRUE  (accept<P> ("abcd"));
        EXPECT_TRUE  (accept<P> ("abd"));
        EXPECT_FALSE (accept<P> ("abc"));*/
    }

    TEST (Parse, Repeated) {

        using namespace machine;

        EXPECT_TRUE  (accept<star<any>> (""));
        EXPECT_TRUE  (accept<star<any>> ("a"));
        EXPECT_TRUE  (accept<star<any>> ("abc"));

        EXPECT_TRUE  (accept<star<exactly<'a'>>> (""));
        EXPECT_TRUE  (accept<star<exactly<'a'>>> ("a"));
        EXPECT_TRUE  (accept<star<exactly<'a'>>> ("aaaa"));
        EXPECT_FALSE (accept<star<exactly<'a'>>> ("b"));
        EXPECT_FALSE (accept<star<exactly<'a'>>> ("aaab"));
        EXPECT_FALSE (accept<star<exactly<'a'>>> ("baaa"));

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
/*
        using R = repeated<repeated<exactly<'a'>>, 2>;

        EXPECT_TRUE (accept<R> (""));
        EXPECT_TRUE (accept<R> ("a"));
        EXPECT_TRUE (accept<R> ("aaaa"));*/
    }

    TEST (Parse, Backtrack) {

        using namespace machine;

        using M = sequence<
            repeated<exactly<'a','b','c','d'>>,
            exactly<'a','b','d'>>;

        EXPECT_TRUE  (accept<M> ("abcdabcdabcdabd"));
        EXPECT_FALSE (accept<M> ("abcdabcdabcdab"));
        EXPECT_FALSE (accept<M> ("abcdabcdabcdabx"));
/*
        using N = sequence<
            repeated<exactly<'a','b'>>,
            exactly<'a'>>;

        EXPECT_TRUE  (accept<N> ("aba"));
        EXPECT_TRUE  (accept<N> ("ababa"));
        EXPECT_FALSE (accept<N> ("abab"));
*/
        using Q = sequence<
            repeated<exactly<'a','b'>>,
            exactly<'b'>
        >;

        EXPECT_TRUE  (accept<Q> ("abb"));
        EXPECT_TRUE  (accept<Q> ("abababb"));
        EXPECT_FALSE (accept<Q> ("ababab"));
/*
        using R = sequence<
            repeated<exactly<'a','a'>>,
            exactly<'a'>
        >;

        EXPECT_TRUE  (accept<R> ("aaa"));
        EXPECT_TRUE  (accept<R> ("aaaaa"));
        EXPECT_FALSE (accept<R> (""));
        EXPECT_FALSE (accept<R> ("aa"));*/

    }
}
