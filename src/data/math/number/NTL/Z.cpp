
#include <data/math/number/NTL/Z.hpp>
#include <data/encoding/integer.hpp>

namespace NTL {

    void conv (ZZ &x, const data::encoding::signed_decimal::string &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid decimal string";
        if (u[0] == '-') x = -conv<NTL::ZZ> (u.c_str () + 1);
        else conv (x, u.c_str ());
    }

    void conv (ZZ &x, const data::encoding::base58::string &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid base58 string";
        x = data::encoding::base58::decode<data::Z> (u)->Value;
    }
}

namespace data::math::def {
    division<Z, N> divmod<Z, Z>::operator () (const Z &a, const nonzero<Z> &b) {
        if (b.Value == 0) throw math::division_by_zero {};
        division<Z, N> result {};
        NTL::DivRem (result.Quotient.Value, result.Remainder.Value, a.Value, b.Value.Value);

        if (result.Remainder.Value < 0) {
            NTL::ZZ abs_b = NTL::abs (b.Value.Value);
            result.Remainder.Value += abs_b;
            result.Quotient.Value += 1;
        }

        return result;
    }
}

namespace data::math::number {

    std::strong_ordering operator <=> (const Z &a, const Z &b) {
        switch (NTL::compare(a.Value, b.Value)) {
            case -1: return std::strong_ordering::less;
            case  1: return std::strong_ordering::greater;
            default: return std::strong_ordering::equal;
        }
    }

    std::strong_ordering operator <=> (const N &a, const N &b) {
        switch (NTL::compare(a.Value, b.Value)) {
            case -1: return std::strong_ordering::less;
            case  1: return std::strong_ordering::greater;
            default: return std::strong_ordering::equal;
        }
    }

    std::ostream &operator << (std::ostream &o, const N &n) {
        if (o.flags () & std::ios::hex) {
            encoding::hexidecimal::write (o, n);
            return o;
        }

        if (o.flags () & std::ios::dec) {
            encoding::decimal::write (o, n);
            return o;
        }

        throw exception {} << "invalid encoding";
    }

    std::ostream &operator << (std::ostream &o, const Z &n) {
        if (o.flags () & std::ios::hex) {
            encoding::hexidecimal::write (o, n);
            return o;
        }

        if (o.flags () & std::ios::dec) {
            encoding::signed_decimal::write (o, n);
            return o;
        }

        throw exception {} << "invalid encoding";
    }

    std::istream &operator >> (std::istream &i, Z &z) {
        encoding::integer::string x;
        i >> x;
        if (i) z = Z::read (x);
        return i;
    }

    std::istream &operator >> (std::istream &i, N &n) {
        encoding::natural::string x;
        i >> x;
        if (i) n = N::read (x);
        return i;
    }

    // for these next 4 functions, we already know that the string is valid.
    Z inline Z_read_dec (string_view x) {
        if (x[0] == '-') return -Z (NTL::conv<NTL::ZZ> (std::string {x}.c_str () + 1));
        return Z (NTL::conv<NTL::ZZ> (std::string {x}.c_str ()));
    }

    Z inline Z_read_hex (string_view x) {
        return Z (NTL::import_bin<byte> (byte_slice (*encoding::hex::read (x.substr (2))),
            endian::order::little, endian::order::native, arithmetic::negativity::twos));
    }

    N inline N_read_hex (string_view x) {
        return N (NTL::import_bin<byte> (byte_slice (*encoding::hex::read (x.substr (2))),
            endian::order::little, endian::order::native, arithmetic::negativity::nones));
    }

    N inline N_read_dec (string_view x) {
        return N (NTL::conv<NTL::ZZ> (std::string {x}.c_str ()));
    }

    Z Z::read (string_view s) {
        if (!encoding::integer::valid (s)) throw exception {} << "invalid number string " << s;
        if (encoding::hexidecimal::valid (s)) return Z_read_hex (s);
        return Z_read_dec (s);
    }

    N N::read (string_view x) {
        if (!encoding::natural::valid (x)) throw exception {} << "invalid number string \"" << x << "\"";
        if (encoding::hexidecimal::valid (x)) return N_read_hex (x);
        return N_read_dec (x);
    }

    // ZZ only does bit operations on positive numbers, so we
    // take more general bit operations and reduce them to
    // operations on positive numbers based on various identities.
    Z operator & (const Z &a, const Z &b) {
        bool a_is_non_negative = !is_negative (a);
        bool b_is_non_negative = !is_negative (b);
        if (a_is_non_negative && b_is_non_negative) return Z (a.Value & b.Value);
        if (!a_is_non_negative && !b_is_non_negative) return ~(~a | ~b);
        if (!b_is_non_negative) return a - (a & ~b);
        return b - (~a & b);
    }

    Z operator | (const Z &a, const Z &b) {
        bool a_is_non_negative = !is_negative (a);
        bool b_is_non_negative = !is_negative (b);
        if (a_is_non_negative && b_is_non_negative) return Z (a.Value | b.Value);
        return ~(~a & ~b);
    }

    Z operator ^ (const Z &a, const Z &b) {
        bool a_is_non_negative = !is_negative (a);
        bool b_is_non_negative = !is_negative (b);
        if (a_is_non_negative && b_is_non_negative) return Z (a.Value ^ b.Value);
        return (~a & b) | (a & ~b);
    }
}

namespace data::encoding::decimal {

    string write (const N &x) {
        std::stringstream ss;
        ss << x.Value;
        return string {ss.str ()};
    }

}

namespace data::encoding::signed_decimal {

    string write (const Z &x) {
        std::stringstream ss;
        ss << x.Value;
        return string {ss.str ()};
    }
}

namespace data::encoding::hexidecimal {

    std::ostream &write (std::ostream &o, const math::number::Z &zz, hex::letter_case x, neg n) {
        switch (n) {
            case neg::twos:{
                auto z = math::number::Z_bytes<endian::little, neg::twos, byte> (zz);
                return write<endian::little, byte> (o, static_cast<const oriented<endian::little, byte> &> (z), x);
            }
            case neg::BC: {
                auto z = math::number::Z_bytes<endian::little, neg::BC, byte> (zz);
                return write<endian::little, byte> (o, static_cast<const oriented<endian::little, byte> &> (z), x);
            }
            default: {
                auto n = math::number::N_bytes<endian::little, byte> (N (zz));
                return write<endian::little, byte> (o, static_cast<const oriented<endian::little, byte> &> (n), x);
            }
        }
    }
}

