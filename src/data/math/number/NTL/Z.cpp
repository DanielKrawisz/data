
#include <data/math/number/NTL/Z.hpp>

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
        division<Z, N> result {};
        NTL::DivRem (result.Quotient.Value, result.Remainder.Value, a.Value, b.Value.Value);

        if (result.Remainder.Value < 0) {
            NTL::ZZ abs_b = NTL::abs (b.Value.Value);
            result.Remainder.Value += abs_b;
            result.Quotient.Value += (b.Value.Value > 0) ? -1 : 1;
        }

        return result;
    }
}

namespace data::math::number::NTL {

    template <std::unsigned_integral U>
    static ZZ import_bin (
        slice<const U> input,
        // the ordering of the overall array.
        endian::order word_order,
        // the ordering of each value in the array.
        endian::order byte_order,
        arithmetic::negativity neg
    ) {

        if (input.size () == 0) return ZZ ();

        if (word_order == endian::order::little &&
            neg == arithmetic::negativity::nones &&
            sizeof (U) == sizeof (unsigned char) &&
            (byte_order == endian::order::native || sizeof (unsigned char) == 1))
            return ZZFromBytes (reinterpret_cast<const unsigned char *> (input.data ()), input.size ());

        if (word_order != endian::order::big || word_order != endian::order::little)
            throw std::invalid_argument ("invalid word order");

        if (byte_order != endian::order::big || byte_order != endian::order::little)
            throw std::invalid_argument ("invalid byte order");

        // deal with negative numbers.
        if (neg == arithmetic::negativity::twos) {
            if (word_order == endian::order::big) {
                if (arithmetic::twos::is_negative (arithmetic::Words<endian::order::big, const U> {input})) {
                    const auto magnitude =
                        arithmetic::twos::negate<endian::order::big, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones);
                }
            } else {
                if (arithmetic::twos::is_negative (arithmetic::Words<endian::order::little, const U> {input})) {
                    const auto magnitude =
                        arithmetic::twos::negate<endian::order::little, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones);
                }
            }
        } else if (neg == arithmetic::negativity::BC) {
            if (word_order == endian::order::big) {
                if (arithmetic::BC::is_negative (arithmetic::Words<endian::big, const U> {input})) {
                    const auto magnitude =
                        arithmetic::BC::negate<endian::big, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones
                    );
                }
            } else {
                if (arithmetic::BC::is_negative (arithmetic::Words<endian::little, const U> {input})) {
                    const auto magnitude =
                        arithmetic::BC::negate<endian::little, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones
                    );
                }
            }
        }

        // If everything is little endian then we can simply cast
        // the array to a byte array.
        if (word_order == endian::order::little &&
            (byte_order == endian::order::little ||
                (sizeof (U) == sizeof (unsigned char) && sizeof (unsigned char) == 1))) {
            return ZZFromBytes (
                reinterpret_cast<const unsigned char *> (input.data ()),
                input.size () * sizeof (U));
        }

        bytestring<U> bytes (input.size ());

        if (word_order == endian::order::little) {
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[i];
        } else {
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[input.size () - i - 1];
        }

        if (byte_order != endian::order::little && sizeof (U) > 1) {
            for (U &x : bytes)
                x = boost::endian::endian_reverse<U> (x);
        }

        return ZZFromBytes (
            reinterpret_cast<const unsigned char *> (bytes.data ()),
            bytes.size () * sizeof (U));
    }

}
