// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/math/linear/exterior.hpp>

namespace data::math::space {

    template <ring X, size_t dim> using scalar = exterior<X, dim, 0>;
    template <ring X, size_t dim> using vector = exterior<X, dim, 1>;
    template <ring X, size_t dim> using bivec = exterior<X, dim, 2>;
    template <ring X, size_t dim> using trivec = exterior<X, dim, 3>;

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

        using vector = data::math::exterior<X, dim, 1>;
        using planar = data::math::exterior<X, dim, 2>;

        // whether a line contains a point, etc.
        template <size_t a, size_t b>
        constexpr static bool contains (const simplex<a> &, const simplex<b> &);

        // connect two points to make a line, etc.
        template <size_t a, size_t b>
        constexpr static simplex<a + b> connect (const simplex<a> &, const simplex<b> &);

        // we represent objects as exterior algebraic objects of one higher dimension.
        template <size_t order> struct simplex : data::math::exterior<X, dim + 1, order + 1> {

            // valid if nonzero and not parallel to the affine subspace.
            constexpr bool valid () const;

            // subtract a simplex from a simplex and you get an exterior object.
            // (ie, point - point = vector.
            constexpr exterior<X, dim, order + 1> operator - (const simplex &p) const;

            // and of course we can also invert the former operation.
            constexpr simplex operator + (const exterior<X, dim, order> &v) const;
        };

        struct transformation : matrix<X, dim + 1, dim + 1> {
            constexpr bool valid () const;

            constexpr transformation operator * (const transformation &) const;

            template <size_t order>
            constexpr simplex<order> operator * (const simplex<order> &) const;
        };

        constexpr static transformation translation (const vector &);
        constexpr static transformation scale (const X &);

        // flip over the first exterior object, leave the other unchanged (must be linearly independent).
        template <size_t order>
        constexpr static transformation flip (const exterior<X, dim, order> &, const exterior<X, dim, dim - order> &);
    };

    template <field X, size_t dim>
    //requires normed<X> && Ordered<decltype (data::norm (std::declval<X> ()))>
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

        struct transformation : matrix<X, dim + 1, dim + 1> {
            // must be orthogonal
            bool valid () const;

            transformation operator * (const transformation &) const;

            template <size_t order>
            simplex<order> operator * (const simplex<order> &) const;
        };
    };

    template <field X, size_t dim> struct projective {
        template <size_t order> using affine_exterior = data::math::template exterior<X, order, dim + 1>;

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
        struct simplex : data::math::exterior<coordinate, order, dim> {
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
    constexpr bool affine<X, dim>::transformation::valid () const {
        if (!invertable (*this) && (*this)[dim * dim - 1] != X {1}) return false;
        for (int index = dim * (dim - 1); index < dim * dim - 1; index++) if ((*this)[index] != X {0}) return false;
        return true;
    }

    template <field X, size_t dim> template <size_t order>
    constexpr affine<X, dim>::transformation inline affine<X, dim>::flip (const exterior<X, dim, order> &a, const exterior<X, dim, dim - order> &b) {
        return projector (b, a) - projector (a, b);
    }
}

#endif

