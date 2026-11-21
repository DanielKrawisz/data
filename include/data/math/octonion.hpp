// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/quaternion.hpp>

namespace data::math {
    template <Ring R> class octonion;

    template <Ring R> bool operator == (const octonion<R> &, const octonion<R> &);

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> bool operator == (const octonion<R> &, const W &);

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator + (const octonion<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator + (const W &, const octonion<R> &);

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator - (const octonion<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator - (const W &, const octonion<R> &);

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator * (const octonion<R> &, const W &);
    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator * (const W &, const octonion<R> &);

    template <Field R> octonion<R> operator ~ (const octonion<R> &);
    template <Field R> octonion<R> operator / (const octonion<R> &, const octonion<R> &);
    template <Field R, ImplicitlyConvertible<R> W> octonion<R> operator / (const octonion<R> &, const W &);
    template <Field R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> operator / (const W &, const octonion<R> &);
    template <Field R> octonion<R> operator / (const octonion<R> &, const complex<R> &);
    template <Field R> octonion<R> operator / (const octonion<R> &, const quaternion<R> &);

    template <Ring R> std::ostream &operator << (std::ostream &o, const octonion<R> &x);

    template <Ring R>
    struct octonion : public cayley_dickson<quaternion<R>> {
        using com = complex<R>;
        using ham = quaternion<R>;
        using oct = cayley_dickson<ham>;

        using oct::oct;
        octonion (const com &x) : octonion {ham {x}} {}
        octonion (const ham &x) : oct {x, ham {}} {}
        octonion (const com &a, const com &b, const com &c, const com &d) :
            octonion {ham {a, b}, ham {c, d}} {}
        octonion (R r, R i, R j, R k, R l, R m, R n, R o) :
            octonion {com {r, i}, com {j, k}, com {l, m}, com {n, o}} {}
        octonion (oct &&o) : oct {o} {}
        
        static octonion E0 () {
            static octonion e0 {1, 0, 0, 0, 0, 0, 0, 0};
            return e0;
        }

        static octonion E1 () {
            static octonion e1 {0, 1, 0, 0, 0, 0, 0, 0};
            return e1;
        }

        static octonion E2 () {
            static octonion e2 {0, 0, 1, 0, 0, 0, 0, 0};
            return e2;
        }

        static octonion E3 () {
            static octonion e3 {0, 0, 0, 1, 0, 0, 0, 0};
            return e3;
        }

        static octonion E4 () {
            static octonion e4 {0, 0, 0, 0, 1, 0, 0, 0};
            return e4;
        }

        static octonion E5 () {
            static octonion e5 {0, 0, 0, 0, 0, 1, 0, 0};
            return e5;
        }

        static octonion E6 () {
            static octonion e6 {0, 0, 0, 0, 0, 0, 1, 0};
            return e6;
        }

        static octonion E7 () {
            static octonion e7 {0, 0, 0, 0, 0, 0, 0, 1};
            return e7;
        }
        
        octonion operator * () const {
            return oct::operator * ();
        }
        
        octonion operator + (const octonion &x) const {
            return oct::operator + (x);
        }
        
        octonion operator - () const {
            return oct::operator - ();
        }
        
        octonion operator - (const octonion &x) const {
            return oct::operator - (x);
        }
        
        octonion operator * (octonion x) const {
            return oct::operator * (x);
        }
    };

    template <Ring R> bool inline operator == (const octonion<R> &a, const octonion<R> &b) {
        return static_cast<cayley_dickson<quaternion<R>>> (a) == static_cast<cayley_dickson<quaternion<R>>> (b);
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> bool inline operator == (const octonion<R> &a, const W &b) {
        return a == octonion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator + (const octonion<R> &a, const W &b) {
        return a + octonion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator + (const W &a, const octonion<R> &b) {
        return octonion<R> (a) + b;
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator - (const octonion<R> &a, const W &b) {
        return a - octonion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator - (const W &a, const octonion<R> &b) {
        return octonion<R> (a) + b;
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator * (const octonion<R> &a, const W &b) {
        return a * octonion<R> (b);
    }

    template <Ring R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator * (const W &a, const octonion<R> &b) {
        return octonion<R> (a) + b;
    }

    template <Field R, ImplicitlyConvertible<R> W> octonion<R> operator / (const octonion<R> &a, const W &b) {
        auto bb = R (b);
        return octonion<R> {a.Even / bb, a.Odd / bb};
    }

    template <Field R> octonion<R> operator / (const octonion<R> &a, const complex<R> &b) {
        return octonion<R> {a.Even / b, a.Odd / b};
    }

    template <Field R> octonion<R> operator / (const octonion<R> &a, const quaternion<R> &b) {
        return octonion<R> {a.Even / b, a.Odd / b};
    }

    template <Field R, ImplicitlyConvertible<quaternion<R>> W> octonion<R> inline operator / (const W &a, const octonion<R> &b) {
        return octonion<R> (a) + b;
    }

    template <Field R> octonion<R> inline operator ~ (const octonion<R> &x) {
        if (x == 0) throw division_by_zero {};
        using oct = octonion<R>::oct;
        return def::inverse<def::times<oct>, oct> {} (nonzero {static_cast<const oct &> (x)}).Value;
    }

    template <Field R> octonion<R> inline operator / (const octonion<R> &a, const octonion<R> &b) {
        return a * ~b;
    }

    template <Ring R> std::ostream &operator << (std::ostream &o, const octonion<R> &x) {
        return o << "(" << ev (x) << " + k" << od (x) << ")";
    }

    namespace def {

        template <typename R>
        struct inverse<plus<octonion<R>>, octonion<R>> {
            octonion<R> operator () (const octonion<R> &a, const octonion<R> &b) {
                return b - a;
            }
        };

        template <typename q>
        struct times<octonion<q>> {
            octonion<q> operator () (const octonion<q> &a, const octonion<q> &b) {
                return a * b;
            }

            nonzero<octonion<q>> operator () (const nonzero<octonion<q>> &a, const nonzero<octonion<q>> &b) {
                return a * b;
            }
        };

        template <typename q>
        struct plus<octonion<q>> {
            octonion<q> operator () (const octonion<q> &a, const octonion<q> &b) {
                return a + b;
            }
        };

        template <typename q>
        struct inverse<times<octonion<q>>, octonion<q>> : inverse<times<q>, q> {
            nonzero<octonion<q>> operator () (const nonzero<octonion<q>> &a, const nonzero<octonion<q>> &b) {
                return b / a;
            }
        };

        template <typename q>
        struct divide<octonion<q>, octonion<q>> {
            octonion<q> operator () (const octonion<q> &a, const nonzero<octonion<q>> &b) {
                if (b == 0) throw division_by_zero {};
                return a / b.Value;
            }
        };

        template <typename q> struct ev<octonion<q>> : ev<cayley_dickson<quaternion<q>>> {};

        template <typename q> struct od<octonion<q>> : od<cayley_dickson<quaternion<q>>> {};

        template <typename q>
        struct divide<octonion<q>, quaternion<q>> {
            octonion<q> operator () (const octonion<q> &a, const nonzero<quaternion<q>> &b);
        };

        template <typename q>
        struct divide<octonion<q>, complex<q>> {
            octonion<q> operator () (const octonion<q> &a, const nonzero<complex<q>> &b);
        };

        template <typename q>
        struct divide<octonion<q>, q> {
            octonion<q> operator () (const octonion<q> &a, const nonzero<q> &b);
        };

        template <WholeNumber Z>
        struct round<octonion<Z>> {
            constexpr octonion<Z> operator () (const octonion<Z> &x) {
                return x;
            }
        };

        template <std::floating_point X>
        struct round<octonion<X>> {
            constexpr octonion<X> operator () (const octonion<X> &x) {
                return octonion<X> {data::round (math::ev (x)), data::round (math::od (x))};
            }
        };
    }
}

namespace data::math::linear {
    
    template <typename q> 
    struct dimensions<q, octonion<q>> : dimensions<q, cayley_dickson<quaternion<q>>> {};

    template <typename q>
    struct dimensions<complex<q>, octonion<q>> {
        static constexpr dimension value = 4;
    };

    template <typename q>
    struct dimensions<quaternion<q>, octonion<q>> {
        static constexpr dimension value = 2;
    };
    
}
