
#include <data/math/number/NTL/Z.hpp>

namespace NTL {

    void conv (ZZ &x, const data::encoding::signed_decimal::string &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid decimal string";
        if (u[0] == '-') x = -conv<NTL::ZZ> (u.c_str () + 1);
        else conv (x, u.c_str ());
    }

    void conv (data::encoding::signed_decimal::string &x, const ZZ &u) {
        x = data::encoding::signed_decimal::write (data::math::number::Z (u));
    }

    void conv (data::encoding::decimal::string &x, const ZZ &u) {
        x = data::encoding::decimal::write (data::math::number::N (u));
    }

    void conv (ZZ &x, const data::encoding::base58::string &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid base58 string";
        x = data::encoding::base58::decode<data::Z> (u)->Value;
    }

    void conv (data::encoding::base58::string &x, const ZZ &u) {
        x = data::encoding::base58::encode (data::math::number::Z (u));
    }

    ZZ pow_mod (const ZZ &base, const ZZ &exp, const ZZ &mod) {
        if (mod == 1) return ZZ (0);
        if (base < 0 || base > mod) {

            NTL::ZZ modded = base % mod;

            if (modded < 0) modded += mod;

            return PowerMod (modded, exp, mod);
        }

        return PowerMod (base, exp, mod);
    }
}
