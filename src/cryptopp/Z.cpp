#include <cryptopp/Z.hpp>

namespace CryptoPP {

    Integer import_dec (const dec_int &x) {
        return CryptoPP::Integer {x.c_str ()};
    }

    dec_int export_dec (const Integer &x) {
        return dec_int {CryptoPP::IntToString (x, 10)};
    }

    std::strong_ordering operator <=> (const Z &a, const Z &b) {
        auto c = static_cast<const CryptoPP::Integer &> (a).Compare (b);

        if (c < 0) return std::strong_ordering::less;
        if (c > 0) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

}

namespace data::math::def {

    CryptoPP::Z abs<CryptoPP::Z>::operator () (const CryptoPP::Z &x) {
        return x.IsNegative () ? -x : x;
    }

    size_t size_in_base<CryptoPP::Z, 2>::operator () (const CryptoPP::Z &x) {
        if (x.IsNegative ())
            return (-x - 1).BitCount () + 1;

        return x.BitCount () + 1;
    }

    CryptoPP::Z pow_mod<CryptoPP::Z, CryptoPP::Z, CryptoPP::Z>::operator () (
        const CryptoPP::Z &x,
        const CryptoPP::Z &y,
        const nonzero<CryptoPP::Z> &m
    ) {
        auto modulus = data::abs (m.Value);

        if (!modulus)
            throw std::domain_error {"zero modulus"};

        auto result = a_exp_b_mod_c (x, y, modulus);

        if (result.IsNegative ())
            result += modulus;

        return CryptoPP::Z {std::move (result)};
    }

}
