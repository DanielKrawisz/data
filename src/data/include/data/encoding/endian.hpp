// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <boost/endian.hpp>

#include <data/stream.hpp>
#include <data/size.hpp>
#include <data/norm.hpp>
#include <data/divide.hpp>
#include <data/increment.hpp>
#include <data/math/infinite.hpp>

namespace data::endian {
    
    using order = boost::endian::order;
    constexpr order big = boost::endian::order::big;
    constexpr order little = boost::endian::order::little;
    
    constexpr inline order opposite (order o) {
        return o == big ? little : big;
    } 
    
    // convert native to and from the given endian ordering. 
    template <typename X, order> struct native;
    
    template <typename X> struct native<X, big> {
        static X from (const X x) {
            return boost::endian::native_to_big<X> (x);
        }
        
        static X to (const X x) {
            return boost::endian::big_to_native<X> (x);
        }
    };
    
    template <typename X> struct native<X, little> {
        static X from (const X x) {
            return boost::endian::native_to_little<X> (x);
        }
        
        static X to (const X x) {
            return boost::endian::little_to_native<X> (x);
        }
    };
    
    template <bool is_signed, std::size_t bytes> struct native_representation;
    
    template <bool is_signed, std::size_t bytes> using to_native = native_representation<is_signed, bytes>::type;
    
    template <> struct native_representation<true, 1> {
        using type = int_least8_t;
    };
    
    template <> struct native_representation<true, 2> {
        using type = int_least16_t;
    };
    
    template <> struct native_representation<true, 3> {
        using type = int_least32_t;
    };
    
    template <> struct native_representation<true, 4> {
        using type = int_least32_t;
    };
    
    template <> struct native_representation<true, 5> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 6> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 7> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 8> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<false, 1>{
        using type = uint_least8_t;
    };
    
    template <> struct native_representation<false, 2>{
        using type = uint_least16_t;
    };
    
    template <> struct native_representation<false, 3> {
        using type = uint_least32_t;
    };
    
    template <> struct native_representation<false, 4> {
        using type = uint_least32_t;
    };
    
    template <> struct native_representation<false, 5> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 6> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 7> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 8> {
        using type = uint_least64_t;
    };

    template <bool is_signed, order r, size_t size>
    using integral = boost::endian::endian_arithmetic<r, to_native<is_signed, size>, 8 * size, boost::endian::align::no>;
    
}

namespace boost::endian {

    template <order Order, class T, std::size_t n_bits, align Align>
    data::writer<data::byte> inline &operator << (data::writer<data::byte> &w, endian_arithmetic<Order, T, n_bits, Align> x) {
        w.write (x.data (), n_bits / 8);
        return w;
    }

    template <order Order, class T, std::size_t n_bits, align Align>
    data::reader<data::byte> inline &operator >> (data::reader<data::byte> &r, endian_arithmetic<Order, T, n_bits, Align> &x) {
        r.read (x.data (), n_bits / 8);
        return r;
    }
}

namespace data::meta {

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct size<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr size_t operator () (boost::endian::endian_arithmetic<Order, T, n_bits, Align>) {
            return n_bits / 8;
        }
    };
}

namespace data::math::number {

    template <endian::order Order, std::unsigned_integral T, std::size_t n_bits, boost::endian::align Align>
    struct increment<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &x) const {
            return nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
                static_cast<T> (x.value () + 1)
            };
        }
    };

    template <endian::order Order, std::unsigned_integral T, std::size_t n_bits, boost::endian::align Align>
    struct decrement<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> &x) const {
            return x.Value.value () - 1;
        }

        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &x) const {
            T n = x.value ();
            if (n == 0) return n;
            return n - 1;
        }
    };

    template <endian::order Order, std::signed_integral T, std::size_t n_bits, boost::endian::align Align>
    struct increment<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &z) const {
            return z.value () + 1;
        }
    };

    template <endian::order Order, std::signed_integral T, std::size_t n_bits, boost::endian::align Align>
    struct decrement<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &z) const {
            return z.value () - 1;
        }
    };
}

namespace data::math {

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct numeric_limits<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr static boost::endian::endian_arithmetic<Order, T, n_bits, Align> Max {std::numeric_limits<T>::max ()};
        constexpr static boost::endian::endian_arithmetic<Order, T, n_bits, Align> Min {std::numeric_limits<T>::min ()};

        constexpr static boost::endian::endian_arithmetic<Order, T, n_bits, Align> &max () {
            return Max;
        }

        constexpr static boost::endian::endian_arithmetic<Order, T, n_bits, Align> &min () {
            return Min;
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct abs<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &x) {
            return data::abs (x.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct negate<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &x) {
            return data::negate (x.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct plus<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> a,
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> b) {
            return data::plus (a.value (), b.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct minus<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> a,
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> b) {
            return data::minus (a.value (), b.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct inverse<plus<boost::endian::endian_arithmetic<Order, T, n_bits, Align>>, boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &a,
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &b) {
            return data::minus (b.value (), a.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct times<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &a,
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &b) {
            return a.value () * b.value ();
        }

        constexpr nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> operator () (
            const nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> &a,
            const nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> &b) {
            return nonzero<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {a.Value.value () * b.Value.value ()};
        }
    };

    template <endian::order Order, std::unsigned_integral T, std::size_t n_bits, boost::endian::align Align>
    struct divide<boost::endian::endian_arithmetic<Order, T, n_bits, Align>, boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        using ue = boost::endian::endian_arithmetic<Order, T, n_bits, Align>;
        constexpr division<ue, ue> operator () (const ue &dividend, const nonzero<ue> &divisor) {
            return {ue (dividend.value () / divisor.Value.value ()),
                ue (dividend.value () % divisor.Value.value ())};
        }
    };

    template <endian::order Order, std::signed_integral T, std::unsigned_integral U, std::size_t n_bits, boost::endian::align Align>
    struct divide<boost::endian::endian_arithmetic<Order, T, n_bits, Align>, boost::endian::endian_arithmetic<Order, U, n_bits, Align>> {
        using se = boost::endian::endian_arithmetic<Order, T, n_bits, Align>;
        using ue = boost::endian::endian_arithmetic<Order, U, n_bits, Align>;
        constexpr division<se, ue> operator () (const se &dividend, const nonzero<ue> &divisor) {
            auto d = divide<T, U> {} (dividend.value (), nonzero<U> {divisor.Value.value ()});
            return division<se, ue> {se (d.Quotient), ue (d.Remainder)};
        }
    };

    template <endian::order Order, std::signed_integral T, std::size_t n_bits, boost::endian::align Align>
    struct divide<boost::endian::endian_arithmetic<Order, T, n_bits, Align>, boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        using se = boost::endian::endian_arithmetic<Order, T, n_bits, Align>;
        using ue = boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align>;
        constexpr division<se, ue> operator () (const se &dividend, const nonzero<se> &divisor) {
            auto d = divide<T, T> {} (dividend.value (), nonzero<T> {divisor.Value.value ()});
            return division<se, ue> {se (d.Quotient), ue (d.Remainder)};
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct negate_mod<boost::endian::endian_arithmetic<Order, T, n_bits, Align>,
        boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> a,
            nonzero<boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align>> n) {
            return data::negate_mod (a.value (), nonzero {n.Value.value ()});
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct square_mod<boost::endian::endian_arithmetic<Order, T, n_bits, Align>,
    boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> a,
            nonzero<boost::endian::endian_arithmetic<Order, std::make_unsigned_t<T>, n_bits, Align>> n) {
            return data::square_mod (a.value (), nonzero {n.Value.value ()});
        }
    };

    template <endian::order Order, class u, class w, class v, std::size_t n_bits, boost::endian::align Align>
    struct times_mod<boost::endian::endian_arithmetic<Order, u, n_bits, Align>,
        boost::endian::endian_arithmetic<Order, w, n_bits, Align>,
        boost::endian::endian_arithmetic<Order, v, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order,
            decltype (data::times_mod (std::declval<u> (), std::declval<w> (), std::declval<nonzero<v>> ())),
            n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, u, n_bits, Align> a,
            boost::endian::endian_arithmetic<Order, w, n_bits, Align> b,
            nonzero<boost::endian::endian_arithmetic<Order, v, n_bits, Align>> n) {
            return data::times_mod (a.value (), b.value (), nonzero {n.Value.value ()});
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct bit_not<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &x) {
            return static_cast<T> (~x.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct bit_and<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> a,
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> b) {
            return data::bit_and (a.value (), b.value ());
        }
    };

    template <endian::order Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct bit_xor<boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        constexpr boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> a,
            boost::endian::endian_arithmetic<Order, T, n_bits, Align> b) {
            return data::bit_xor (a.value (), b.value ());
        }
    };
}

namespace data {

    // big endian signed integer unaligned types
    typedef endian::integral<true, endian::big, 1>           int8_big;
    typedef endian::integral<true, endian::big, 2>          int16_big;
    typedef endian::integral<true, endian::big, 3>          int24_big;
    typedef endian::integral<true, endian::big, 4>          int32_big;
    typedef endian::integral<true, endian::big, 5>          int40_big;
    typedef endian::integral<true, endian::big, 6>          int48_big;
    typedef endian::integral<true, endian::big, 7>          int56_big;
    typedef endian::integral<true, endian::big, 8>          int64_big;

    // big endian unsigned integer unaligned types
    typedef endian::integral<false, endian::big, 1>         uint8_big;
    typedef endian::integral<false, endian::big, 2>        uint16_big;
    typedef endian::integral<false, endian::big, 3>        uint24_big;
    typedef endian::integral<false, endian::big, 4>        uint32_big;
    typedef endian::integral<false, endian::big, 5>        uint40_big;
    typedef endian::integral<false, endian::big, 6>        uint48_big;
    typedef endian::integral<false, endian::big, 7>        uint56_big;
    typedef endian::integral<false, endian::big, 8>        uint64_big;

    // little endian signed integer unaligned types
    typedef endian::integral<true, endian::little, 1>     int8_little;
    typedef endian::integral<true, endian::little, 2>    int16_little;
    typedef endian::integral<true, endian::little, 3>    int24_little;
    typedef endian::integral<true, endian::little, 4>    int32_little;
    typedef endian::integral<true, endian::little, 5>    int40_little;
    typedef endian::integral<true, endian::little, 6>    int48_little;
    typedef endian::integral<true, endian::little, 7>    int56_little;
    typedef endian::integral<true, endian::little, 8>    int64_little;

    // little endian unsigned integer unaligned types
    typedef endian::integral<false, endian::little, 1>   uint8_little;
    typedef endian::integral<false, endian::little, 2>  uint16_little;
    typedef endian::integral<false, endian::little, 3>  uint24_little;
    typedef endian::integral<false, endian::little, 4>  uint32_little;
    typedef endian::integral<false, endian::little, 5>  uint40_little;
    typedef endian::integral<false, endian::little, 6>  uint48_little;
    typedef endian::integral<false, endian::little, 7>  uint56_little;
    typedef endian::integral<false, endian::little, 8>  uint64_little;
}

#endif

