// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/types.hpp>
#include <data/math/figurate.hpp>
#include <data/math/linear/space.hpp>
#include <data/math/algebra/algebra.hpp>

namespace data::math::linear {

    template <field X, size_t A, size_t B> using matrix = array<X, A, B>;

    template <field X, size_t A, size_t B> X det (const matrix<X, A, B> &);
    template <field X, size_t A> X det (const matrix<X, A, A> &);

    template <field X, size_t A, size_t B> bool invertable (const matrix<X, A, B> &);
    template <field X, size_t A> bool invertable (const matrix<X, A, A> &);
    template <field X, size_t A> matrix<X, A, A> inverse (const matrix<X, A, A> &);

};

namespace data::math::space {

    // the exterior algebra comprises scalars, vectors, asymmetric 2-tensors, etc.
    template <field X, size_t dim, size_t order> struct exterior;

    template <field X, size_t dim> using scalar = exterior<X, dim, 0>;
    template <field X, size_t dim> using vector = exterior<X, dim, 1>;
    // nonstandard terminology but it helps to remember what these things are.
    template <field X, size_t dim> using planar = exterior<X, dim, 2>;
    template <field X, size_t dim> using spacer = exterior<X, dim, 3>;

    // scalar multiplication
    template <field X, size_t dim, size_t u>
    exterior <X, dim, u> operator * (const exterior<X, dim, u> &, const X &);

    // addition
    template <field X, size_t dim, size_t u>
    exterior <X, dim, u> operator + (const exterior<X, dim, u> &, const exterior<X, dim, u> &);

    // exterior product
    template <field X, size_t dim, size_t A, size_t B>
    exterior <X, dim, A + B> operator ^ (const exterior<X, dim, A> &, const exterior<X, dim, B> &);

    // Hodge star
    template <field X, size_t dim, size_t order>
    requires requires (const X &x, const X &y) {
        {x * data::inner (x, y)};
    } exterior<X, dim, dim - order> operator * (const exterior<X, dim, order> &);

    // generate a matrix that projects onto the subspace defined by the exterior object.
    // the second exterior object is left unchanged.
    template <field X, size_t dim, size_t order>
    linear::matrix<X, dim, dim> projector (const exterior<X, dim, order> &a, const exterior<X, dim, dim - order> &b);

    template <field X, size_t dim, size_t order> requires requires (const X &x, const X &y) {
        {x * data::inner (x, y)};
    } auto operator * (const exterior<X, dim, order> &a, const exterior<X, dim, order> &b);

    // specialization for scalar type.
    template <field X, size_t dim> class exterior<X, dim, 0> {
        X Value;
    public:

        exterior (const X &x) : Value {x} {}
    };

    namespace {

        // Forward declaration
        template <typename, size_t, size_t, typename> struct exterior_component_tuple;

        // Specialization using index sequence
        template <typename X, size_t dim, size_t order, size_t... is>
        struct exterior_component_tuple<X, dim, order, std::index_sequence<is...>> {
            using type = std::tuple<exterior<X, dim - is, order>...>;
        };

        template <typename X, size_t dim, size_t order>
        using exterior_components = exterior_component_tuple<X, dim - 1, order - 1, std::make_index_sequence<dim - order + 1>>::type;

        template <field X, size_t dim, size_t order>
        array<X, binomial (dim, order)> flatten (const exterior_components<X, dim, order> &);
    }

    template <field X, size_t dim, size_t order> class exterior {
        array<X, binomial (dim, order)> Values;

    public:
        template <typename... Args>
        exterior (Args &&...args) : Values {flatten<X, dim, order> (exterior_components<X, dim, order> {std::forward<Args> (args)...})} {}
    };

    // affine geometry is like Euclid without circles.
    // technically, we do not have the full exterior algebra
    // available because we don't have the Hodge star without
    // a notion of perpendicularity.
    template <field X, size_t dim> struct affine {
        // technically, this is an incorrect use of the term.
        template <size_t order> struct simplex;

        using point = simplex<0>;
        using line = simplex<1>;
        using plane = simplex<2>;
        using space = simplex<3>;

        using vector = data::math::space::exterior<X, dim, 1>;
        using planar = data::math::space::exterior<X, dim, 2>;

        // whether a line contains a point, etc.
        template <size_t a, size_t b>
        static bool contains (const simplex<a> &, const simplex<b> &);

        // connect two points to make a line, etc.
        template <size_t a, size_t b>
        static simplex<a + b> connect (const simplex<a> &, const simplex<b> &);

        // we represent objects as exterior algebraic objects of one higher dimension.
        template <size_t order> struct simplex : data::math::space::exterior<X, dim + 1, order + 1> {

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

    template <field X, size_t dim> requires normed<X> &&
    ordered<decltype (data::norm (std::declval<X> ()))>
    struct Euclidian : affine<X, dim> {
        template <size_t order> using simplex = affine<X, dim>::simplex;

        using point = affine<X, dim>::point;
        using line = affine<X, dim>::line;
        using plane = affine<X, dim>::plane;
        using space = affine<X, dim>::space;

        using R = decltype (data::norm (std::declval<X> ()));

        // distance squared
        R quadrance (const point &, const point &);

        // rational trigonometry
        R spread (const line &, const line &);

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

    template <field X, size_t dim>
    struct elliptic : Euclidian<X, dim> {
        template <size_t order>
        struct simplex : Euclidian<X, dim>::template simplex<order> {
            using Euclidian<X, dim>::template simplex<order>::simplex;

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

    template <field X, size_t dim> struct projective {
        template <size_t order> using affine_exterior = space::template exterior<X, order, dim + 1>;

        template <size_t order>
        struct exterior : affine_exterior<order + 1> {
            using affine_exterior<order + 1>::affine_exterior;
            bool valid () const;

            // two exterior objects in projective space are equal if they are proportional to one another.
            bool operator == (const exterior &x) const;
        };

        using coordinate = unsigned_limit<X>;

        // no finite value can appear before an infinite value.
        template <size_t order>
        struct simplex : space::exterior<coordinate, order, dim> {
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
    
}

namespace data::math {
    template <field X, size_t dim, size_t order>
    struct inverse<plus<space::exterior<X, dim, order>>, space::exterior<X, dim, order>> {
        space::exterior<X, dim, order> operator () (const space::exterior<X, dim, order> &, const space::exterior<X, dim, order> &) const;
    };

    template <field X, size_t dim, size_t order>
    struct conjugate<space::exterior<X, dim, order>> {
        space::exterior<X, dim, dim - order> operator () (const space::exterior<X, dim, order> &) const;
    };
}

namespace data::math::linear {

    template <field X, size_t A, size_t B> X inline det (const matrix<X, A, B> &) {
        return 0;
    }

    template <field X, size_t A, size_t B> bool inline invertable (const matrix<X, A, B> &) {
        return false;
    }

    template <field X, size_t A> bool inline invertable (const matrix<X, A, A> &m) {
        return det (m) != 0;
    }

}

// NOTE: hereafter we use X {0} to represent the zero element of the field and X {1} to represent the unit.
// this is not necessarilly correct for all fields but it ought to work in ordinary cases. To be perfect we
// may need to be more careful in a later version.

namespace data::math::space {
    // valid if nonzero.
    template <field X, size_t dim> template <size_t order>
    bool inline projective<X, dim>::exterior<order>::valid () const {
        return affine_exterior<order + 1>::valid () && static_cast<affine_exterior<order + 1>> (*this) != affine_exterior<order + 1> {0};
    }

    template <field X, size_t dim>
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

    template <field X, size_t dim> template <size_t order>
    bool projective<X, dim>::simplex<order>::valid () const {
        if (!space::template exterior<coordinate, order, dim>::valid ()) return false;
        // infinite values are not allowed to appear after the first finite value.
        bool first_finite_value = false;
        for (const coordinate &v : *this)
            if (!is_infinite (v)) first_finite_value = true;
            else if (first_finite_value) return false;
        return true;
    };

    template <field X, size_t dim> template <size_t order>
    bool inline elliptic<X, dim>::simplex<order>::valid () const {
        return static_cast<exterior<X, order + 1, dim + 1>> (*this) * static_cast<exterior<X, order + 1, dim + 1>> (*this) == X {0};
    }

    template <field X, size_t dim>
    bool affine<X, dim>::transformation::valid () const {
        if (!invertable (*this) && (*this)[dim * dim - 1] != X {1}) return false;
        for (int index = dim * (dim - 1); index < dim * dim - 1; index++) if ((*this)[index] != X {0}) return false;
        return true;
    }

    template <field X, size_t dim> template <size_t order>
    affine<X, dim>::transformation inline affine<X, dim>::flip (const exterior<X, order, dim> &a, const exterior<X, dim - order, dim> &b) {
        return projector (b, a) - projector (a, b);
    }
}

#endif

