// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <data/bytestring.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/encoding/halves.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/encoding/words.hpp>

namespace data::math::number {
    
    template <bool is_signed, endian::order, size_t size> struct bounded;
    
    template <endian::order r, size_t size>
    struct bounded<false, r, size> : bytestring<r, size> {
        using bit32 = uint32;
        using bit64 = uint64;
        
        using array = bytestring<r, size>;
        
        bounded() : array(0x00) {}
        
        bounded(const uint64 x) : array(0x00) {
            words().set(0, lesser_half(endian::arithmetic<r, uint64, 64>{x}));
            words().set(1, greater_half(endian::arithmetic<r, uint64, 64>{x}));
        }
        
        bounded(const array& b) : array{b} {}
        
        explicit bounded(slice<byte, size>);
        explicit bounded(string_view s);
        
        operator slice<byte, size>() const {
            return slice<byte, size>{const_cast<byte*>(array::data())};
        }
        
        math::sign sign() const;
        
        bool operator==(const int64 x) const {
            return x < 0 ? false : (*this == static_cast<data::uint64>(x));
        }
        
        // power
        bounded operator^(const bounded&) const;
        bounded& operator^=(const bounded&);
        
        bool operator<(const bounded&) const;
        bool operator<=(const bounded&) const;

        bool operator>(const bounded& d) const;
        bool operator>=(const bounded& d) const;
        
        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
        bounded& operator*=(const bounded&);
        
        bounded& operator+=(const uint32&);
        bounded& operator-=(const uint32&);
        bounded& operator*=(const uint32&);
        
        static bounded max();
        static bounded min();
        static N_bytes<r> modulus();
        
        bounded& operator++() {
            operator+=(1);
            return *this;
        }
        
        bounded& operator--() {
            operator-=(1);
            return *this;
        }
        
        bounded operator++(int) {
            bounded n = *this;
            ++(*this);
            return n;
        }
        
        bounded operator--(int) {
            bounded n = *this;
            ++(*this);
            return n;
        }
        
        bounded& operator<<=(int32);
        bounded& operator>>=(int32);
        
        math::division<bounded> divide(const bounded&) const;
        
        bounded operator/(const bounded& n) const {
            return divide(n).Quotient;
        }
        
        bounded operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
    private:
        explicit bounded(const N_bytes<r>& n) : bounded{} {
            if (n.size() <= size) {
                if (r == endian::little) {
                    std::copy(n.begin(), n.end(), array::begin());
                } else {
                    std::copy(n.begin(), n.end(), array::begin() + (size - n.size()));
                }
            }
            if (n > N_bytes<r> {max()}) throw std::out_of_range{"N_bytes too big"};
            if (r == endian::little) {
                std::copy(n.begin(), n.begin() + size, array::begin());
            } else {
                std::copy(n.begin() + (n.size() - size), n.end(), array::begin());
            }
        }
        
        bounded(const bounded<true, r, size>&) {
            throw method::unimplemented{"bounded<size, o, false>{bounded<size, o, true>}"};
        }
        
        friend struct abs<bounded<false, r, size>, bounded<true, r, size>>;
        
        data::arithmetic::words<uint32, r> words() {
            throw method::unimplemented{"words"};
        }
    };
    
    template <endian::order r, size_t size>
    struct bounded<true, r, size> : data::bytestring<r, size> {
        using bit32 = int32;
        using bit64 = int64;
        
        using array = data::bytestring<r, size>;
        
        bounded() : array{0} {}
        
        bounded(const int64 x) : array(x < 0 ? 0xff : 0x00) {
            words().set(0, lesser_half(endian::arithmetic<r, int64, 64>{x}));
            words().set(1, greater_half(endian::arithmetic<r, int64, 64>{x}));
        }
        
        bounded(const array& x) : array{x} {}
        
        bounded(const bounded<false, r, size>&) {
            throw method::unimplemented{"signed bounded from unsigned bounded"};
        }
        
        explicit bounded(string_view s);
        
        explicit bounded(slice<byte, size>);
        
        operator slice<byte, size>() const {
            return slice<byte>::data();
        }
        
        bounded& operator=(const bounded& d) {
            bytes::operator=(static_cast<const bytes&>(d));
            return *this;
        }
        
        bounded operator-() const;
        
        math::sign sign() const;
        
        bool operator==(const int64 x) const {
            return *this == data::math::number::bounded<true, r, size>(x);
        }
        
        // power
        bounded operator^(const bounded<size, r, false>&) const;
        bounded& operator^=(const bounded<size, r, false>&);

        bool operator<(const bounded& d) const;
        bool operator<=(const bounded& d) const;

        bool operator>(const bounded& d) const;
        bool operator>=(const bounded& d) const;

        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
        bounded& operator*=(const bounded&);

        bounded& operator+=(const int32&);
        bounded& operator-=(const int32&);
        bounded& operator*=(const int32&);
        
        static bounded max();
        static bounded min();

        static N_bytes<r> modulus() {
            return bounded<false, r, size>::modulus();
        }

        bounded& operator++() {
            operator+=(1);
            return *this;
        }

        bounded& operator--() {
            operator-=(1);
            return *this;
        }
        
        bounded operator++(int) {
            bounded z = *this;
            ++(*this);
            return z;
        }

        bounded operator--(int) {
            bounded z = *this;
            --(*this);
            return z;
        }

        bounded& operator<<=(int32);
        bounded& operator>>=(int32);

        math::division<bounded> divide(const bounded&) const;
        
        bounded operator/(const bounded& n) const {
            return divide(n).Quotient;
        }

        bounded operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
    private:
        explicit bounded(const Z_bytes<r>& z) {
            if (z > Z_bytes<r> {max()} || z < Z_bytes<r> {min()}) throw std::out_of_range{"Z_bytes too big"};
            throw method::unimplemented{"bounded{Z_bytes}"};
        }
        
        data::arithmetic::words<int32, r> words() {
            throw method::unimplemented{"words"};
        }
    };
    
}

// Declare the plus and times operations on these types
// as satisfying the expected relations. 
namespace data::math {
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<data::plus<math::number::bounded<is_signed, r, size>>, 
        math::number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<data::plus<math::number::bounded<is_signed, r, size>>, 
        math::number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<data::times<math::number::bounded<is_signed, r, size>>, 
        math::number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<data::times<math::number::bounded<is_signed, r, size>>, 
        math::number::bounded<is_signed, r, size>> {};
}

template <data::endian::order r, size_t size>
inline std::ostream& operator<<(std::ostream& s, const data::math::number::bounded<true, r, size>& n) {
    return s << data::math::number::Z_bytes<r>{n};
}

template <data::endian::order r, size_t size>
inline std::ostream& operator<<(std::ostream& s, const data::math::number::bounded<false, r, size>& n) {
    return s << data::math::number::N_bytes<r>{n};
}

namespace data::math::number {
    /*
    template <size_t size, endian::order o>
    inline math::sign bounded<size, o, false>::sign() const {
        return *this == 0 ? math::zero : math::positive;
    }
        
    template <size_t size, endian::order o>
    inline math::sign bounded<size, o, true>::sign() const {
        if (*this == 0) return math::zero;
        return bytes::operator[](o == endian::big ? 0 : size - 1) < 0x80 ? math::positive : math::negative;
    }
    
    template <size_t size, endian::order o>
    struct abs<bounded<size, o, false>, bounded<size, o, false>> {
        bounded<size, o, false> operator()(const bounded<size, o, false>& i) {
            return i;
        }
    };
        
    template <size_t size, endian::order o, bool is_signed>
    struct sign<bounded<size, o, is_signed>> {
        math::sign operator()(const bounded<size, o, is_signed>& i) {
            return i.sign();
        }
    };

    template <size_t size, endian::order o, bool is_signed>
    struct arg<bounded<size, o, is_signed>> {
        bounded<size, o, false> operator()(const bounded<size, o, is_signed>& i) {
            if (i == 0) throw division_by_zero{};
            return i.sign() == math::negative ? -1 : 1;
        }
    };
    */
}
    
namespace data::encoding::hexidecimal {
    
    template <bool is_signed, endian::order r, size_t size>
    std::string write(const math::number::bounded<is_signed, r, size>& n);

}

namespace data::encoding::decimal {
    
    template <bool is_signed, endian::order r, size_t size>
    std::string write(const math::number::bounded<is_signed, r, size>& n);
    
}
    
namespace data::math::number {
/*
    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator<<=(int32 bits) {
        return operator=(array::operator<<(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator>>=(int32 bits) {
        return operator=(array::operator>>(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator<<=(int32 bits) {
        return operator=(array::operator<<(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator>>=(int32 bits) {
        return operator=(array::operator>>(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator+=(const bounded<size, o, false>& n) {
        methods::plus(size, array::words(), n.words(), array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator+=(const bounded<size, o, true>& n) {
        methods::plus(size, array::words(), n.words(), array::words());
        return *this;
    }
    
    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator-=(const bounded<size, o, false>& n) {
        methods::plus(size, array::words(), n.words(), array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator-=(const bounded<size, o, true>& n) {
        methods::plus(array::words_type::Last, array::words(), n.words(), array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator*=(const bounded<size, o, false>& n) {
        return operator=(operator*(n));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator+=(const bit32& n) {
        return methods::plus(size, array::words(), n, array::words());
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>
    bounded<size, o, true>::operator-() const {
        return array::operator~() + 1;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator+=(const bit32& n) {
        return methods::plus(size, array::words(), n, array::words());
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator-=(const bit32& n) {
        methods::minus(size, array::words(), n, array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator-=(const bit32& n) {
        words_type out = array::words();
        methods::minus(size, array::words(), n, out);
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator*=(const bit32& n) {
        methods::times(size, array::words(), n, array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline math::division<bounded<size, o, false>>
    bounded<size, o, false>::divide(const bounded& n) const {
        return math::number::natural::divide(*this, n);
    }

    template <size_t size, endian::order o>
    inline math::division<bounded<size, o, true>>
    bounded<size, o, true>::divide(const bounded& n) const {
        return math::number::integer::divide(*this, n);
    }

    template <size_t size, endian::order o>
    bool bounded<size, o, false>::operator<(const bounded& n) const {
        return methods::less(words_type::Last, array::words(), n.words());
    }

    template <size_t size, endian::order o>
    bool bounded<size, o, false>::operator<=(const bounded& n) const {
        return methods::less_equal(words_type::Last, array::words(), n.words());
    }

    template <size_t size, endian::order o>
    bool bounded<size, o, true>::operator<(const bounded& n) const {
        return methods::less(words_type::Last, array::words(), n.words());
    }

    template <size_t size, endian::order o>
    bool bounded<size, o, true>::operator<=(const bounded& n) const {
        return methods::less_equal(words_type::Last, array::words(), n.words());
    }
*/
    template <endian::order o, size_t size>
    bounded<false, o, size>::bounded(string_view s) : bounded{} {
        if (!encoding::natural::valid(s)) throw std::invalid_argument{"not a natural number"};

        if (encoding::hexidecimal::valid(s) && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
        *this = bounded{N_bytes<o>{s}};
    }
/*
    template <size_t size, endian::order o>
    bounded<size, o, true>::bounded(string_view s) : bounded{} {
        if (!encoding::integer::valid(s)) throw std::invalid_argument{"not an integer"};
            
        // Is there a minus sign?
        bool minus_sign = encoding::integer::negative(s);
        bool hexidecimal = encoding::hexidecimal::valid(s);
        
        if (hexidecimal) {
            if (s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            byte fill = minus_sign ? 0xff : 0x00;
            for (int i = 0; i < size; i++) this->operator[](i) = fill;
            bytes b{encoding::hexidecimal::read(s, o)};
        } else {
            *this = bounded{Z_bytes<o>{s}};
        }
        
    }
*/
    template <endian::order o, size_t size>
    bounded<false, o, size> bounded<false, o, size>::min() {
        return 0;
    }

    template <endian::order o, size_t size>
    bounded<false, o, size> bounded<false, o, size>::max() {
        bounded b{};
        for (int i = 0; i <= size; i++) b[i] = 0xff;
        return b;
    }
/*
    template <size_t size, endian::order o>
    N_bytes<o> bounded<size, o, false>::modulus() {
        std::string one = std::string{"0x01"} + encoding::hexidecimal::write(bounded{0}).substr(2);
        return N_bytes<o> {one};
    }

    template <size_t size, endian::order o>
    bounded<size, o, true> bounded<size, o, true>::min() {
        bounded b{};
        words_type w = b.words();
        w.set(words_type::Last, 0x80000000);
        return b;
    }

    template <size_t size, endian::order o>
    bounded<size, o, true> bounded<size, o, true>::max() {
        bounded b{};
        words_type w = b.words();
        w.set(words_type::Last, 0x7fffffff);
        for (int i = 0; i < words_type::Last; i++) w.set(i, 0xffffffff);
        return b;
    }
*/
}

namespace data {
    
    template <size_t size> using uint = math::number::bounded<false, endian::order::big, size>;
    template <size_t size> using integer = math::number::bounded<true, endian::order::big, size>;

    namespace encoding::hexidecimal {

        template <bool is_signed, endian::order r, size_t size>
        std::string write(const math::number::bounded<is_signed, r, size>& n) {
            std::stringstream ss;
            ss << std::hex << n;
            return ss.str();
        }

    }

    namespace encoding::decimal {

        template <bool is_signed, endian::order r, size_t size>
        std::string write(const math::number::bounded<is_signed, r, size>& n) {
            std::stringstream ss;
            ss << std::dec << n;
            return ss.str();
        }

    }

}

#endif
