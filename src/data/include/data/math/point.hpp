// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/types.hpp>
#include <data/math/linear/space.hpp>
#include <data/math/algebra/algebra.hpp>

namespace data::math::linear {

    template <skew_field X, size_t A, size_t B> using matrix = array<X, A, B>;

    template <skew_field X, size_t A, size_t B> X det (const matrix<X, A, B> &);
    template <skew_field X, size_t A> X det (const matrix<X, A, A> &);

    template <skew_field X, size_t A, size_t B> bool invertable (const matrix<X, A, B> &);
    template <skew_field X, size_t A> bool invertable (const matrix<X, A, A> &);
    template <skew_field X, size_t A> matrix<X, A, A> inverse (const matrix<X, A, A> &);

};

namespace data::math::space {

    // the exterior algebra comprises scalars, vectors, asymmetric 2-tensors, etc.
    template <skew_field X, size_t order, size_t dim> struct exterior;

    template <skew_field X, size_t dim> using scalar = exterior<X, 0, dim>;
    template <skew_field X, size_t dim> using vector = exterior<X, 1, dim>;
    // nonstandard terminology but it helps to remember what these things are.
    template <skew_field X, size_t dim> using planar = exterior<X, 2, dim>;
    template <skew_field X, size_t dim> using spacer = exterior<X, 3, dim>;

    // scalar multiplication
    template <skew_field X, size_t u, size_t dim>
    exterior <X, u, dim> operator * (const exterior<X, u, dim> &, const X &);

    // addition
    template <skew_field X, size_t u, size_t dim>
    exterior <X, u, dim> operator + (const exterior<X, u, dim> &, const exterior<X, u, dim> &);

    template <skew_field X, size_t A, size_t B, size_t dim>
    exterior <X, A + B, dim> operator ^ (const exterior<X, A, dim> &, const exterior<X, B, dim> &);

    template <skew_field X, size_t order, size_t dim>
    X operator * (const exterior<X, order, dim> &, const exterior<X, dim - order, dim> &);

    // generate a matrix that projects onto the subspace defined by the exterior object.
    // the second exterior object is left unchanged.
    template <skew_field X, size_t order, size_t dim>
    linear::matrix<X, dim, dim> projector (const exterior<X, order, dim> &a, const exterior<X, dim - order, dim> &b);

    template <skew_field X, size_t order, size_t dim> requires requires (const X &x, const X &y) {
        {x * inner (x, y)};
    } auto operator * (const exterior<X, order, dim> &a, const exterior<X, order, dim> &b);

    // Hodge star
    template <skew_field X, size_t order, size_t dim>
    requires requires (const X &x, const X &y) {
        {x * inner (x, y)};
    } exterior<X, dim - order, dim> operator * (const exterior<X, order, dim> &);

    // affine geometry is like Euclid without circles.
    // technically, we do not have the full exterior algebra
    // available because we don't have the Hodge star without
    // a notion of perpendicularity.
    template <skew_field X, size_t dim> struct affine {
        // technically, this is an incorrect use of the term.
        template <size_t order> struct simplex;

        using point = simplex<0>;
        using line = simplex<1>;
        using plane = simplex<2>;
        using space = simplex<3>;

        using vector = typename space::vector<X, dim>;
        using planar = typename space::planar<X, dim>;

        // whether a line contains a point, etc.
        template <size_t a, size_t b>
        static bool contains (const simplex<a> &, const simplex<b> &);

        // connect two points to make a line, etc.
        template <size_t a, size_t b>
        static simplex<a + b> connect (const simplex<a> &, const simplex<b> &);

        // we represent objects as exterior algebraic objects of one higher dimension.
        template <size_t order> struct simplex : space::exterior<X, order + 1, dim + 1> {
            simplex (std::initializer_list<simplex<order - 1>>);

            // valid if nonzero and not parallel to the affine subspace.
            bool valid () const;

            // subtract a simplex from a simplex and you get an exterior object.
            // (ie, point - point = vector.
            exterior<X, order + 1, dim> operator - (const simplex &p) const;

            // and of course we can also invert the former operation.
            simplex operator + (const exterior<X, order, dim> &v) const;
        };

        struct transformation : linear::matrix<X, dim + 1, dim + 1> {
            bool valid () const;

            transformation operator * (const transformation &) const;

            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };

        static transformation translation (const vector &);
        static transformation scale (const X &);

        // flip over the first exterior object, leave the other unchanged (must be linearly independent).
        template <size_t order>
        static transformation flip (const exterior<X, order, dim> &, const exterior<X, dim - order, dim> &);
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

        // rational trigonometry
        Q spread (const line &, const line &);

        struct transformation : affine<X, dim>::transformation {
            // must be orthogonal.
            bool valid () const;

            transformation operator * (const transformation &) const;
        };

        static transformation translation (const vector<X, dim> &);
        static transformation scale (const X &);

        // flip over the exterior object, leave everything perpendicular to it unchanged.
        // rotations can be made out of two vector flips multiplied together. We do not
        // have a native function for rotations because you cannot do an arbitrary
        // rotation, depending on the field.
        template <size_t order>
        static transformation flip (const exterior<X, order, dim> &);
    };

    template <skew_field X, ordered Q, size_t dim>
    struct elliptic : Euclidian<X, Q, dim> {
        template <size_t order>
        struct simplex : Euclidian<X, Q, dim>::simplex<order> {
            using Euclidian<X, Q, dim>::template simplex<order>::simplex;

            // must be of quadrance 1.
            bool valid () const;
        };

        using point = simplex<1>;
        using line = simplex<2>;
        using plane = simplex<3>;
        using space = simplex<4>;

        template <size_t a, size_t b>
        static bool contains (const simplex<a> &, const simplex<b> &);

        struct transformation : linear::matrix<X, dim + 1, dim + 1> {
            // must be orthogonal
            bool valid () const;

            transformation operator * (const transformation &) const;

            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    template <skew_field X, size_t dim> struct projective {
        template <size_t order> using affine_exterior = space::template exterior<X, order, dim + 1>;

        template <size_t order>
        struct exterior : affine_exterior<order + 1> {
            using affine_exterior<order + 1>::affine_exterior;
            bool valid () const;

            // two exterior objects in projective space are equal if they are proportional to one another.
            bool operator == (const exterior &x) const;
        };

        using value = unsigned_limit<X>;

        template <size_t order>
        struct simplex : space::exterior<value, order, dim> {
            bool valid () const;
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
        using array::array;
        exterior (array &&);
        exterior (const array &);
        // it's valid if the symmetry properties are correct.
        bool valid ();
    };
    
}

namespace data::math::linear {

    template <skew_field X, size_t A, size_t B> X inline det (const matrix<X, A, B> &) {
        return 0;
    }

    template <skew_field X, size_t A, size_t B> bool inline invertable (const matrix<X, A, B> &) {
        return false;
    }

    template <skew_field X, size_t A> bool inline invertable (const matrix<X, A, A> &m) {
        return det (m) != 0;
    }

}

// NOTE: hereafter we use X {0} to represent the zero element of the field and X {1} to represent the unit.
// this is not necessarilly correct for all fields but it ought to work in ordinary cases. To be perfect we
// may need to be more careful in a later version.

namespace data::math::space {
    // valid if nonzero.
    template <skew_field X, size_t dim> template <size_t order>
    bool inline projective<X, dim>::exterior<order>::valid () const {
        return affine_exterior<order + 1>::valid () && static_cast<affine_exterior<order + 1>> (*this) != affine_exterior<order + 1> {0};
    }

    template <skew_field X, size_t dim>
    template <size_t order>
    bool inline projective<X, dim>::exterior<order>::operator == (const exterior &x) const {
        auto ia = this->begin ();
        auto ib = x.begin ();
        // the ratio between elements in these exterior objects.
        X ratio {0};
        while (ia != this->end ()) {
            // if any element is zero when the corresponding element in the
            // other object is nonzere, then these are definitely not equal.
            if (*ia == X {0} && *ib != X {0} || *ia != X {0} && *ib == X {0}) return false;
            if (*ia == X {0} && *ib == X {0}) {
                if (ratio == X {0}) ratio = *ia / *ib;
                else if (ratio != *ia / *ib) return false;
            }
        }

        return true;
    }

    template <skew_field X, size_t dim> template <size_t order>
    bool projective<X, dim>::simplex<order>::valid () const {
        if (!space::template exterior<value, order, dim>::valid ()) return false;
        // infinite values are not allowed to appear after the first finite value.
        bool first_finite_value = false;
        for (const value &v : *this)
            if (!is_infinite (v)) first_finite_value = true;
            else if (first_finite_value) return false;
        return true;
    };

    template <skew_field X, ordered Q, size_t dim> template <size_t order>
    bool inline elliptic<X, Q, dim>::simplex<order>::valid () const {
        return static_cast<exterior<X, order + 1, dim + 1>> (*this) * static_cast<exterior<X, order + 1, dim + 1>> (*this) == X {0};
    }

    template <skew_field X, size_t dim>
    bool affine<X, dim>::transformation::valid () const {
        if (!invertable (*this) && (*this)[dim * dim - 1] != X {1}) return false;
        for (int index = dim * (dim - 1); index < dim * dim - 1; index++) if ((*this)[index] != X {0}) return false;
        return true;
    }

    template <skew_field X, size_t dim> template <size_t order>
    affine<X, dim>::transformation inline affine<X, dim>::flip (const exterior<X, order, dim> &a, const exterior<X, dim - order, dim> &b) {
        return projector (b, a) - projector (a, b);
    }
}

#endif

