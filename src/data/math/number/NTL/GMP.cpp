// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/NTL/Z.hpp>
#include <rotella/akslib.h>

#include <cstring>
#include <gmpxx.h>
#include <NTL/ZZ_limbs.h>

namespace data::math::number::NTL {

    mpz_class to_mpz (const ZZ &x)
    {
        mpz_class result;

        const long n = x.size();

        if (n != 0) {
            mp_limb_t* p = mpz_limbs_write(result.get_mpz_t(), n);

            std::memcpy(
                p,
                NTL::ZZ_limbs_get(x),
                n * sizeof (mp_limb_t));

            mpz_limbs_finish(
                result.get_mpz_t(),
                ::NTL::sign(x) < 0 ? -n : n
            );
        }

        return result;
    }

    NTL::ZZ to_ZZ (const mpz_class &x)
    {
        NTL::ZZ result;

        const mp_size_t n = mpz_size(x.get_mpz_t());

        if (n != 0) {
            NTL::ZZ_limbs_set(
                result,
                mpz_limbs_read (x.get_mpz_t ()),
                n);

            if (mpz_sgn(x.get_mpz_t()) < 0)
                result = -result;
        }

        return result;
    }

    template <typename F>
    requires std::invocable<F, const mpz_class &>
    auto GMP_apply (const ZZ &x, F &&f)
    {
        mpz_class gx = to_mpz (x);

        using R = std::invoke_result_t<F, const mpz_class &>;

        if constexpr (std::same_as<R, mpz_class>)
            return to_ZZ (std::invoke(std::forward<F> (f), gx));
        else
            return std::invoke (std::forward<F> (f), gx);
    }

    bool AKS_is_prime (const N &z) {
        return GMP_apply (z.Value, [] (const mpz_class &m) {
            return ::aks (m);
        });
    }

    maybe<ZZ> GMP_root (const ZZ &z, uint64 p) {
        mpz_class mpz = to_mpz (z);
        mpz_class root;
        if (mpz_root (root.get_mpz_t (), mpz.get_mpz_t (), p)) return to_ZZ (root);
        return {};
    }

    set<N> roots (const N &n, uint64 p) {
        // one is the only number with a zeroth root.
        if (p == 0) return n == 1 ? set<N> {N {1}} : set<N> {};

        if (p == 1 || n == 0 || n == 1) return set<N> {n};

        maybe<ZZ> zz_root = GMP_root (n.Value, p);
        if (zz_root) return set<N> {N (*zz_root)};
        return {};
    }

    set<Z> roots (const Z &n, uint64 p) {
        if (p == 0) return n == 1 ? set<Z> {n} : set<Z> {};

        if (p == 1 || n == 0 || n == 1) return set<Z> {n};

        if (p % 2 == 0) {
            if (n < 0) return set<Z> {};

            maybe<ZZ> zz_root = GMP_root (n.Value, p);
            if (zz_root) return set<Z> {Z (*zz_root), -Z (*zz_root)};
            return {};
        }

        N abs = data::abs (n);
        maybe<ZZ> zz_root = GMP_root (abs.Value, p);
        if (zz_root) return is_negative (n) ? set<Z> {-Z (*zz_root)}: set<Z> {Z (*zz_root)};
        return {};

    }

}

