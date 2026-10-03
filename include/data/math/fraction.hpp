// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/complex.hpp>
#include <data/math/number/rational.hpp>
#include <data/math/nonzero.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/octonion.hpp>

namespace data::math {

    // fraction is capable of taking any normed integral domain
    // (such as the integers or the gaussian integers)
    // and turning it into its fraction field. We can also use
    // it on the unit quaternions to form the rational quaternions
    // and same with octonions.
    template <typename Z, typename N = decltype (quadrance (std::declval<Z> ()))>
    requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    struct fraction;

    // construct a fraction
    template <typename Z, typename N = decltype (quadrance (std::declval<Z> ()))>
    constexpr math::fraction<Z, N> inline over (const Z &numerator, const Z &denominator);

    template <typename Z, typename N>
    constexpr bool operator == (const fraction<Z, N> &, const fraction<Z, N> &);

    template <typename Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr bool inline operator == (const fraction<Z> &a, const ZZ &b);

    template <typename Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr bool inline operator == (const ZZ &a, const fraction<Z> &b);

    template <Ordered Z, Ordered N>
    constexpr auto operator <=> (const fraction<Z, N> &x, const fraction<Z, N> &y);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr auto operator <=> (const fraction<Z> &, const ZZ &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr auto operator <=> (const ZZ &, const fraction<Z> &);

    template <typename Z, typename N> constexpr fraction<Z, N> operator - (const fraction<Z, N> &);
    template <typename Z, typename N> constexpr fraction<Z, N> operator ~ (const fraction<Z, N> &);

    template <typename Z, typename N>
    requires requires (Z z) { { *z } -> Same<Z>; }
    constexpr fraction<Z, N> operator * (const fraction<Z, N> &);

    template <typename Z, typename N> constexpr fraction<Z, N> operator + (const fraction<Z, N> &, const fraction<Z, N> &);
    template <typename Z, typename N> constexpr fraction<Z, N> operator - (const fraction<Z, N> &, const fraction<Z, N> &);
    template <typename Z, typename N> constexpr fraction<Z, N> operator * (const fraction<Z, N> &, const fraction<Z, N> &);
    template <typename Z, typename N> constexpr fraction<Z, N> operator / (const fraction<Z, N> &, const fraction<Z, N> &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator + (const fraction<Z> &, const ZZ &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator - (const fraction<Z> &, const ZZ &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator * (const fraction<Z> &, const ZZ &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator / (const fraction<Z> &, const ZZ &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator + (const ZZ &, const fraction<Z> &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator - (const ZZ &, const fraction<Z> &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator * (const ZZ &, const fraction<Z> &);

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> operator / (const ZZ &, const fraction<Z> &);

    namespace def {

        template <typename Z, typename N>
        struct numerator<fraction<Z, N>> {
            constexpr Z operator () (const fraction<Z, N> &x) {
                return x.Numerator;
            }
        };

        template <typename Z, typename N>
        struct denominator<fraction<Z, N>> {
            constexpr N operator () (const fraction<Z, N> &x) {
                return x.Denominator.Value;
            }
        };

        template <Ordered Z, Ordered N> struct sign<fraction<Z, N>> {
            constexpr math::sign operator () (const fraction<Z, N> &x);
        };

        template <typename Z, typename N>
        struct identity<plus<fraction<Z, N>>, fraction<Z, N>> : identity<plus<Z>, Z> {
            constexpr fraction<Z, N> operator () ();
        };

        template <typename Z, typename N>
        struct identity<times<fraction<Z, N>>, fraction<Z, N>> : identity<times<Z>, Z> {
            constexpr fraction<Z, N> operator () ();
        };

        template <typename Z, typename N>
        struct inverse<plus<fraction<Z, N>>, fraction<Z, N>> : inverse<plus<Z>, Z> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &a, const fraction<Z, N> &b);
        };

        template <typename Z, typename N> struct plus<fraction<Z, N>> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &a, const fraction<Z, N> &b) {
                return a + b;
            }
        };

        template <typename Z, typename N> struct times<fraction<Z, N>> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &a, const fraction<Z, N> &b) {
                return a * b;
            }

            constexpr nonzero<fraction<Z, N>> operator () (const nonzero<fraction<Z, N>> &a, const nonzero<fraction<Z, N>> &b) {
                return nonzero<fraction<Z, N>> {a.Value * b.Value};
            }
        };

        template <typename Z, typename N> struct inverse<times<fraction<Z, N>>, fraction<Z, N>>;

        template <typename Z, typename N> struct divide<fraction<Z, N>> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &, const nonzero<fraction<Z, N>> &);
        };

        template <typename Z, typename N> struct conjugate<fraction<Z, N>> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &x);
        };

        template <typename Z, typename N> requires requires (const Z &z) {
            { data::abs (z) };
        } struct abs<fraction<Z, N>> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &x);
        };

        template <typename Z> requires requires (const Z &x, const Z &y) {
            { inner<Z> {} (x, y) };
        } struct norm<fraction<Z, N>> {
            auto operator () (const fraction<Z, N> &);
        };

        template <WholeNumber Z, WholeNumber N>
        struct floor<fraction<Z, N>> {
            constexpr Z operator () (const fraction<Z, N> &x) {
                auto div = data::divmod (x.Numerator, x.Denominator);
                if (data::is_negative (div.Remainder))
                    return div.Quotient - 1;

                return div.Quotient;
            }
        };

        template <WholeNumber Z, WholeNumber N>
        struct ceiling<fraction<Z, N>> {
            constexpr Z operator () (const fraction<Z, N> &x) {
                auto div = data::divmod (x.Numerator, x.Denominator);
                if (data::is_positive (div.Remainder))
                    return div.Quotient + 1;

                return div.Quotient;
            }
        };

        template <WholeNumber Z, WholeNumber N>
        struct frac<fraction<Z, N>> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &x) {
                return x - data::floor (x);
            }
        };

        template <WholeNumber Z, WholeNumber N>
        struct round<fraction<Z, N>> {
            constexpr Z operator () (const fraction<Z, N> &x) {
                auto floor = data::floor (x);
                auto fractional_part = x - floor;
                if (fractional_part > fraction<Z, N> {1, 2}) return floor + 1;
                if (fractional_part < fraction<Z, N> {1, 2}) return floor;
                return data::even (floor) ? floor : floor + 1;
            }
        };

        template <WholeNumber Z, WholeNumber N>
        struct is_whole<fraction<Z, N>> {
            constexpr bool operator () (const fraction<Z, N> &x) {
                return x.Denominator.Value == 1;
            }
        };

        template <WholeNumber Z, WholeNumber N>
        struct mod<fraction<Z, N>, N> {
            constexpr fraction<Z, N> operator () (const fraction<Z, N> &x, const nonzero<N> &n) {
                return n.Value * data::frac (x / n.Value);
            }
        };

        // a way of constructing fractions.
        template <typename Z, typename N = decltype (quadrance (std::declval<Z> ()))> struct over;
    }

    template <typename Z, typename N>
    std::ostream &operator << (std::ostream &o, const fraction<Z, N> &x);

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    struct fraction {

        Z Numerator;
        nonzero<N> Denominator;

        constexpr bool valid () const {
            return data::valid (Numerator) && data::valid (Denominator);
        }

        constexpr fraction ();

        template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
        constexpr fraction (ZZ n);

        template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
        constexpr fraction (ZZ n, ZZ d);

        constexpr fraction &operator += (const fraction &f);
        constexpr fraction &operator -= (const fraction &f);
        constexpr fraction &operator *= (const fraction &f);
        constexpr fraction &operator /= (const fraction &f);

        // only use this if your fraction is already in lowest terms.
        constexpr fraction (Z n, nonzero<N> d) : Numerator {n}, Denominator {d} {}
        friend struct def::over<Z, N>;
    };

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline over (const Z &numerator, const Z &denominator) {
        return def::over<Z, N> {} (numerator, denominator);
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    constexpr inline fraction<Z, N>::fraction () : Numerator {0}, Denominator {1u} {}

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr inline fraction<Z, N>::fraction (ZZ n, ZZ d) : fraction (over<Z> (Z (n), Z (d))) {}

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr inline fraction<Z, N>::fraction (ZZ n) : Numerator {Z (n)}, Denominator {1u} {}

    template <Ordered Z, Ordered N>
    constexpr auto inline operator <=> (const fraction<Z, N> &x, const fraction<Z, N> &y) {
        if constexpr (requires { typename twice<Z>::type; }) {
            using doubled = typename twice<Z>::type;
            return static_cast<doubled> (x.Numerator) * static_cast<doubled> (y.Denominator.Value) <=> static_cast<doubled> (y.Numerator) * static_cast<doubled> (x.Denominator.Value);
        } else return x.Numerator * static_cast<Z> (y.Denominator.Value) <=> static_cast<Z> (y.Numerator * x.Denominator.Value);
    }

    template <typename Z, typename N>
    constexpr bool inline operator == (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a.Numerator == b.Numerator && (a.Numerator == 0 || a.Denominator == b.Denominator);
    }

    template <typename Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr bool inline operator == (const fraction<Z> &a, const ZZ &b) {
        return a == fraction<Z> {b};
    }

    template <typename Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr bool inline operator == (const ZZ &a, const fraction<Z> &b) {
        return fraction<Z> {a} == b;
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr auto inline operator <=> (const fraction<Z> &a, const ZZ &b) {
        return a <=> fraction<Z> {b};
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr auto inline operator <=> (const ZZ &a, const fraction<Z> &b) {
        return fraction<Z> {a} <=> b;
    }

}

namespace data::math::def {

    template <typename Z, typename N>
    requires NumberSystem<Z, N>
    struct over<Z, N> {
        constexpr fraction<Z, N> operator () (const Z &numerator, const Z &denominator) {
            if (denominator == 0) throw division_by_zero {};
            if (numerator == 0) return fraction<Z, N> {Z {0}, nonzero<N> {N {1u}}};
            N dabs = data::abs (denominator);
            N gcd_ab = data::GCD (data::abs (numerator), dabs);
            fraction<Z, N> x {Z (numerator * (denominator < 0 ? -1 : 1)) / Z (gcd_ab), nonzero<N> (dabs / gcd_ab)};
            return denominator < 0 ? -x: x;
        }
    };

    template <typename Z>
    requires SignedIntegral<Z>
    struct over<Z, Z> {
        constexpr fraction<Z, Z> operator () (const Z &numerator, const Z &denominator) {
            if (denominator == 0) throw division_by_zero {};
            if (numerator == 0) return fraction<Z, Z> {Z {0}, nonzero<Z> {Z {1}}};
            Z dabs = data::abs (denominator);
            Z gcd_ab = data::GCD (data::abs (numerator), dabs);
            fraction<Z, Z> x {Z (numerator * (denominator < 0 ? -1 : 1)) / Z (gcd_ab), nonzero<Z> (dabs / gcd_ab)};
            return denominator < 0 ? -x: x;
        }
    };

    template <WholeNumber Z, typename N>
    struct over<complex<Z>, N> {
        fraction<complex<Z>, N> operator () (const complex<Z> &numerator, const N &denominator);
        fraction<complex<Z>, N> operator () (const complex<Z> &numerator, const complex<Z> &denominator);
    };

    template <WholeNumber Z, typename N>
    struct over<quaternion<Z>, N> {
        fraction<quaternion<Z>, N> operator () (const quaternion<Z> &numerator, const N &denominator);
        fraction<quaternion<Z>, N> operator () (const quaternion<Z> &numerator, const quaternion<Z> &denominator);
    };

    template <WholeNumber Z, typename N>
    struct over<octonion<Z>, N> {
        fraction<octonion<Z>, N> operator () (const octonion<Z> &numerator, const N &denominator);
        fraction<octonion<Z>, N> operator () (const octonion<Z> &numerator, const octonion<Z> &denominator);
    };

    template <WholeNumber Z, typename N>
    struct ev<fraction<Z, N>> {
        fraction<Z, N> operator () (const fraction<Z, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct ev<fraction<complex<Z>, N>> {
        fraction<Z, N> operator () (const fraction<complex<Z>, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct ev<fraction<quaternion<Z>, N>> {
        fraction<complex<Z>, N> operator () (const fraction<quaternion<Z>, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct ev<fraction<octonion<Z>, N>> {
        fraction<quaternion<Z>, N> operator () (const fraction<octonion<Z>, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct od<fraction<Z, N>> {
        fraction<Z, N> operator () (const fraction<Z, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct od<fraction<complex<Z>, N>> {
        fraction<Z, N> operator () (const fraction<complex<Z>, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct od<fraction<quaternion<Z>, N>> {
        fraction<complex<Z>, N> operator () (const fraction<quaternion<Z>, N> &x);
    };

    template <WholeNumber Z, typename N>
    struct od<fraction<octonion<Z>, N>> {
        fraction<quaternion<Z>, N> operator () (const fraction<octonion<Z>, N> &x);
    };
}

namespace data::math {

    template <typename Z, typename N>
    std::ostream inline &operator << (std::ostream &o, const fraction<Z, N> &x) {
        if (x.Denominator.Value == 1) return o << x.Numerator;
        return o << "(" << x.Numerator << " / " << x.Denominator.Value << ")";
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator + (const fraction<Z> &a, const ZZ &b) {
        return a + fraction<Z> {b};
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator - (const fraction<Z> &a, const ZZ &b) {
        return a - fraction<Z> {b};
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator * (const fraction<Z> &a, const ZZ &b) {
        return a * fraction<Z> {b};
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator / (const fraction<Z> &a, const ZZ &b) {
        return a / fraction<Z> {b};
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator + (const ZZ &a, const fraction<Z> &b) {
        return fraction<Z> {a} + b;
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator - (const ZZ &a, const fraction<Z> &b) {
        return fraction<Z> {a} - b;
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator * (const ZZ &a, const fraction<Z> &b) {
        return fraction<Z> {a} * b;
    }

    template <Ordered Z, typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    constexpr fraction<Z> inline operator / (const ZZ &a, const fraction<Z> &b) {
        return fraction<Z> {a} / b;
    }
}

namespace data::math::def {

    template <Ordered Z, Ordered N>
    constexpr math::sign inline sign<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return x == 0 ? zero : x < fraction<Z, N> {0} ? negative : positive;
    }

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline identity<plus<fraction<Z, N>>, fraction<Z, N>>::operator () () {
        return fraction<Z, N> {identity<plus<Z>, Z>::operator () ()};
    }

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline identity<times<fraction<Z, N>>, fraction<Z, N>>::operator () () {
        return fraction<Z, N> {identity<times<Z>, Z>::operator () ()};
    }

    template <typename Z, typename N> struct inverse<times<fraction<Z, N>>, fraction<Z, N>> {
        constexpr nonzero<fraction<Z, N>> operator () (const nonzero<fraction<Z, N>> &x) const {
            if (x.Value.Numerator == 0) throw division_by_zero {};
            return nonzero {fraction<Z, N> {
                Z (x.Value.Denominator.Value) * static_cast<int> (data::sign (x.Value.Numerator)),
                nonzero {data::abs (x.Value.Numerator)}}};
        }

        constexpr nonzero<fraction<Z, N>> operator () (const nonzero<fraction<Z, N>> &a, const nonzero<fraction<Z, N>> &b) const {
            return nonzero {b.Value / a.Value};
        }
    };

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline inverse<plus<fraction<Z, N>>, fraction<Z, N>>::operator () (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return b - a;
    }

    template <typename Z, typename N> requires requires (const Z &z) {
        { data::abs (z) };
    } constexpr fraction<Z, N> inline abs<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return fraction<Z, N> {Z (data::abs (x.Numerator)), math::nonzero<N> {N (data::abs (x.Denominator.Value))}};
    }

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline conjugate<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return x;
    }

    template <WholeNumber Z, typename N>
    fraction<Z, N> inline ev<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return x;
    }

    template <WholeNumber Z, typename N>
    fraction<Z, N> inline od<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return 0;
    }

    template <WholeNumber Z, typename N>
    fraction<Z, N> inline ev<fraction<complex<Z>, N>>::operator () (const fraction<complex<Z>, N> &x) {
        return math::over<Z, N> (math::ev (data::numerator (x)), data::denominator (x));
    }

    template <WholeNumber Z, typename N>
    fraction<Z, N> inline od<fraction<complex<Z>, N>>::operator () (const fraction<complex<Z>, N> &x) {
        return math::over<Z, N> (math::od (data::numerator (x)), data::denominator (x));
    }

    template <WholeNumber Z, typename N>
    fraction<complex<Z>, N> inline ev<fraction<quaternion<Z>, N>>::operator () (const fraction<quaternion<Z>, N> &x) {
        return math::over<complex<Z>, N> (math::ev (data::numerator (x)), data::denominator (x));
    }

    template <WholeNumber Z, typename N>
    fraction<quaternion<Z>, N> inline ev<fraction<octonion<Z>, N>>::operator () (const fraction<octonion<Z>, N> &x) {
        return math::over<quaternion<Z>, N> (math::ev (data::numerator (x)), data::denominator (x));
    }

    template <WholeNumber Z, typename N>
    fraction<complex<Z>, N> inline od<fraction<quaternion<Z>, N>>::operator () (const fraction<quaternion<Z>, N> &x) {
        return math::over<complex<Z>, N> (math::od (data::numerator (x)), data::denominator (x));
    }

    template <WholeNumber Z, typename N>
    fraction<quaternion<Z>, N> inline od<fraction<octonion<Z>, N>>::operator () (const fraction<octonion<Z>, N> &x) {
        return math::over<quaternion<Z>, N> (math::od (data::numerator (x)), data::denominator (x));
    }

}

namespace data::math {

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline operator - (const fraction<Z, N> &x) {
        auto z = x;
        z.Numerator = -z.Numerator;
        return z;
    }

    template <typename Z, typename N> constexpr fraction<Z, N> inline operator ~ (const fraction<Z, N> &x) {
        return over (Z (x.Denominator.Value), x.Numerator);
    }

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline operator + (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return def::over<Z, N> {} (b.Numerator * a.Denominator.Value + a.Numerator * b.Denominator.Value,
            Z (a.Denominator.Value * b.Denominator.Value));
    }

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline operator - (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a + (-b);
    }

    template <typename Z, typename N>
    constexpr fraction<Z, N> inline operator * (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return fraction<Z> {Z (a.Numerator * b.Numerator), Z (a.Denominator.Value * b.Denominator.Value)};
    }

    template <typename Z, typename N> constexpr fraction<Z, N> operator / (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a * def::inverse<def::times<fraction<Z>>, fraction<Z>> {} (nonzero {b}).Value;
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    constexpr fraction<Z, N> inline &fraction<Z, N>::operator += (const fraction &f) {
        return *this = *this + f;
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    constexpr fraction<Z, N> inline &fraction<Z, N>::operator -= (const fraction &f) {
        return *this = *this - f;
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    constexpr fraction<Z, N> inline &fraction<Z, N>::operator *= (const fraction &f) {
        return *this = *this * f;
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    constexpr fraction<Z, N> inline &fraction<Z, N>::operator /= (const fraction &f) {
        return *this = *this / f;
    }
}
