// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC_ENDIAN
#define DATA_ARITHMETIC_ENDIAN

#include <boost/endian/arithmetic.hpp>

#include <data/stream.hpp>
#include <data/increment.hpp>
#include <data/divide.hpp>
#include <data/arithmetic/halves.hpp>
#include <data/arithmetic/words.hpp>
#include <data/math/infinite.hpp>

namespace data {

    template <bool is_signed, boost::endian::order Order, std::size_t bytes> struct endian_integral;

    // big endian signed integer unaligned types
    typedef endian_integral<true, endian::big, 1>           int8_big;
    typedef endian_integral<true, endian::big, 2>          int16_big;
    typedef endian_integral<true, endian::big, 3>          int24_big;
    typedef endian_integral<true, endian::big, 4>          int32_big;
    typedef endian_integral<true, endian::big, 5>          int40_big;
    typedef endian_integral<true, endian::big, 6>          int48_big;
    typedef endian_integral<true, endian::big, 7>          int56_big;
    typedef endian_integral<true, endian::big, 8>          int64_big;

    // big endian unsigned integer unaligned types
    typedef endian_integral<false, endian::big, 1>         uint8_big;
    typedef endian_integral<false, endian::big, 2>        uint16_big;
    typedef endian_integral<false, endian::big, 3>        uint24_big;
    typedef endian_integral<false, endian::big, 4>        uint32_big;
    typedef endian_integral<false, endian::big, 5>        uint40_big;
    typedef endian_integral<false, endian::big, 6>        uint48_big;
    typedef endian_integral<false, endian::big, 7>        uint56_big;
    typedef endian_integral<false, endian::big, 8>        uint64_big;

    // little endian signed integer unaligned types
    typedef endian_integral<true, endian::little, 1>     int8_little;
    typedef endian_integral<true, endian::little, 2>    int16_little;
    typedef endian_integral<true, endian::little, 3>    int24_little;
    typedef endian_integral<true, endian::little, 4>    int32_little;
    typedef endian_integral<true, endian::little, 5>    int40_little;
    typedef endian_integral<true, endian::little, 6>    int48_little;
    typedef endian_integral<true, endian::little, 7>    int56_little;
    typedef endian_integral<true, endian::little, 8>    int64_little;

    // little endian unsigned integer unaligned types
    typedef endian_integral<false, endian::little, 1>   uint8_little;
    typedef endian_integral<false, endian::little, 2>  uint16_little;
    typedef endian_integral<false, endian::little, 3>  uint24_little;
    typedef endian_integral<false, endian::little, 4>  uint32_little;
    typedef endian_integral<false, endian::little, 5>  uint40_little;
    typedef endian_integral<false, endian::little, 6>  uint48_little;
    typedef endian_integral<false, endian::little, 7>  uint56_little;
    typedef endian_integral<false, endian::little, 8>  uint64_little;

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> operator << (const endian_integral<z, o, s> &, int);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> operator >> (const endian_integral<z, o, s> &, int);

    // arithmetic
    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<true, o, s> operator - (const endian_integral<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<true, o, s> operator % (const endian_integral<z, o, s> &, const endian_integral<true, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> operator &= (const endian_integral<z, o, s> &, const endian_integral<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> operator |= (const endian_integral<z, o, s> &, const endian_integral<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> operator ^= (const endian_integral<z, o, s> &, const endian_integral<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> &operator &= (endian_integral<z, o, s> &, const endian_integral<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<false, o, s> &operator %= (endian_integral<z, o, s> &, uint64);

    template <bool z, boost::endian::order o, std::size_t s>
    writer<byte> &operator << (writer<byte> &w, endian_integral<z, o, s> x);

    template <bool z, boost::endian::order o, std::size_t s>
    reader<byte> &operator >> (reader<byte> &r, endian_integral<z, o, s> &x);
    
    template <bool is_signed, boost::endian::order Order, std::size_t bytes>
    struct endian_integral : boost::endian::endian_arithmetic<Order, endian::to_native<is_signed, bytes>, 8 * bytes, boost::endian::align::no> {
        
        using native_type = endian::to_native<is_signed, bytes>;
        using boost_arith = boost::endian::endian_arithmetic<Order, native_type, 8 * bytes, boost::endian::align::no>;
        using boost_arith::data;
        // TODO these constructors should be constexpr
        using boost_arith::boost_arith;
        using iterator = byte *;
        using const_iterator = byte const *;
        
        // TODO these constructors should be constexpr
        endian_integral (const boost_arith &x);
        endian_integral (boost_arith &&x);
        
        iterator begin ();
        const_iterator begin () const;
        iterator end ();
        const_iterator end () const;
        
        explicit operator bytes_view () const;
        explicit operator slice<byte> ();
        
        using opposite_endian = endian_integral<is_signed, endian::opposite (Order), bytes>;
        
        explicit operator opposite_endian () const {
            return opposite_endian {native_type ()};
        }
        
        explicit endian_integral (const opposite_endian &x) : boost_arith {native_type (x)} {}

        endian_integral &operator ++ ();
        endian_integral &operator -- ();
        endian_integral operator ++ (int);
        endian_integral operator -- (int);

        endian_integral &operator += (endian_integral);
        endian_integral &operator -= (endian_integral);
        endian_integral &operator *= (endian_integral);
        endian_integral &operator /= (endian_integral);

        endian_integral &operator += (native_type);
        endian_integral &operator -= (native_type);
        endian_integral &operator *= (native_type);
        endian_integral &operator /= (native_type);

        endian_integral &operator &= (native_type);
        endian_integral &operator |= (native_type);

        endian_integral &operator <<= (int);
        endian_integral &operator >>= (int);

        size_t size () const {
            return bytes;
        }

        const byte &operator [] (int i) const {
            if (i >= int (bytes))
                throw std::out_of_range {std::string {"access index "} + std::to_string (i) +
                    " in arithmetic number of size " + std::to_string (bytes)};
            if (i < 0) return operator [] (bytes + i);
            return data () [i];
        }

        using words_type = arithmetic::Words<Order, byte>;

        words_type words () {
            return words_type {slice<byte> (this->data (), bytes)};
        }

        const words_type words () const {
            return words_type {slice<byte> (const_cast<byte*> (this->data ()), bytes)};
        }
    };
    
}

namespace data::encoding {
    
    template <bool z, endian::order o, size_t size> struct count_digits<endian_integral<z, o, size>> {
        constexpr static bool is_signed = z;
        constexpr static size_t value = size;
    };
    
    template <bool is_signed, endian::order o, size_t size> struct half_of<endian_integral<is_signed, o, size>> {
        using type = endian_integral<is_signed, o, count_digits<typename half_of<endian::to_native<is_signed, size>>::type>::value>;
        static type greater_half (endian_integral<is_signed, o, size> u) {
            return type {half_of<endian::to_native<is_signed, size>>::greater_half ((endian::to_native<is_signed, size>) (u))};
        }
        
        static type lesser_half (endian_integral<is_signed, o, size> u) {
            return type {half_of<endian::to_native<is_signed, size>>::lesser_half ((endian::to_native<is_signed, size>) (u))};
        }
    };
    
    template <bool is_signed, endian::order o, size_t size> struct twice<endian_integral<is_signed, o, size>> {
        using type = endian_integral<is_signed, o, 2 * size>;
        static type extend (endian_integral<is_signed, o, size> x) {
            return (typename twice<endian::to_native<is_signed, size>>::type) (x);
        }
    };

}

namespace data::math::number {
    template <boost::endian::order o, std::size_t z>
    struct increment<endian_integral<false, o, z>> {
        nonzero<endian_integral<false, o, z>> operator () (const endian_integral<false, o, z> &u) {
            nonzero<endian_integral<false, o, z>> x {u};
            x.Value++;
            return x;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct increment<endian_integral<true, o, z>> {
        endian_integral<true, o, z> operator () (const endian_integral<true, o, z> &u) {
            auto x = u;
            return ++x;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct decrement<endian_integral<false, o, z>> {
        endian_integral<false, o, z> operator () (const nonzero<endian_integral<false, o, z>> &u) {
            auto x = u.Value;
            return --x;
        }

        endian_integral<false, o, z> operator () (const endian_integral<false, o, z> &u) {
            if (data::is_zero (u)) return u;
            auto x = u;
            return --x;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct decrement<endian_integral<true, o, z>> {
        endian_integral<true, o, z> operator () (const endian_integral<true, o, z> &u) {
            auto x = u;
            return --x;
        }
    };
}

namespace data::math {

    template <bool x, boost::endian::order o, std::size_t z>
    struct numeric_limits<endian_integral<x, o, z>> {
        constexpr static endian_integral<x, o, z> Max {std::numeric_limits<endian::to_native<x, z>>::max ()};
        constexpr static endian_integral<x, o, z> Min {std::numeric_limits<endian::to_native<x, z>>::min ()};

        constexpr static endian_integral<x, o, z> &max () {
            return Max;
        }

        constexpr static endian_integral<x, o, z> &min () {
            return Min;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct abs<endian_integral<false, o, z>> {
        endian_integral<false, o, z> operator () (const endian_integral<false, o, z> &x) {
            return x;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct abs<endian_integral<true, o, z>> {
        endian_integral<false, o, z> operator () (const endian_integral<true, o, z> &x) {
            return endian_integral<false, o, z> (data::abs ((typename endian_integral<true, o, z>::native_type) (x)));
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct negate<endian_integral<false, o, z>> {
        endian_integral<true, o, z> operator () (const endian_integral<false, o, z> &x);
    };

    template <boost::endian::order o, std::size_t z>
    struct negate<endian_integral<true, o, z>> {
        endian_integral<true, o, z> operator () (const endian_integral<true, o, z> &x);
    };

    template <endian::order r, size_t x>
    struct inverse<plus<endian_integral<true, r, x>>, endian_integral<true, r, x>> {
        endian_integral<true, r, x> operator () (
            const endian_integral<true, r, x> &a,
            const endian_integral<true, r, x> &b) {
            return b - a;
        }
    };

    template <bool z, boost::endian::order o, std::size_t n>
    struct times<endian_integral<z, o, n>> {
        endian_integral<z, o, n> operator () (
            const endian_integral<z, o, n> &a,
            const endian_integral<z, o, n> &b) {
            return a * b;
        }

        nonzero<endian_integral<z, o, n>> operator ()
        (const nonzero<endian_integral<z, o, n>> &a, const nonzero<endian_integral<z, o, n>> &b) {
            return nonzero<endian_integral<z, o, n>> {a.Value * b.Value};
        }
    };

    template <endian::order r, size_t x>
    struct divide<endian_integral<false, r, x>, endian_integral<false, r, x>> {
        using ue = endian_integral<false, r, x>;
        using uen = ue::native_type;
        division<ue, ue> operator () (const ue &dividend, const nonzero<ue> &divisor) {
            return {static_cast<ue> (static_cast<uen> (dividend) / static_cast<uen> (divisor.Value)),
                static_cast<ue> (static_cast<uen> (dividend) % static_cast<uen> (divisor.Value))};
        }
    };

    template <endian::order r, size_t x>
    struct divide<endian_integral<true, r, x>, endian_integral<false, r, x>> {
        using se = endian_integral<true, r, x>;
        using ue = endian_integral<false, r, x>;
        using sen = se::native_type;
        using uen = ue::native_type;
        division<se, ue> operator () (const se &dividend, const nonzero<ue> &divisor) {
            auto d = divide<sen, uen> {} (static_cast<sen> (dividend), nonzero<uen> {static_cast<uen> (divisor.Value)});
            return division<se, ue> {se (d.Quotient), ue (d.Remainder)};
        }
    };

    template <endian::order r, size_t x>
    struct divide<endian_integral<true, r, x>, endian_integral<true, r, x>> {
        using se = endian_integral<true, r, x>;
        using ue = endian_integral<false, r, x>;
        using sen = se::native_type;
        using uen = ue::native_type;
        division<se, ue> operator () (const se &dividend, const nonzero<se> &divisor) {
            auto d = divide<sen, sen> {} (static_cast<sen> (dividend), nonzero<sen> {static_cast<sen> (divisor.Value)});
            return division<se, ue> {se (d.Quotient), ue (d.Remainder)};
        }
    };

}

namespace data {

    template <bool z, boost::endian::order o, std::size_t n>
    endian_integral<z, o, n>::endian_integral (const boost_arith &x) : boost_arith (x) {}

    template <bool z, boost::endian::order o, std::size_t n>
    endian_integral<z, o, n>::endian_integral (boost_arith &&x) : boost_arith (x) {}

    template <bool z, boost::endian::order o, std::size_t n>
    endian_integral<z, o, n>::iterator endian_integral<z, o, n>::begin () {
        return data ();
    }

    template <bool z, boost::endian::order o, std::size_t n>
    endian_integral<z, o, n>::const_iterator inline endian_integral<z, o, n>::begin () const {
        return data ();
    }

    template <bool z, boost::endian::order o, std::size_t n>
    endian_integral<z, o, n>::iterator inline endian_integral<z, o, n>::end () {
        return data () + n;
    }

    template <bool x, boost::endian::order o, std::size_t n>
    endian_integral<x, o, n>::const_iterator inline endian_integral<x, o, n>::end () const {
        return data () + n;
    }

    template <bool x, boost::endian::order o, std::size_t n>
    inline endian_integral<x, o, n>::operator bytes_view () const {
        return bytes_view (data (), n);
    }

    template <bool x, boost::endian::order o, std::size_t n>
    inline endian_integral<x, o, n>::operator slice<byte> () {
        return slice<byte> (data (), n);
    }

    template <bool z, boost::endian::order o, std::size_t s>
    writer<byte> inline &operator << (writer<byte> &w, endian_integral<z, o, s> x) {
        w.write (x.data (), s);
        return w;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    reader<byte> inline &operator >> (reader<byte> &r, endian_integral<z, o, s> &x) {
        return r >> slice<byte> (x);
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator ++ () {
        ++static_cast<endian_integral<z, o, s>::boost_arith &> (*this);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator -- () {
        --static_cast<endian_integral<z, o, s>::boost_arith &> (*this);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline endian_integral<z, o, s>::operator ++ (int) {
        auto n = *this;
        ++(*this);
        return n;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline endian_integral<z, o, s>::operator -- (int) {
        auto n = *this;
        --(*this);
        return n;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<true, o, s> inline operator - (const endian_integral<z, o, s> &x) {
        using p = endian_integral<z, o, s>::native_type;
        using q = std::make_signed_t<p>;
        return static_cast<endian_integral<true, o, s>> (-static_cast<q> (static_cast<p> (x)));
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<true, o, s> inline operator % (const endian_integral<z, o, s> &a, const endian_integral<true, o, s> &b) {
        return static_cast<endian_integral<true, o, s>> (
            static_cast<typename endian_integral<z, o, s>::native_type> (a) % static_cast<typename endian_integral<true, o, s>::native_type> (b));
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline operator << (const endian_integral<z, o, s> &x, int i) {
        return (typename endian_integral<z, o, s>::native_type) (x) << i;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline operator >> (const endian_integral<z, o, s> &x, int i) {
        return (typename endian_integral<z, o, s>::native_type) (x) >> i;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator += (endian_integral x) {
        static_cast<boost_arith &> (*this) += static_cast<boost_arith> (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator -= (endian_integral x) {
        static_cast<boost_arith &> (*this) -= static_cast<boost_arith> (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator *= (endian_integral x) {
        static_cast<boost_arith &> (*this) *= static_cast<boost_arith> (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator /= (endian_integral x) {
        static_cast<boost_arith &> (*this) /= static_cast<boost_arith> (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator += (native_type x) {
        boost_arith::operator += (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator -= (native_type x) {
        boost_arith::operator -= (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator *= (native_type x) {
        boost_arith::operator *= (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator /= (native_type x) {
        boost_arith::operator /= (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator &= (native_type x) {
        boost_arith::operator &= (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator |= (native_type x) {
        boost_arith::operator |= (x);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator <<= (int i) {
        boost_arith::operator <<= (i);
        return *this;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    endian_integral<z, o, s> inline &endian_integral<z, o, s>::operator >>= (int i) {
        boost_arith::operator >>= (i);
        return *this;
    }

    // big endian signed integer unaligned types
    template struct endian_integral<true, endian::big, 1>;
    template struct endian_integral<true, endian::big, 2>;
    template struct endian_integral<true, endian::big, 3>;
    template struct endian_integral<true, endian::big, 4>;
    template struct endian_integral<true, endian::big, 5>;
    template struct endian_integral<true, endian::big, 6>;
    template struct endian_integral<true, endian::big, 7>;
    template struct endian_integral<true, endian::big, 8>;

    // big endian unsigned integer unaligned types
    template struct endian_integral<false, endian::big, 1>;
    template struct endian_integral<false, endian::big, 2>;
    template struct endian_integral<false, endian::big, 3>;
    template struct endian_integral<false, endian::big, 4>;
    template struct endian_integral<false, endian::big, 5>;
    template struct endian_integral<false, endian::big, 6>;
    template struct endian_integral<false, endian::big, 7>;
    template struct endian_integral<false, endian::big, 8>;

    // little endian signed integer unaligned types
    template struct endian_integral<true, endian::little, 1>;
    template struct endian_integral<true, endian::little, 2>;
    template struct endian_integral<true, endian::little, 3>;
    template struct endian_integral<true, endian::little, 4>;
    template struct endian_integral<true, endian::little, 5>;
    template struct endian_integral<true, endian::little, 6>;
    template struct endian_integral<true, endian::little, 7>;
    template struct endian_integral<true, endian::little, 8>;

    // little endian unsigned integer unaligned types
    template struct endian_integral<false, endian::little, 1>;
    template struct endian_integral<false, endian::little, 2>;
    template struct endian_integral<false, endian::little, 3>;
    template struct endian_integral<false, endian::little, 4>;
    template struct endian_integral<false, endian::little, 5>;
    template struct endian_integral<false, endian::little, 6>;
    template struct endian_integral<false, endian::little, 7>;
    template struct endian_integral<false, endian::little, 8>;

    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::big, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::big, 8> &x);

    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::big, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::big, 8> &x);

    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<true, endian::little, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<true, endian::little, 8> &x);

    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, endian_integral<false, endian::little, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, endian_integral<false, endian::little, 8> &x);

}

#endif


