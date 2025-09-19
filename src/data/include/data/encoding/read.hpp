// Copyright (c) 2015 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_READ
#define DATA_ENCODING_READ

#include <data/concepts.hpp>
#include <data/encoding/endian.hpp>
#include <data/maybe.hpp>
#include <data/encoding/base64.hpp>

namespace data::encoding {

    // read values from a serialized format.
    template <typename X, typename ...context> struct read;

    // this says that by default, context doesn't matter.
    template <typename X, typename context, typename ...contexts> struct read<X, context, contexts...> : read<X, contexts...> {};

    // The default read function.
    template <std::default_initializable X> struct read<X> {
        maybe<X> operator () (string_view z) const {
            X x;
            if constexpr (std::integral<X>) {
                auto [ptr, ec] = std::from_chars (z.data (), z.data () + z.size (), x);
                if (ec == std::errc {}) return x;
            } else if constexpr (requires (std::istream &ss) {
                { ss >> x };
            }) {
                std::stringstream ss {std::string {z}};
                ss >> x;
                if (bool (ss)) return x;
            } else if constexpr (requires () {
                { X {z} };
            }) {
                try {
                    return X {z};
                } catch (...) {}
            } 
            return {};
        }
    };

    template <> struct read<bool> {
        maybe<bool> operator () (string_view x) const {
            if (x == "0") return false;
            if (x == "1") return true;
            string lower = to_lower (std::string {x});
            if (lower == "true") return true;
            if (lower == "false") return false;
            if (lower == "yes") return true;
            if (lower == "no") return false;
            if (lower == "on") return true;
            if (lower == "off") return false;
            return {};
        }
    };

    // not sure why this is necessary, but I can't get the default >> to error. 
    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct read<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        maybe<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> operator () (string_view z) const {
            auto x = read<T> {} (z);
            if (!bool (x)) return {};
            return {boost::endian::endian_arithmetic<Order, T, n_bits, Align> {*x}};
        }
    };

    // assume bytes is encoded in base 64 by default.
    template <> struct read<bytes> {
        maybe<bytes> operator () (string_view x) const {
            return base64::read (x);
        }
    };
}

#endif
