// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_EXTERIOR
#define DATA_MATH_LINEAR_EXTERIOR

#include <data/tuple.hpp>
#include <data/math/linear/matrix.hpp>
#include <data/math/field.hpp>
#include <data/math/combinatorics.hpp>

namespace data::math::space {

    // the exterior algebra comprises scalars, vectors, asymmetric 2-tensors, etc.
    template <ring X, size_t dim, size_t order> requires (order <= dim) struct exterior;

    template <ring X, size_t dim> using scalar = exterior<X, dim, 0>;
    template <ring X, size_t dim> using vector = exterior<X, dim, 1>;
    template <ring X, size_t dim> using bivec = exterior<X, dim, 2>;
    template <ring X, size_t dim> using trivec = exterior<X, dim, 3>;

    // scalar multiplication
    template <ring X, size_t dim, size_t u>
    exterior<X, dim, u> operator * (const exterior<X, dim, u> &, const X &);

    // addition
    template <ring X, size_t dim, size_t u>
    exterior<X, dim, u> operator + (const exterior<X, dim, u> &, const exterior<X, dim, u> &);

    // exterior product
    template <ring X, size_t dim, size_t A, size_t B> requires (A + B <= dim)
    exterior<X, dim, A + B> operator ^ (const exterior<X, dim, A> &, const exterior<X, dim, B> &);

    // Hodge star
    template <ring X, size_t dim, size_t order>
    requires requires (const X &x, const X &y) {
        {x * inner (x, y)};
    } exterior<X, dim, dim - order> operator * (const exterior<X, dim, order> &);

    template <ring X, size_t dim, size_t order>
    X contract (const exterior<X, dim, order> &, const exterior<X, dim, dim - order> &);
}

namespace data::math::def {
    // this turns exterior objects into additive groups.
    template <ring X, size_t dim, size_t order>
    struct inverse<plus<space::exterior<X, dim, order>>, space::exterior<X, dim, order>> {
        space::exterior<X, dim, order> operator () (const space::exterior<X, dim, order> &a, const space::exterior<X, dim, order> &b) const {
            return b - a;
        }
    };

    // this means that an inner product is automatically defined on exterior objects.
    template <ring X, size_t dim, size_t order>
    struct conjugate<space::exterior<X, dim, order>> {
        space::exterior<X, dim, dim - order> operator () (const space::exterior<X, dim, order> &x) const {
            return *x;
        }
    };
}

namespace data::math::space {

    // generate a matrix that projects onto the subspace defined by the exterior object.
    // the second exterior object is left unchanged.
    template <ring X, size_t dim, size_t order>
    linear::matrix<X, dim, dim> projector (const exterior<X, dim, order> &a, const exterior<X, dim, dim - order> &b);

    // specialization for scalar type.
    template <ring X, size_t dim> struct exterior<X, dim, 0> : array<X> {
        using array<X>::array;
    };

    // specialization for vector type.
    template <ring X, size_t dim> struct exterior<X, dim, 1> : array<X, dim> {
        using array<X, dim>::array;
    };

    namespace {
        template<class X, size_t dim, size_t order, size_t... Is>
        auto make_exterior_tuple (std::index_sequence<Is...>)
        -> std::tuple<exterior<X, dim - Is - 1, order - 1>...>;

        template <ring X, size_t dim, size_t order> struct exterior_parent {
            using type = decltype (make_exterior_tuple<X, dim, order> (std::make_index_sequence<dim - order + 1> {}));
        };

        // An exterior object of dimension d and order n has an element for every
        // permutation of every subset of the numbers 0 ... d - 1. However, elements
        // for permutations of the same subset are the same except that they may
        // differ by a sign. We save space by storing only one element for each
        // permutation. Here we mock a reference to an element in an exterior object
        // remembering whether it needs to change sign.
        template<class X> struct exterior_accessor;

        template <typename X> std::ostream &operator << (std::ostream &o, exterior_accessor<X> x);

        template<class X> struct exterior_accessor {
            X *Ptr;
            sign Signature;

            constexpr exterior_accessor (X &ref, sign s) : Ptr (&ref), Signature (s) {}

            constexpr operator X () const noexcept { return Signature == math::negative ? -(*Ptr) : (*Ptr); }

            constexpr exterior_accessor &operator = (const X &val) noexcept {
                *Ptr = Signature == math::negative ? -val : val;
                return *this;
            }

            constexpr exterior_accessor &operator += (const X &val) noexcept {
                *Ptr += Signature == math::negative ? -val : val;
                return *this;
            }

            constexpr exterior_accessor &operator -= (const X &val) noexcept {
                *Ptr -= Signature == math::negative ? -val : val;
                return *this;
            }

            constexpr exterior_accessor &operator *= (const X &val) noexcept {
                *Ptr *= val;
                return *this;
            }

            constexpr exterior_accessor &operator /= (const X &val) noexcept {
                *Ptr /= val;
                return *this;
            }

            constexpr bool operator == (const X &val) const {
                return X (*this) == val;
            }
        };

        template <typename X>
        std::ostream &operator << (std::ostream &o, exterior_accessor<X> x) {
            return o << X (x);
        }

        // we ensure that there are at least 2 arguments of size_t here but
        // that is necessarily true anyway since we have a special case for
        // exterior orders 0 and 1. Self can be either a const reference or
        // a reference to an exterior object.
        // result will be either the field element of the exterior object or
        // an exterior_accessor.
        template <size_t order, class Self, class... Sizes>
        constexpr auto access_exterior (Self &&self, size_t a, size_t b, Sizes... sizes);
    }

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    struct exterior : exterior_parent<X, dim, order>::type {
        using parent = exterior_parent<X, dim, order>::type;
        using parent::tuple;

        // all arguments must be size_t.
        template <typename ...sizes> requires (order == sizeof... (sizes))
        constexpr X operator [] (sizes... indices) const {
            return access_exterior<order> (*this, indices...);
        }

        // all arguments must be size_t.
        template <typename ...sizes> requires (order == sizeof... (sizes))
        constexpr exterior_accessor<X> operator [] (sizes... indices) {
            return access_exterior<order> (*this, indices...);
        }

        template <typename ex, typename val> struct ex_it;

        using iterator = ex_it<exterior<X, dim, order>, X>;
        using const_iterator = ex_it<const exterior<X, dim, order>, const X>;

        iterator begin ();
        iterator end ();

        const_iterator begin () const;
        const_iterator end () const;

    };

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    template <typename ex, typename val>
    struct exterior<X, dim, order>::ex_it {
        using value_type = val;
        using difference_type = int;

        value_type &operator * () const;

        value_type *operator -> () const {
            return &operator * ();
        }

        bool operator == (const ex_it &e) const {
            return Exterior == e.Exterior && It == e.It;
        }

        ex_it &operator ++ () {
            It++;
            return *this;
        }

        ex_it operator ++ (int) {
            auto x = *this;
            ++(*this);
            return x;
        }

        ex_it (): Exterior {nullptr}, It {} {}
        ex_it (ex *v, sublists<size_t>::iterator i):
            Exterior {v}, It {i} {}

    private:
        ex *Exterior;
        sublists<size_t>::iterator It;
    };

    // scalar multiplication
    template <ring X, size_t dim, size_t u>
    exterior<X, dim, u> operator * (const exterior<X, dim, u> &a, const X &b) {
        exterior<X, dim, u> result;
        auto ri = result.begin ();
        auto ai = a.begin ();
        while (ri != result.end ()) {
            *ri = *ai * b;
            ri++;
            ai++;
        }
        return result;
    }

    // addition
    template <ring X, size_t dim, size_t u>
    exterior<X, dim, u> operator + (const exterior<X, dim, u> &a, const exterior<X, dim, u> &b) {
        exterior<X, dim, u> result;
        auto ri = result.begin ();
        auto ai = a.begin ();
        auto bi = b.begin ();
        while (ri != result.end ()) {
            *ri = *ai + *bi;
            ri++;
            ai++;
            bi++;
        }
        return result;
    }

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    exterior<X, dim, order>::iterator inline exterior<X, dim, order>::begin () {
        return iterator {this, sublists<size_t> {range<size_t> (0, dim - 1), order}.begin ()};
    }

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    exterior<X, dim, order>::iterator inline exterior<X, dim, order>::end () {
        return iterator {this, sublists<size_t> {range<size_t> (0, dim - 1), order}.end ()};
    }

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    exterior<X, dim, order>::const_iterator inline exterior<X, dim, order>::begin () const {
        return const_iterator {this, sublists<size_t> {range<size_t> (0, dim - 1), order}.begin ()};
    }

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    exterior<X, dim, order>::const_iterator inline exterior<X, dim, order>::end () const {
        return const_iterator {this, sublists<size_t> {range<size_t> (0, dim - 1), order}.end ()};
    }

    namespace {

        // x can be either a const exterior & or an exterior &.
        template <size_t order, class Self>
        decltype (auto) exterior_get (Self &&x, slice<size_t> indices, int index_index = 0);

        template <typename it, typename sen>
        constexpr sign bubble_sort_with_signature (it begin, sen end);

        // we ensure that there are at least 2 arguments of size_t here but
        // that is necessarily true anyway since we have a special case for
        // exterior orders 0 and 1.
        template <size_t order, class Self, class... Sizes>
        constexpr auto access_exterior (Self &&self, size_t a, size_t b, Sizes... sizes) {
            cross<size_t> indices {a, b, static_cast<size_t> (sizes)...};
            sign signature = bubble_sort_with_signature (indices.begin (), indices.end ());
            // check for any duplicate indices.
            // we know we have at least two indices, so this will work.
            auto i = indices.begin ();
            auto j = i;
            j++;
            do {
                if (*j == *i) throw exception {"Indices to exterior object must all be different"};
                i = j;
                j++;
            } while (j != indices.end ());

            auto result = exterior_get<order> (self, indices);
            if constexpr (std::is_const_v<unref<Self>>) {
                return signature == sign::negative ? -result : result;
            } else {
                return exterior_accessor {exterior_get<order> (self, indices), signature};
            }
        }

        bool is_sorted (slice<size_t>);

        template <std::size_t... I> decltype (auto) inline get_element (auto &&x, std::index_sequence<I...>) {
            return std::forward<decltype (x)> (x)[I...];
        }

        list<size_t> complement (list<size_t> whole, list<size_t> part);
    }

    template <ring X, size_t dim, size_t order> requires (order <= dim)
    template <typename ex, typename val>
    exterior<X, dim, order>::ex_it<ex, val>::value_type inline &exterior<X, dim, order>::ex_it<ex, val>::operator * () const {
        cross<size_t> indices {*It};
        return exterior_get<order> (*Exterior, indices);
    }

    template <ring X, size_t dim, size_t order>
    X inline contract (const exterior<X, dim, order> &a, const exterior<X, dim, dim - order> &b) {
        return get_element (a ^ b, std::make_index_sequence<order> {});
    }

    // exterior product
    template <ring X, size_t dim, size_t A, size_t B> requires (A + B <= dim)
    exterior<X, dim, A + B> operator ^ (const exterior<X, dim, A> &a, const exterior<X, dim, B> &b) {
        if constexpr (A == 0) {
            return b * a[];
        }

        if constexpr (B == 0) {
            return a * b[];
        }

        // first we form the outer product.
        exterior<exterior<X, dim, B>, dim, A> outer;

        auto oi = outer.begin ();
        auto ai = a.begin ();

        while (oi != outer.end ()) {
            *oi = b * *ai;
            oi++;
            ai++;
        }

        // now we populate the result.
        exterior<X, dim, A + B> result;
        auto ri = result.begin ();

        sublists<size_t> subs {range<size_t> (0, dim - 1), A + B};
        for (auto sub = subs.begin (); sub != subs.end (); sub++) {
            permutations<size_t> perms {cross<size_t> (*sub)};
            for (auto p = perms.begin (); p != perms.end (); p++) {
                // if the first A elements of p are sorted and the last B elements of p are sorted
                if (!is_sorted (slice<size_t> {p->data (), A}) ||
                    !is_sorted (slice<size_t> {p->data () + A, B})) continue;

                *ri += exterior_get<B> (
                    exterior_get<A> (outer, slice<size_t> {p->data (), A}),
                    slice<size_t> {p->data (), A + B}, A);
            }
            ri++;
        }

        return result;
    }

    template <ring X, size_t dim, size_t order>
    requires requires (const X &x, const X &y) {
        {x * inner (x, y)};
    } exterior<X, dim, dim - order> operator * (const exterior<X, dim, order> &x) {
        exterior<X, dim, dim - order> result;
        list<size_t> dims = range<size_t> (0, dim - 1);
        for (const auto &sub : sublists<size_t> {dims, dim - order}) {
            cross<size_t> result_indices {sub};
            cross<size_t> input_indices {complement (dims, sub)};
            exterior_get<dim - order> (result, result_indices) = exterior_get<order> (x, input_indices);
        }

        return result;
    }

    namespace {

        template <size_t order, class Self>
        decltype (auto) exterior_get (Self &&x, slice<size_t> indices, int index_index) {
            if constexpr (order == 0)
                return x[];
            else if constexpr (order == 1)
                return x[indices[index_index] - index_index];
            else {
                using tuple = unref<Self>::parent;
                return apply_at (
                    static_cast<std::conditional_t<
                        std::is_const_v<unref<Self>>, const tuple&, tuple&>
                    > (x),
                    [&](auto &&sub) -> decltype (auto) {
                        return exterior_get<order - 1> (
                            std::forward<decltype (sub)> (sub),
                            indices,
                            index_index + 1);
                    },
                    indices[index_index] - index_index);
            }
        }

        // there is also a merge sort version of this if we need it.
        template <typename it, typename sen>
        constexpr sign bubble_sort_with_signature (it begin, sen end) {
            int swaps = 0;
            it i = begin;
            while (true) {
                it j = i;
                j++;
                if (j == end) return swaps % 2 == 0 ? sign::positive : sign::negative;
                do {
                    if (*j < *i) {
                        std::swap (*i, *j);
                        swaps++;
                    }
                    j++;
                } while (j != end);
                i++;
            }
        }

        bool is_sorted (slice<size_t> x) {
            if (x.size () < 2) return true;
            auto i = x.begin ();
            auto last = *i;
            i++;
            do {
                if (last > *i) return false;
            } while (i != x.end ());
            return true;
        }
    }
}

#endif
