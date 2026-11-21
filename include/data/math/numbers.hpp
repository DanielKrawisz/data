// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/integral.hpp>
#include <data/math/number/NTL/Z.hpp>
#include <data/math/number/bounded.hpp>
#include <data/math/number/bytes.hpp>

#if DATA_CRYPTO
#include <cryptopp/Z.hpp>
#include <openssl/Z.hpp>
#endif

namespace data::math::def {
#if DATA_CRYPTO
    CryptoPP::Z inline convert<CryptoPP::Z, OpenSSL::Z>::operator () (const OpenSSL::Z &z) const {
        return CryptoPP::Z (Z_bytes_big (z));
    }

    OpenSSL::Z inline convert<OpenSSL::Z, CryptoPP::Z>::operator () (const CryptoPP::Z &z) const {
        return OpenSSL::Z (Z_bytes_big (z));
    }
#endif

    // default definition of pow_mod
    template <typename Base, GroupNumber Exp, GroupNumber Mod> struct pow_mod<Base, Exp, Mod> {
        constexpr Mod operator () (const Base &x, const Exp &y, const nonzero<Mod> &z) {

            if consteval {

                // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
                // the result. We transform to a type that is bigger than this and then transform back.

                using innerBase = doubled<Base>;
                using innerExp = doubled<Exp>;
                using innerMod = doubled<Mod>;

                return math::convert<Mod> (
                    math::binary_accumulate_pow_mod (
                        math::convert<innerBase> (x),
                        math::convert<innerExp> (y),
                        math::nonzero {math::convert<innerMod> (z.Value)}));
            } else {
#if DATA_CRYPTO
                auto a = math::convert<OpenSSL::Z> (x);
                auto b = math::convert<OpenSSL::Z> (y);
                auto c = math::convert<OpenSSL::Z> (z.Value);

                auto r = OpenSSL::pow_mod (a, b, c);

                return math::convert<Mod> (r);
#else
                auto a = math::convert<Z> (x);
                auto b = math::convert<Z> (y);
                auto c = math::convert<Z> (z.Value);

                auto r = Z {NTL::pow_mod (a.Value, b.Value, c.Value)};

                return math::convert<Mod> (r);
#endif
            }

        }
    };
}

