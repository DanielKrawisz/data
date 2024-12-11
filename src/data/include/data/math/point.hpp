// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/types.hpp>
#include <data/math/linear/space.hpp>
#include <data/math/algebra/algebra.hpp>

namespace data::math {

    template <skew_field X, size_t A, size_t B> using matrix = array<X, A, B>;

    template <skew_field X, size_t A, size_t B> X det (const matrix<X, A, B> &);
    template <skew_field X, size_t A> X det (const matrix<X, A, A> &);

};

namespace data::math::space {

    template <skew_field X, size_t order, size_t dim> struct exterior;

    template <skew_field X, size_t dim> using scalar = exterior<X, 0, dim>;
    template <skew_field X, size_t dim> using vector = exterior<X, 1, dim>;

    template <skew_field X, size_t A, size_t B, size_t dim>
    exterior <X, A + B, dim> operator ^ (const exterior<X, A, dim> &, const exterior<X, B, dim> &);

    template <skew_field X, size_t order, size_t dim>
    exterior <X, dim - order, dim> operator * (const exterior<X, order, dim> &);

    template <skew_field X, size_t order, size_t dim>
    X operator * (const exterior<X, order, dim> &, const exterior<X, dim - order, dim> &);

    template <ordered Q, skew_field X, size_t order, size_t dim> requires requires (const X &x, const X &y) {
        {x * inner (x, y)} -> implicitly_convertible_to<Q>;
    } Q operator * (const exterior<X, order, dim> &a, const exterior<X, order, dim> &b);

    // scalar multiplication
    template <skew_field X, size_t u, size_t dim>
    exterior <X, u, dim> operator * (const exterior<X, u, dim> &, const X &);

    template <skew_field X, size_t u, size_t dim>
    exterior <X, u, dim> operator + (const exterior<X, u, dim> &, const exterior<X, u, dim> &);

    // affine geometry is like Euclid without circles.
    template <skew_field X, size_t dim> struct affine {
        template <size_t order> struct simplex;

        using point = simplex<0>;
        using line = simplex<1>;
        using plane = simplex<2>;
        using space = simplex<3>;

        // whether a line contains a point, etc.
        template <size_t a, size_t b>
        static bool contains (const simplex<a> &, const simplex<b> &);

        // connect two points to make a line, etc.
        template <size_t a, size_t b>
        static simplex<a + b> connect (const simplex<a> &, const simplex<b> &);

        // we represent objects as exterior algebraic objects of one higher dimension.
        template <size_t order> struct simplex : exterior<X, order + 1, dim + 1> {
            // valid if a valid exterior object and nonzero.
            bool valid () const;

            simplex operator + (const exterior<X, order, dim> &v) const;

            exterior<X, order, dim> operator - (const simplex &p) const;
        };

        struct transformation : matrix<X, dim + 1, dim + 1> {
            transformation (const simplex<dim> &from, const simplex<dim> &to);

            bool valid () const;

            transformation operator * (const transformation &) const;

            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    template <skew_field X, ordered Q, size_t dim> requires normed<X, Q>
    struct Euclidian : affine<X, dim> {
        template <size_t order> using simplex = affine<X, dim>::simplex;

        using point = affine<X, dim>::point;
        using line = affine<X, dim>::line;
        using plane = affine<X, dim>::plane;
        using space = affine<X, dim>::space;

        // distance squared
        Q quadrance (const point &, const point &);

        struct transformation : affine<X, dim>::transformation {
            bool valid () const;

            transformation operator * (const transformation &) const;
        };

        static transformation translation (const vector<X, dim> &);
        static transformation scale (const X &);
        static transformation flip (const vector<X, dim> &);
        // from two flips we can get a rotation. We do not have a native
        // function for rotations because you cannot do an arbitrary
        // rotation, depending on the field.
    };

    template <skew_field X, ordered Q, size_t dim> requires normed<X, Q>
    struct elliptic {
        template <size_t order>
        struct simplex : exterior<X, order + 1, dim + 1> {
            bool valid () const;
        };

        using point = simplex<1>;
        using line = simplex<2>;
        using plane = simplex<3>;
        using space = simplex<4>;

        template <size_t a, size_t b>
        static bool contains (const simplex<a> &, const simplex<b> &);

        struct transformation : simplex<2> {
            // must be orthogonal.
            bool valid () const;

            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    template <skew_field X, size_t dim> struct projective {
        template <size_t order>
        struct exterior : space::exterior<X, order, dim> {
            bool valid () const;

            bool operator == (const exterior &x) const;
        };

        using value = unsigned_limit<X>;

        template <size_t order>
        struct simplex : space::exterior<value, order + 1, dim + 1> {

            bool valid () const;

            bool operator == (const simplex &x) const;
        };

        using point = simplex<1>;
        using line = simplex<2>;
        using plane = simplex<3>;
        using space = simplex<4>;

        struct transformation : exterior<2> {
            transformation (const simplex<dim> &from, const simplex<dim> &to);
            bool valid () const;
            template <size_t order>
            transformation operator * (const exterior<order> &) const;
            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    namespace {
        // Primary template
        template <typename X, size_t u, size_t N, size_t... Args>
        struct box_array : box_array<X, u - 1, N, N, Args...> {};

        // Specialization to end recursion
        template <typename X, size_t N, size_t... Args>
        struct box_array<X, 0, N, Args...> {
            using type = array<X, Args...>;
        };
    }

    template <skew_field X, size_t order, size_t dim> struct exterior : box_array<X, order, dim>::type {
        using array = box_array<X, order, dim>::type;
        exterior (array &&);
        exterior (const array &);
        // it's valid if the symmetry properties are correct.
        bool valid ();
    };
    
}

#endif

