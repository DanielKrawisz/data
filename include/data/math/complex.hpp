// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/cayley_dickson.hpp>

namespace data::math {

    template <Ring R> struct complex;

    template <Ring R> bool operator == (const complex<R> &, const complex<R> &);

    template <Ring R, ImplicitlyConvertible<R> W> bool operator == (const complex<R> &, const W &);

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> operator + (const complex<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<R> W> complex<R> operator + (const W &, const complex<R> &);

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> operator - (const complex<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<R> W> complex<R> operator - (const W &, const complex<R> &);

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> operator * (const complex<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<R> W> complex<R> operator * (const W &, const complex<R> &);

    template <Field R> complex<R> operator ~ (const complex<R> &);
    template <Field R> complex<R> operator / (const complex<R> &, const complex<R> &);
    template <Field R, ImplicitlyConvertible<R> W> complex<R> operator / (const complex<R> &, const W &);
    template <Field R, ImplicitlyConvertible<R> W> complex<R> operator / (const W &, const complex<R> &);

    template <typename R> std::ostream &operator << (std::ostream &, const complex<R> &);
    
    template <Ring R>
    struct complex : cayley_dickson<R> {
        
        static complex I () {
            static complex i {0, 1};
            return i;
        }

        using cayley_dickson<R>::cayley_dickson;
        complex (cayley_dickson<R> &&c) : cayley_dickson<R> {c} {}
        
        complex operator * () const {
            return cayley_dickson<R>::operator * ();
        }
        
        complex operator + (const complex &x) const {
            return cayley_dickson<R>::operator + (x);
        }
        
        complex operator - () const {
            return cayley_dickson<R>::operator - ();
        }
        
        complex operator - (const complex &x) const {
            return cayley_dickson<R>::operator - (x);
        }
        
        complex operator * (const complex &x) const {
            return cayley_dickson<R>::operator * (x);
        }
    };

    template <typename R> std::ostream &operator << (std::ostream &o, const complex<R> &x) {
        return o << "(" << x.Even << " + i " << x.Odd << ")";
    }
}

namespace data::math::def {

    template <typename q> struct ev<complex<q>> : ev<cayley_dickson<q>> {};

    template <typename q> struct od<complex<q>> : od<cayley_dickson<q>> {};
    
    template <typename q>
    struct inverse<plus<complex<q>>, complex<q>> {
        complex<q> operator () (const complex<q> &a, const complex<q> &b) {
            return b - a;
        }
    };

    template <typename q>
    struct times<complex<q>> {
        complex<q> operator () (const complex<q> &a, const complex<q> &b) {
            return a * b;
        }

        nonzero<complex<q>> operator () (const nonzero<complex<q>> &a, const nonzero<complex<q>> &b) {
            return a * b;
        }
    };

    template <typename q>
    struct plus<complex<q>> {
        complex<q> operator () (const complex<q> &a, const complex<q> &b) {
            return a + b;
        }
    };

    template <Field q>
    struct inverse<times<complex<q>>, complex<q>> : inverse<times<q>, q> {
        nonzero<complex<q>> operator () (const nonzero<complex<q>> &a, const nonzero<complex<q>> &b) {
            return b / a;
        }
    };

    template <WholeNumber Z>
    struct round<complex<Z>> {
        constexpr complex<Z> operator () (const complex<Z> &x) {
            return x;
        }
    };

    template <std::floating_point X>
    struct round<complex<X>> {
        constexpr complex<X> operator () (const complex<X> &x) {
            return complex<X> {data::round (math::ev (x)), data::round (math::od (x))};
        }
    };

}

namespace data::math::linear {
    
    template <typename q> 
    struct dimensions<q, complex<q>> : dimensions<q, cayley_dickson<q>> {};

}

namespace data::math {

    template <Field R> complex<R> inline operator ~ (const complex<R> &x) {
        if (x == 0) throw division_by_zero {};
        return def::inverse<def::times<cayley_dickson<R>>, cayley_dickson<R>> {} (nonzero {static_cast<const cayley_dickson<R> &> (x)}).Value;
    }

    template <Ring R> bool inline operator == (const complex<R> &a, const complex<R> &b) {
        return static_cast<cayley_dickson<R>> (a) == static_cast<cayley_dickson<R>> (b);
    }

    template <Ring R, ImplicitlyConvertible<R> W> bool inline operator == (const complex<R> &a, const W &b) {
        return static_cast<cayley_dickson<R>> (a) == R (b);
    }

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> inline operator + (const complex<R> &a, const W &b) {
        return static_cast<cayley_dickson<R>> (a) + R (b);
    }

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> inline operator + (const W &a, const complex<R> &b) {
        return R (a) + static_cast<cayley_dickson<R>> (b);
    }

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> inline operator - (const complex<R> &a, const W &b) {
        return static_cast<cayley_dickson<R>> (a) - R (b);
    }

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> inline operator - (const W &a, const complex<R> &b) {
        return R (a) - static_cast<cayley_dickson<R>> (b);
    }

    template <Ring R, ImplicitlyConvertible<R> W> complex<R> inline operator * (const complex<R> &a, const W &b) {
        return static_cast<cayley_dickson<R>> (a) * R (b);
    }

    template <Field R, ImplicitlyConvertible<R> W> complex<R> inline operator * (const W &a, const complex<R> &b) {
        return R (a) * static_cast<cayley_dickson<R>> (b);
    }

    template <Field R, ImplicitlyConvertible<R> W> complex<R> inline operator / (const complex<R> &a, const W &b) {
        return static_cast<cayley_dickson<R>> (a) / R (b);
    }

    template <Field R, ImplicitlyConvertible<R> W> complex<R> inline operator / (const W &a, const complex<R> &b) {
        return R (a) / static_cast<cayley_dickson<R>> (b);
    }

    template <Field R> complex<R> inline operator / (const complex<R> &a, const complex<R> &b) {
        return a * ~b;
    }
}
