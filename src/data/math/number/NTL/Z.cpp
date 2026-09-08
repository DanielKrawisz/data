
#include <data/math/number/NTL/Z.hpp>

namespace data::math::number::NTL {

    template <std::unsigned_integral U>
    static ZZ import_bin (
        slice<const U> data,
        // the ordering of the overall array.
        endian::order word_order,
        // the ordering of each value in the array.
        endian::order byte_order,
        arithmetic::negativity neg
    ) {

        const U *op = data.data ();
        size_t count = data.size ();

        if (
            word_order == endian::order::little &&
            neg == arithmetic::negativity::nones &&
            sizeof (U) == sizeof (unsigned char) &&
            (byte_order == endian::order::native || sizeof (unsigned char) == 1)
        ) {
            return ZZFromBytes (reinterpret_cast<const unsigned char *> (op), count);
        }

        if (count == 0) return ZZ ();

        if (word_order != endian::order::big || word_order != endian::order::little)
            throw std::invalid_argument ("invalid word order");

        if (byte_order != endian::order::big || byte_order != endian::order::little)
            throw std::invalid_argument ("invalid byte order");

        const slice<U> input {op, op + count};

        // deal with negative numbers.
        if (neg == arithmetic::negativity::twos) {
            if (word_order == endian::order::big) {
                if (arithmetic::twos::is_negative (arithmetic::Words<endian::order::big, U> {input})) {
                    const auto magnitude =
                        arithmetic::twos::negate<endian::order::big, U> (input);

                    return -import_bin (
                        magnitude.data (),
                        magnitude.size (),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones);
                }
            } else {
                if (arithmetic::twos::is_negative (arithmetic::Words<endian::order::little, U> {input})) {
                    const auto magnitude =
                        arithmetic::twos::negate<endian::order::little, U> (input);

                    return -import_bin (
                        magnitude.data (),
                        magnitude.size (),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones);
                }
            }
        } else if (neg == arithmetic::negativity::BC) {
            if (word_order == endian::order::big) {
                if (arithmetic::BC::is_negative (arithmetic::Words<endian::big, U> {input})) {
                    const auto magnitude =
                        arithmetic::BC::negate<endian::big, U> (input);

                    return -import_bin (
                        magnitude.data (),
                        magnitude.size (),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones
                    );
                }
            } else {
                if (arithmetic::BC::is_negative (arithmetic::Words<endian::little, U> {input})) {
                    const auto magnitude =
                        arithmetic::BC::negate<endian::little, U> (input);

                    return -import_bin (
                        magnitude.data (),
                        magnitude.size (),
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
                reinterpret_cast<const unsigned char *> (op),
                count * sizeof (U));
        }

        bytestring<U> bytes (count);

        if (word_order == endian::order::little) {
            for (size_t i = 0; i < count; ++i)
                bytes[i] = op[i];
        } else {
            for (size_t i = 0; i < count; ++i)
                bytes[i] = op[count - i - 1];
        }

        if (byte_order != endian::order::little && sizeof (U) > 1) {
            for (U &x : bytes)
                x = byteswap (x);
        }

        return ZZFromBytes (
            reinterpret_cast<const unsigned char *> (bytes.data ()),
            count * sizeof (U));
    }

}
