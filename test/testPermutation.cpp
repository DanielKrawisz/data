// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math.hpp"
#include "data/math/algebra/cyclic_group.hpp"
#include "gtest/gtest.h"

namespace data {
    
    TEST (PermutationTest, TestCycle) {
        using ucycle = cycle<uint32>;
        
        ucycle c {};
        ucycle c1 {1};
        ucycle c0 {0};
        ucycle c11 {1, 1};
        
        EXPECT_EQ (c, c);
        EXPECT_EQ (c1, c1);
        EXPECT_EQ (c11, c11);
        EXPECT_NE (c1, c11);
        EXPECT_NE (c0, c1);
        
        ucycle c12 {1, 2};
        ucycle c21 {2, 1};
        
        EXPECT_EQ (c12.head (), 1);
        EXPECT_EQ (c21.head (), 2);
        
        EXPECT_EQ (c12, c12);
        EXPECT_EQ (c12, c21);
        
        EXPECT_EQ (c12.reverse (), c12);
        EXPECT_EQ (c12.reverse (), c21);
        EXPECT_EQ (c21.reverse (), c12);
        EXPECT_EQ (c21.reverse (), c21);
        
        ucycle c123 {1, 2, 3};
        ucycle c231 {2, 3, 1};
        ucycle c312 {3, 1, 2};
        ucycle c321 {3, 2, 1};
        ucycle c213 {2, 1, 3};
        ucycle c132 {1, 3, 2};
        
        EXPECT_EQ (c123, c123);
        EXPECT_EQ (c123, c231);
        EXPECT_EQ (c123, c312);
        EXPECT_EQ (c321, c132);
        EXPECT_NE (c123, c132);
        EXPECT_NE (c321, c231);
        
        EXPECT_EQ (c123.reverse(), c321);
        EXPECT_EQ (c321.reverse(), c123);
        
    }
    
    TEST (PermutationTest, TestPermutation) {
        using perm = permutation<uint32>;
        
        perm invalid1 {{1, 2, 1}};
        perm invalid2 {{1, 2}, {3, 2}};
        
        EXPECT_FALSE (valid (invalid1));
        EXPECT_TRUE (valid (perm {{}, {1}}));
        EXPECT_FALSE (valid (invalid2));
        
        EXPECT_EQ (sign (invalid1), math::zero);
        EXPECT_EQ (sign (invalid2), math::zero);
        
        perm p0 {};
        perm p0_1 {{}};
        perm p0_2 {{}, {}};
        perm p0_3 {{1}};
        perm p0_4 {{1, 1}};
        
        EXPECT_TRUE (valid (p0));
        EXPECT_TRUE (valid (p0_1));
        EXPECT_TRUE (valid (p0_2));
        EXPECT_TRUE (valid (p0_3));
        EXPECT_TRUE (valid (p0_4));
        
        EXPECT_EQ (p0, p0_1);
        EXPECT_EQ (p0, p0_2);
        EXPECT_EQ (p0, p0_3);
        EXPECT_EQ (p0, p0_4);
        
        EXPECT_EQ (p0, p0.inverse ());
        EXPECT_EQ (p0, p0 * p0);
        
        EXPECT_EQ (sign (p0), math::positive);
        
        perm p12 = perm {{1, 2}};
        perm p21 = perm {{2, 1}};
        perm p23 = perm {{2, 3}};
        perm p13 = perm {{1, 3}};
        
        EXPECT_TRUE (valid (p21));
        EXPECT_TRUE (valid (p12));
        EXPECT_TRUE (valid (p13));
        EXPECT_TRUE (valid (p23));
        
        EXPECT_EQ (p0, p12 * p21);
        EXPECT_EQ (p0, p12 * p12);
        EXPECT_EQ (p12, p21);
        
        EXPECT_EQ (sign (p12), math::negative);
        EXPECT_EQ (sign (p23), math::negative);
        EXPECT_EQ (sign (p13), math::negative);
        
        perm p123 = perm {{1, 2, 3}};
        perm p321 = perm {{3, 2, 1}};
        
        EXPECT_TRUE (valid (p123));
        EXPECT_TRUE (valid (p321));
        
        EXPECT_EQ (p0, p123 * p321);
        EXPECT_EQ (p321, p123 * p123);
        EXPECT_EQ (p0, p123 * p123 * p123);
        EXPECT_EQ (p123, p23 * p12);
        EXPECT_EQ (p321, p12 * p23);
        EXPECT_EQ (p321, p13 * p12);
        EXPECT_EQ (p123, p12 * p13);
        
        EXPECT_EQ (sign (p123), math::positive);
        EXPECT_EQ (sign (p321), math::positive);
        
        perm p1234a = perm {{1, 2, 3, 4}};
        perm p1234b = perm {{1, 2}, {3, 4}};
        
        EXPECT_TRUE (valid (p1234a));
        EXPECT_TRUE (valid (p1234b));
        
        EXPECT_EQ (sign (p1234a), math::negative);
        EXPECT_EQ (sign (p1234b), math::positive);
    }
/*
    namespace TinyBang {

        // solving a puzzle from Tiny Bang Story
        constexpr auto d5 = decimal{"5"};
        constexpr auto d6 = decimal{"6"};

        using cycle5 = permutation<uint32>;
        using cycle6 = permutation<uint32>;

        struct puzzle {
            cycle5 Edges;
            cycle6 Vertices;

            static puzzle &goal () {
                static puzzle Goal {{{1, 4, 3, 2}}, {{1, 3, 2, 6, 5}}};
                return Goal;
            }

            static puzzle &identity () {
                static puzzle Identity {{}, {}};
                return Identity;
            }

            static puzzle &rotate_left_circle () {
                static puzzle RotateLeftCircle {{{1, 2}}, {{1, 5}, {2, 4}}};
                return RotateLeftCircle;
            }

            static puzzle &rotate_right_circle () {
                static puzzle RotateRightCircle {{{2, 3, 4, 5}}, {{2, 3, 6, 5}}};
                return RotateRightCircle;
            }

            puzzle operator * (const puzzle &x) const {
                return puzzle {Edges * x.Edges, Vertices * x.Vertices};
            }

            bool operator == (const puzzle &x) const {
                return Edges == x.Edges && Vertices == x.Vertices;
            }

        };

        std::ostream inline &operator << (std::ostream &o, const puzzle &p) {
            return o << "[" << p.Edges.Cycles << ", " << p.Vertices.Cycles << "]";
        }

    }

    uint32 order (const TinyBang::puzzle &p) {
        auto x = p;
        auto e = TinyBang::puzzle::identity ();

        if (p == e) return 0;

        uint32 o = 1;

        while (x != e) {
            o++;
            x = x * p;
        }

        return o;
    }

    TEST (PermutationTest, TestTinyBang) {

        auto e = TinyBang::puzzle::identity ();
        auto goal = TinyBang::puzzle::goal ();
        auto a = TinyBang::puzzle::rotate_left_circle ();
        auto b = TinyBang::puzzle::rotate_right_circle ();

        auto a2 = a * a;
        auto c = b * b;
        auto d = c * b;
        auto b4 = c * c;

        EXPECT_EQ (e, a2);
        EXPECT_EQ (e, b4);
        EXPECT_EQ (e, b * d);

        auto ab = a * b;
        auto ba = b * a;
        auto ac = a * c;
        auto ca = c * a;
        auto ad = a * d;
        auto da = d * a;

        EXPECT_EQ (e, ab * da);
        EXPECT_EQ (e, ba * ad);
        EXPECT_EQ (e, ca * ac);
        EXPECT_EQ (e, ac * ca);

        struct operation {
            list<char> Expression;

            TinyBang::puzzle Transformation;

            bool operator == (const operation &o) const {
                return Transformation == o.Transformation;
            }

            operation operator * (const operation &o) const {
                return operation {data::join (Expression, o.Expression), Transformation * o.Transformation};
            }
        };

        list<operation> moves {};
        list<operation> atoms {operation {{'a'}, a}, operation {{'b'}, b}, operation {{'c'}, c}, operation {{'d'}, d}};
        list<operation> next = atoms;

        std::cout << "about to generate all possible moves until a solution is found. " << std::endl;

        while (true) {

            list<operation> last {};

            for (const operation &op : next) {
                for (const operation &m : moves) {
                    if (op.Transformation == m.Transformation) goto already_generated;
                }

                moves = moves << op;
                last = last << op;

                if (op.Transformation == goal) {
                    std::cout << "solution found: " << op.Expression << std::endl;

                    goto solution_found;
                }

                already_generated:;
            }

            std::cout << "generated " << moves.size () << " moves." << std::endl;

            next = list<operation> {};

            for (const operation &x : atoms) {
                for (const operation &y : last) {
                    next = next << (y * x);
                }
            }

        }

        solution_found:;

    }*/

    /*
    constexpr auto d1 = decimal{"1"};
    constexpr auto d2 = decimal{"2"};
    constexpr auto d3 = decimal{"3"};
    constexpr auto d4 = decimal{"4"};
    constexpr auto d5 = decimal{"5"};
    constexpr auto d6 = decimal{"6"};
    constexpr auto d7 = decimal{"7"};
    constexpr auto d8 = decimal{"8"};
    constexpr auto d9 = decimal{"9"};
    
    template <typename N, auto & natural>
    using symmetric = data::math::algebra::symmetric_group<N, natural>;
    
    template <typename N, auto & natural>
    using alternating = data::math::algebra::alternating_group<N, natural>;
    
    TEST(PermutationTest, TestSymmetric) {
        symmetric<uint32, d1>{};
        symmetric<uint32, d2>{};
        symmetric<uint32, d3>{};
        symmetric<uint32, d4>{};
        symmetric<uint32, d5>{};
        symmetric<uint32, d6>{};
        symmetric<uint32, d7>{};
        symmetric<uint32, d8>{};
        symmetric<uint32, d9>{};
    }
    
    TEST(PermutationTest, TestAlternating) {
        alternating<uint32, d1>{};
        alternating<uint32, d3>{};
        alternating<uint32, d5>{};
        alternating<uint32, d7>{};
        alternating<uint32, d9>{};
    }*/
    
}
