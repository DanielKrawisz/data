// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC
#define DATA_ARITHMETIC

#include <data/stream.hpp>
#include <data/encoding/halves.hpp>
#include <data/encoding/words.hpp>
#include <data/math/abs.hpp>
#include <data/math/sign.hpp>

namespace data::endian {

    template <bool is_signed, boost::endian::order Order, std::size_t bytes> struct arithmetic;

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> &operator ++ (arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> &operator -- (arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> operator ++ (arithmetic<z, o, s> &, int);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> operator -- (arithmetic<z, o, s> &, int);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> operator + (const arithmetic<z, o, s> &, const arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> operator - (const arithmetic<z, o, s> &, const arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> operator * (const arithmetic<z, o, s> &, const arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> operator / (const arithmetic<z, o, s> &, const arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> &operator += (arithmetic<z, o, s> &, const arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> &operator -= (arithmetic<z, o, s> &, const arithmetic<z, o, s> &);

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> &operator *= (arithmetic<z, o, s> &, const arithmetic<z, o, s> &);
    
    template <bool is_signed, boost::endian::order Order, std::size_t bytes>
    struct arithmetic : boost::endian::endian_arithmetic<Order, to_native<is_signed, bytes>, 8 * bytes, boost::endian::align::no> {
        
        using native_type = to_native<is_signed, bytes>;
        using boost_arith = boost::endian::endian_arithmetic<Order, native_type, 8 * bytes, boost::endian::align::no>;
        using boost_arith::data;
        using boost_arith::boost_arith;
        using iterator = byte*;
        using const_iterator = byte const*;
        
        arithmetic (const boost_arith &x) : boost_arith (x) {}
        arithmetic (boost_arith &&x) : boost_arith (x) {}
        
        iterator begin () {
            return data ();
        }
        
        const_iterator begin () const {
            return data ();
        }
        
        iterator end () {
            return data () + bytes;
        }
        
        const_iterator end () const {
            return data () + bytes;
        }
        
        explicit operator bytes_view () const {
            return bytes_view (data (), bytes);
        }
        
        explicit operator slice<byte> () {
            return slice<byte> (data (), bytes);
        }
        
        using opposite_endian = arithmetic<is_signed, opposite (Order), bytes>;
        
        explicit operator opposite_endian () const {
            return opposite_endian {native_type ()};
        }
        
        explicit arithmetic (const opposite_endian& x) : boost_arith {native_type (x)} {}
        
        size_t size () const {
            return bytes;
        }
        
        const byte& operator [] (int i) const {
            if (i >= int (bytes))
                throw std::out_of_range {string {"access index "} + std::to_string (i) +
                    " in arithmetic number of size " + std::to_string (bytes)};
            if (i < 0) return operator [] (bytes + i);
            return data () [i];
        }
        
        using words_type = encoding::words<Order, byte>;
        
        words_type words () {
            return words_type {slice<byte>(this->data(), bytes)};
        }
        
        const words_type words () const {
            return words_type {slice<byte>(const_cast<byte*>(this->data()), bytes)};
        }
    };
    
    template <bool z, boost::endian::order o, std::size_t s>
    writer<byte> inline &operator << (writer<byte> &w, arithmetic<z, o, s> x) {
        return w << bytes_view (x);
    }
    
    template <bool z, boost::endian::order o, std::size_t s>
    reader<byte> inline &operator >> (reader<byte> &r, arithmetic<z, o, s> &x) {
        return r >> slice<byte> (x);
    }
    
    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline operator + (const arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        return static_cast<const arithmetic<z, o, s>::boost_arith &> (a) + static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
    }
    
    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline operator - (const arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        return static_cast<const arithmetic<z, o, s>::boost_arith &> (a) - static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
    }
    
    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline operator * (const arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        return static_cast<const arithmetic<z, o, s>::boost_arith &> (a) * static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
    }
    
    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline operator / (const arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        return static_cast<const arithmetic<z, o, s>::boost_arith &> (a) / static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
    }

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline &operator += (arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        static_cast<arithmetic<z, o, s>::boost_arith &> (a) += static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
        return a;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline &operator -= (arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        static_cast<arithmetic<z, o, s>::boost_arith &> (a) -= static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
        return a;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline &operator *= (arithmetic<z, o, s> &a, const arithmetic<z, o, s> &b) {
        static_cast<arithmetic<z, o, s>::boost_arith &> (a) *= static_cast<const arithmetic<z, o, s>::boost_arith &> (b);
        return a;
    }
    
    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline &operator ++ (arithmetic<z, o, s> &x) {
        ++static_cast<arithmetic<z, o, s>::boost_arith &> (x);
        return x;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline &operator -- (arithmetic<z, o, s> &x) {
        --static_cast<arithmetic<z, o, s>::boost_arith &> (x);
        return x;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline operator ++ (arithmetic<z, o, s> &x, int) {
        auto n = x;
        ++x;
        return n;
    }

    template <bool z, boost::endian::order o, std::size_t s>
    arithmetic<z, o, s> inline operator -- (arithmetic<z, o, s> &x, int) {
        auto n = x;
        --x;
        return n;
    }
    
}

namespace data::encoding {
    
    template <bool z, endian::order o, size_t size> struct count_digits<endian::arithmetic<z, o, size>> {
        constexpr static bool is_signed = z;
        constexpr static size_t value = size;
    };
    
    template <bool is_signed, endian::order o, size_t size> struct half_of<endian::arithmetic<is_signed, o, size>> {
        using type = endian::arithmetic<is_signed, o, count_digits<typename half_of<endian::to_native<is_signed, size>>::type>::value>;
        static type greater_half (endian::arithmetic<is_signed, o, size> u) {
            return type {half_of<endian::to_native<is_signed, size>>::greater_half ((endian::to_native<is_signed, size>) (u))};
        }
        
        static type lesser_half(endian::arithmetic<is_signed, o, size> u) {
            return type {half_of<endian::to_native<is_signed, size>>::lesser_half ((endian::to_native<is_signed, size>) (u))};
        }
    };
    
    template <bool is_signed, endian::order o, size_t size> struct twice<endian::arithmetic<is_signed, o, size>> {
        using type = endian::arithmetic<is_signed, o, 2 * size>;
        static type extend (endian::arithmetic<is_signed, o, size> x) {
            return (typename twice<endian::to_native<is_signed, size>>::type) (x);
        }
    };
    
}

namespace data::math {

    template <boost::endian::order o, std::size_t z>
    struct abs<endian::arithmetic<false, o, z>> {
        endian::arithmetic<false, o, z> operator () (const endian::arithmetic<false, o, z> &x) {
            return x;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct abs<endian::arithmetic<true, o, z>> {
        endian::arithmetic<false, o, z> operator () (const endian::arithmetic<true, o, z> &x) {
            return endian::arithmetic<false, o, z> (data::abs ((typename endian::arithmetic<true, o, z>::native_type) (x)));
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct quadrance<endian::arithmetic<false, o, z>> {
        endian::arithmetic<false, o, z> operator () (const endian::arithmetic<false, o, z> &x) {
            return x * x;
        }
    };

    template <boost::endian::order o, std::size_t z>
    struct quadrance<endian::arithmetic<true, o, z>> {
        endian::arithmetic<false, o, z> operator () (const endian::arithmetic<true, o, z> &x) {
            return data::abs (x) * data::abs (x);
        }
    };

    template <bool s, boost::endian::order o, std::size_t z>
    bool inline is_zero (const endian::arithmetic<s, o, z> &x) {
        return x == 0;
    }

    template <bool s, boost::endian::order o, std::size_t z>
    bool inline is_negative (const endian::arithmetic<s, o, z> &x) {
        return x < 0;
    }

    template <bool s, boost::endian::order o, std::size_t z>
    bool inline is_positive (const endian::arithmetic<s, o, z> &x) {
        return x > 0;
    }

}

namespace data {

    template <bool s, boost::endian::order o, std::size_t z> math::sign inline sign (const endian::arithmetic<s, o, z> &x) {
        return math::is_negative (x) ? math::negative : math::is_positive (x) ? math::positive : math::zero;
    }

    template <bool s, boost::endian::order o, std::size_t z>
    endian::arithmetic<s, o, z> inline increment (const endian::arithmetic<s, o, z> &x) {
        return x + 1;
    }

    template <boost::endian::order o, std::size_t z>
    endian::arithmetic<false, o, z> inline decrement (const endian::arithmetic<false, o, z> &x) {
        return x == 0 ? 0 : x - 1;
    }

    template <boost::endian::order o, std::size_t z>
    endian::arithmetic<true, o, z> inline decrement (const endian::arithmetic<true, o, z> &x) {
        return x - 1;
    }
}

namespace data {
    
    // big endian signed integer unaligned types
    typedef endian::arithmetic<true, endian::big, 1>           int8_big;
    typedef endian::arithmetic<true, endian::big, 2>          int16_big;
    typedef endian::arithmetic<true, endian::big, 3>          int24_big;
    typedef endian::arithmetic<true, endian::big, 4>          int32_big;
    typedef endian::arithmetic<true, endian::big, 5>          int40_big;
    typedef endian::arithmetic<true, endian::big, 6>          int48_big;
    typedef endian::arithmetic<true, endian::big, 7>          int56_big;
    typedef endian::arithmetic<true, endian::big, 8>          int64_big;
    
    // big endian unsigned integer unaligned types
    typedef endian::arithmetic<false, endian::big, 1>         uint8_big;
    typedef endian::arithmetic<false, endian::big, 2>        uint16_big;
    typedef endian::arithmetic<false, endian::big, 3>        uint24_big;
    typedef endian::arithmetic<false, endian::big, 4>        uint32_big;
    typedef endian::arithmetic<false, endian::big, 5>        uint40_big;
    typedef endian::arithmetic<false, endian::big, 6>        uint48_big;
    typedef endian::arithmetic<false, endian::big, 7>        uint56_big;
    typedef endian::arithmetic<false, endian::big, 8>        uint64_big;
    
    // little endian signed integer unaligned types
    typedef endian::arithmetic<true, endian::little, 1>     int8_little;
    typedef endian::arithmetic<true, endian::little, 2>    int16_little;
    typedef endian::arithmetic<true, endian::little, 3>    int24_little;
    typedef endian::arithmetic<true, endian::little, 4>    int32_little;
    typedef endian::arithmetic<true, endian::little, 5>    int40_little;
    typedef endian::arithmetic<true, endian::little, 6>    int48_little;
    typedef endian::arithmetic<true, endian::little, 7>    int56_little;
    typedef endian::arithmetic<true, endian::little, 8>    int64_little;
    
    // little endian unsigned integer unaligned types
    typedef endian::arithmetic<false, endian::little, 1>   uint8_little;
    typedef endian::arithmetic<false, endian::little, 2>  uint16_little;
    typedef endian::arithmetic<false, endian::little, 3>  uint24_little;
    typedef endian::arithmetic<false, endian::little, 4>  uint32_little;
    typedef endian::arithmetic<false, endian::little, 5>  uint40_little;
    typedef endian::arithmetic<false, endian::little, 6>  uint48_little;
    typedef endian::arithmetic<false, endian::little, 7>  uint56_little;
    typedef endian::arithmetic<false, endian::little, 8>  uint64_little;
    
}

namespace data::endian {

    // big endian signed integer unaligned types
    template struct endian::arithmetic<true, endian::big, 1>;
    template struct endian::arithmetic<true, endian::big, 2>;
    template struct endian::arithmetic<true, endian::big, 3>;
    template struct endian::arithmetic<true, endian::big, 4>;
    template struct endian::arithmetic<true, endian::big, 5>;
    template struct endian::arithmetic<true, endian::big, 6>;
    template struct endian::arithmetic<true, endian::big, 7>;
    template struct endian::arithmetic<true, endian::big, 8>;

    // big endian unsigned integer unaligned types
    template struct endian::arithmetic<false, endian::big, 1>;
    template struct endian::arithmetic<false, endian::big, 2>;
    template struct endian::arithmetic<false, endian::big, 3>;
    template struct endian::arithmetic<false, endian::big, 4>;
    template struct endian::arithmetic<false, endian::big, 5>;
    template struct endian::arithmetic<false, endian::big, 6>;
    template struct endian::arithmetic<false, endian::big, 7>;
    template struct endian::arithmetic<false, endian::big, 8>;

    // little endian signed integer unaligned types
    template struct endian::arithmetic<true, endian::little, 1>;
    template struct endian::arithmetic<true, endian::little, 2>;
    template struct endian::arithmetic<true, endian::little, 3>;
    template struct endian::arithmetic<true, endian::little, 4>;
    template struct endian::arithmetic<true, endian::little, 5>;
    template struct endian::arithmetic<true, endian::little, 6>;
    template struct endian::arithmetic<true, endian::little, 7>;
    template struct endian::arithmetic<true, endian::little, 8>;

    // little endian unsigned integer unaligned types
    template struct endian::arithmetic<false, endian::little, 1>;
    template struct endian::arithmetic<false, endian::little, 2>;
    template struct endian::arithmetic<false, endian::little, 3>;
    template struct endian::arithmetic<false, endian::little, 4>;
    template struct endian::arithmetic<false, endian::little, 5>;
    template struct endian::arithmetic<false, endian::little, 6>;
    template struct endian::arithmetic<false, endian::little, 7>;
    template struct endian::arithmetic<false, endian::little, 8>;
    
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::big, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::big, 8> &x);
    
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::big, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::big, 8> &x);
    
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<true, endian::little, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<true, endian::little, 8> &x);
    
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 1> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 1> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 2> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 2> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 3> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 3> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 4> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 4> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 5> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 5> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 6> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 6> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 7> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 7> &x);
    template writer<byte> &operator << (writer<byte> &w, arithmetic<false, endian::little, 8> x);
    template reader<byte> &operator >> (reader<byte> &r, arithmetic<false, endian::little, 8> &x);
    
}

#endif


