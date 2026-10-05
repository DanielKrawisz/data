// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/complex.hpp>

namespace data::math {
    template <Ring R> class quaternion;

    template <Ring R> bool operator == (const quaternion<R> &, const quaternion<R> &);

    template <Ring R, ImplicitlyConvertible<complex<R>> W> bool operator == (const quaternion<R> &, const W &);

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator + (const quaternion<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator + (const W &, const quaternion<R> &);

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator - (const quaternion<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator - (const W &, const quaternion<R> &);

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator * (const quaternion<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator * (const W &, const quaternion<R> &);

    template <Field R> quaternion<R> operator ~ (const quaternion<R> &);
    template <Field R> quaternion<R> operator / (const quaternion<R> &, const quaternion<R> &);
    template <Field R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator / (const quaternion<R> &, const W &);
    template <Field R, ImplicitlyConvertible<complex<R>> W> quaternion<R> operator / (const W &, const quaternion<R> &);

    template <Ring R> std::ostream &operator << (std::ostream &o, const quaternion<R> &x);

    template <Ring R>
    struct quaternion : public cayley_dickson<complex<R>> {
        using complex = math::complex<R>;
        using hamiltonian = cayley_dickson<complex>;
        
        using hamiltonian::hamiltonian;
        quaternion (R r, R i, R j, R k) : quaternion {complex {r, i}, complex {j, k}} {}
        quaternion (const complex &x) : quaternion {x, complex {}} {}
        quaternion (hamiltonian &&c) : hamiltonian {c} {}
        
        static quaternion I () {
            static quaternion i {0, 1, 0, 0};
            return i;
        }

        static quaternion J () {
            static quaternion j {0, 0, 1, 0};
            return j;
        }

        static quaternion K () {
            static quaternion k {0, 0, 0, 1};
            return k;
        }
        
        quaternion conjugate () const {
            return hamiltonian::conjugate ();
        }
        
        quaternion operator * () const {
            return hamiltonian::operator * ();
        }
        
        quaternion operator + (const quaternion &x) const {
            return hamiltonian::operator + (x);
        }
        
        quaternion operator - () const {
            return hamiltonian::operator - ();
        }
        
        quaternion operator - (const quaternion &x) const {
            return hamiltonian::operator - (x);
        }
        
        quaternion operator * (const quaternion &x) const {
            return hamiltonian::operator * (x);
        }
    };

    template <Ring R> bool operator == (const quaternion<R> &a, const quaternion<R> &b) {
        return static_cast<cayley_dickson<complex<R>>> (a) == static_cast<cayley_dickson<complex<R>>> (b);
    }

    template <Ring R> std::ostream &operator << (std::ostream &o, const quaternion<R> &x) {
        return o << "(" << ev (x) << " + j " << od (x) << ")";
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> bool inline operator == (const quaternion<R> &a, const W &b) {
        return a == quaternion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator + (const quaternion<R> &a, const W &b) {
        return a + quaternion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator + (const W &a, const quaternion<R> &b) {
        return quaternion<R> (a) + b;
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator - (const quaternion<R> &a, const W &b) {
        return a - quaternion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator - (const W &a, const quaternion<R> &b) {
        return quaternion<R> (a) - b;
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator * (const quaternion<R> &a, const W &b) {
        return a * quaternion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator * (const W &a, const quaternion<R> &b) {
        return quaternion<R> (a) * b;
    }

    template <Field R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator / (const quaternion<R> &a, const W &b) {
        return a / quaternion<R> (b);
    }

    template <Field R, ImplicitlyConvertible<complex<R>> W> quaternion<R> inline operator / (const W &a, const quaternion<R> &b) {
        return quaternion<R> (a) / b;
    }

    template <Field R> quaternion<R> inline operator ~ (const quaternion<R> &x) {
        if (x == 0) throw division_by_zero {};
        using CD = quaternion<R>::hamiltonian;
        return def::inverse<def::times<CD>, CD> {} (nonzero {static_cast<const CD &> (x)}).Value;
    }

    template <Field R> quaternion<R> inline operator / (const quaternion<R> &a, const quaternion<R> &b) {
        return a * ~b;
    }
}

namespace data::math::def {
    template <typename q> struct ev<quaternion<q>> : ev<cayley_dickson<complex<q>>> {};

    template <typename q> struct od<quaternion<q>> : od<cayley_dickson<complex<q>>> {};
    
    template <typename R>
    struct inverse<plus<quaternion<R>>, quaternion<R>> {
        quaternion<R> operator () (const quaternion<R> &a, const quaternion<R> &b) {
            return b - a;
        }
    };

    template <typename q>
    struct times<quaternion<q>> {
        quaternion<q> operator () (const quaternion<q> &a, const quaternion<q> &b) {
            return a * b;
        }

        nonzero<quaternion<q>> operator () (const nonzero<quaternion<q>> &a, const nonzero<quaternion<q>> &b) {
            return a * b;
        }
    };

    template <typename q>
    struct plus<quaternion<q>> {
        quaternion<q> operator () (const quaternion<q> &a, const quaternion<q> &b) {
            return a + b;
        }
    };

    template <typename q>
    struct inverse<times<quaternion<q>>, quaternion<q>> : inverse<times<q>, q> {
        nonzero<quaternion<q>> operator () (const nonzero<quaternion<q>> &a, const nonzero<quaternion<q>> &b) {
            return b / a;
        }
    };

    template <WholeNumber Z>
    struct round<quaternion<Z>> {
        constexpr quaternion<Z> operator () (const quaternion<Z> &x) {
            return x;
        }
    };

    template <std::floating_point X>
    struct round<quaternion<X>> {
        constexpr quaternion<X> operator () (const quaternion<X> &x) {
            return quaternion<X> {data::round (math::ev (x)), data::round (math::od (x))};
        }
    };
    
}

namespace data::math::linear {
    
    template <typename q> 
    struct dimensions<q, math::quaternion<q>> : dimensions<q, math::cayley_dickson<complex<q>>> {};

    template <typename q>
    struct dimensions<math::complex<q>, quaternion<q>> {
        static constexpr dimension value = 2;
    };
    
}
