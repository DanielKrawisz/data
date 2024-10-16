// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/types.hpp>
#include <data/math/linear/space.hpp>
#include <data/math/algebra/algebra.hpp>

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
    
    template <skew_field X, size_t A, size_t B> using matrix = array<X, A, B>;

    // affine geometry is like Euclid without circles.
    // we need a norm.
    template <skew_field X, size_t dim> struct affine {
        template <size_t order>
        struct simplex : exterior<X, order + 1, dim + 1> {
            bool valid () const;

            simplex operator + (const exterior<X, order, dim> &v) const;

            exterior<X, order, dim> operator - (const simplex &p) const;
        };

        using point = simplex<1>;
        using line = simplex<2>;
        using plane = simplex<3>;
        using space = simplex<4>;

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
        using point = affine<X, dim>::point;
        using line = affine<X, dim>::line;
        using plane = affine<X, dim>::plane;
        using space = affine<X, dim>::space;

        struct transformation : affine<X, dim>::transformation {

            bool valid () const;

            transformation operator * (const transformation &) const;
        };

        static transformation translation (const vector<X, dim> &);
        static transformation scale (const X &);
        static transformation flip (const vector<X, dim> &);
    };

    template <skew_field X, size_t dim> struct elliptic {
        template <size_t order>
        struct simplex : exterior<X, order, dim + 1> {
            bool valid () const;
        };

        using point = simplex<1>;
        using line = simplex<2>;
        using plane = simplex<3>;
        using space = simplex<4>;

        struct transformation : simplex<2> {
            bool valid () const;

            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    template <skew_field X, size_t dim> struct projective {
        template <size_t order>
        struct simplex : exterior<X, order + 1, dim + 1> {
            bool valid () const;

            bool operator == (const simplex &x) const;
        };

        using point = simplex<1>;
        using line = simplex<2>;
        using plane = simplex<3>;
        using space = simplex<4>;

        struct transformation : matrix<X, dim + 1, dim + 1> {
            transformation (const simplex<dim> &from, const simplex<dim> &to);
            bool valid () const;
            transformation operator * (const transformation &) const;
            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    template <skew_field X, size_t dim> struct projective_point : array<unsigned_limit<X>, dim> {
        bool valid () const;
    };

    template <skew_field X, size_t dim>
    template <size_t order>
    bool inline projective<X, dim>::simplex<order>::valid () const {
        return static_cast<exterior<X, order, dim + 1>> (*this) != exterior<X, order, dim + 1> {};
    }

    template <skew_field X, size_t dim>
    template <size_t order>
    bool projective<X, dim>::simplex<order>::operator == (const simplex &x) const {
        X ratio {};

        for (int i = 0; i < dim + 1; i++) {
            const X &left = (*this)[i];
            const X &right = x[i];

            if (left == X {} || right == X {}) {
                if (left != right) return false;
            } else if (ratio == X {}) ratio = left / right;
            else if (ratio != left / right) return false;
        }

        return true;
    }

    template <skew_field X, size_t dim>
    bool projective_point<X, dim>::valid () const {
        bool finite = false;

        for (const X &x : *this) {
            if (!data::valid (x)) return false;
            if (x != X::infinity ()) {
                if (finite) return false;
                finite = true;
            }
        }

        return true;
    }
    
}

#endif

