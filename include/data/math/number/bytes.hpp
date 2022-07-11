// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES
#define DATA_MATH_NUMBER_BYTES

#include <data/encoding/integer.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/math/number/integer.hpp>

namespace data::math::number {
    
    using Z = gmp::Z;
    using N = gmp::N;
    
    template <endian::order r> N_bytes<r> inline N_bytes<r>::read(string_view x) {
        if (!encoding::natural::valid(x)) throw std::invalid_argument{string{"invalid number string"} + string{x}};
        if (encoding::hexidecimal::valid(x)) return *encoding::natural::read<r>(x);
        if (encoding::decimal::valid(x)) return N_bytes<r>(N::read(x));
        throw std::invalid_argument{string{"invalid number string"} + string{x}};
    }
    
    template <endian::order r> Z_bytes<r, ones> inline Z_bytes<r, ones>::read(string_view x) {
        if (!encoding::integer::valid(x)) throw std::invalid_argument{string{"invalid number string"} + string{x}};
        if (encoding::hexidecimal::valid(x)) return *encoding::integer::read<r, ones>(x);
        return Z_bytes<r, ones>(Z::read(x));
    }
    
    template <endian::order r> Z_bytes<r, twos> inline Z_bytes<r, twos>::read(string_view x) {
        if (!encoding::integer::valid(x)) throw std::invalid_argument{string{"invalid number string"} + string{x}};
        if (encoding::hexidecimal::valid(x)) return *encoding::integer::read<r, twos>(x);
        return Z_bytes<r, twos>(Z::read(x));
    }
    
    template <endian::order r> 
    std::ostream inline &operator<<(std::ostream& o, const N_bytes<r>& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        else if (o.flags() & std::ios::dec) return encoding::decimal::write(o, N(n));
        return o;
    }
    
    template <endian::order r, complement c> 
    std::ostream inline &operator<<(std::ostream& o, const Z_bytes<r, c>& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        if (o.flags() & std::ios::dec) return encoding::integer::write(o, Z(n));
        return o;
    }
    
    template <endian::order r> inline N_bytes<r>::operator double() const {
        return double(N(*this));
    }
    
    template <endian::order r> inline Z_bytes<r, ones>::operator double() const {
        return double(Z(*this));
    }
    
    template <endian::order r> inline Z_bytes<r, twos>::operator double() const {
        return double(Z(*this));
    }
    
    template <endian::order r> N_bytes<r> inline operator/(const N_bytes<r> &x, const N_bytes<r> &j) {
        return natural::divide<N_bytes<r>>(x, j).Quotient;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator/(const Z_bytes<r, c> &x, const Z_bytes<r, c> &j) {
        return integer::divide<Z_bytes<r, c>>(x, j).Quotient;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator/(const Z_bytes<r, c> &x, const N_bytes<r> &j) {
        return integer::divide<Z_bytes<r, c>>(x, Z_bytes<r, c>{j}).Quotient;
    }
    
    template <endian::order r> N_bytes<r> inline operator/(const N_bytes<r> &x, uint64 j) {
        return integer::divide<N_bytes<r>>(x, N_bytes<r>{j}).Quotient;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator/(const Z_bytes<r, c> &x, int64 j) {
        return integer::divide<Z_bytes<r, c>>(x, Z_bytes<r, c>{j}).Quotient;
    }
    
    template <endian::order r> N_bytes<r> inline operator%(const N_bytes<r> &x, const N_bytes<r> &j) {
        return natural::divide<N_bytes<r>>(x, j).Remainder;
    }
    
    template <endian::order r> N_bytes<r> inline operator%(const Z_bytes<r, ones> &x, const N_bytes<r> &j) {
        return integer::divide<Z_bytes<r, ones>, N_bytes<r>>(x, Z_bytes<r, ones>{j}).Remainder;
    }
    
    template <endian::order r> Z_bytes<r, twos> inline operator%(const Z_bytes<r, twos> &x, const Z_bytes<r, twos> &j) {
        return integer::divide<Z_bytes<r, twos>>(x, j).Remainder;
    }
    
    template <endian::order r> uint64 inline operator%(const N_bytes<r> &x, uint64 j) {
        return uint64(natural::divide<N_bytes<r>>(x, N_bytes<r>{j}).Remainder);
    }
    
    template <endian::order r, complement c> uint64 inline operator%(const Z_bytes<r, c> &x, uint64 j) {
        return uint64(integer::divide<Z_bytes<r, c>>(x, Z_bytes<r, c>{j}).Remainder);
    }
    
}

#include <data/encoding/digits.hpp>
#include <data/math/number/integer.hpp>

namespace data::encoding::decimal {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        return std::make_shared<math::number::N_bytes<r>>(read_base<math::number::N_bytes<r>>(s, 10, digit));
    }
    
    template <endian::order r> 
    string inline write(const math::number::N_bytes<r> &n) {
        return string{write_base(math::number::gmp::N(n), characters())};
    }
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, math::number::complement n> 
    ptr<math::number::Z_bytes<r, n>> read(string_view s) {
        if (!valid(s)) return nullptr;
        bool negative = s[0] == '-';
        string_view positive = negative ? s.substr(1) : s; 
        auto z = std::make_shared<math::number::Z_bytes<r, n>>((math::number::Z_bytes<r, n>)(*decimal::read<r>(positive)));
        if (negative) *z = -*z;
        return z;
    }
    
}

namespace data::encoding::natural {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> inline read(string_view s) {
        if (!valid(s)) return nullptr;
        if (hexidecimal::valid(s)) {
            auto p = hexidecimal::read<r>(s);
            if (p == nullptr) return nullptr;
            return std::make_shared<math::number::N_bytes<r>>(math::number::N_bytes<r>::read(bytes_view(*p)));
        }
        
        return decimal::read<r>(s);
    }
    
}

namespace data::encoding::hexidecimal {
    
    namespace {
        template <endian::order r, math::number::complement c> struct get_bytes_type {
            using value = math::number::Z_bytes<r, c>;
        };
        
        template <endian::order r> struct get_bytes_type<r, math::number::nones> {
            using value = math::number::N_bytes<r>;
        };
        
        template <endian::order r, math::number::complement c> using bytes_type = get_bytes_type<r, c>::value;
        
        using nat = math::number::gmp::N;
        
        template <hex::letter_case zz>
        inline nat read_num(const integer<math::number::nones, zz>& n) {
            return read_base<nat>(n.substr(2), 16, &digit);
        } 
        
        template <typename N>
        std::string write_hexidecimal(const N& n) {
            static std::string Characters = hex::characters_lower();
            if (n == 0) return "0x00";
            std::string p = write_base<N>(n, Characters);
            if ((p.size() % 2) == 1) return std::string{"0x0"} + p;
            return std::string{"0x"} + p;
        }
        
        template <math::number::complement n, hex::letter_case zz> struct divide_by_int {
            math::division<integer<n, zz>, uint64> operator()(const integer<n, zz> &z, uint64 x) {
                throw method::unimplemented{"hex divide ones"};
            }
        };
        
        template <hex::letter_case zz> struct divide_by_int<math::number::nones, zz> {
            math::division<integer<math::number::nones, zz>, uint64> operator()(const integer<math::number::nones, zz> &z, uint64 x) {
                if (x == 0) throw math::division_by_zero{};
                // it is important to have this optimization. 
                if (x == 16) {
                    int last = z.size() - 1;
                    return math::division<integer<math::number::nones, zz>, uint64>{
                        integer<math::number::nones, zz>{std::string{"0x0"} + z.substr(2, last - 2)}, 
                        static_cast<uint64>(digit(z[last]))};
                }
                
                math::division<nat> div = math::number::natural::divide(read_num(z), nat{x});
                return math::division<integer<math::number::nones, zz>, uint64>{
                    integer<math::number::nones, zz>{write_hexidecimal(div.Quotient)}, uint64(div.Remainder)};
            }
        };
        
    }
    
    template <math::number::complement n, hex::letter_case zz>
    math::division<integer<n, zz>, uint64> integer<n, zz>::divide(uint64 x) const {
        return divide_by_int<n, zz>{}(*this, x);
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator/(const integer &x) const  {
        return integer{write(bytes_type<endian::little, c>::read(*this) / bytes_type<endian::little, c>::read(x))};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator%(const integer &x) const {
        return integer{write(bytes_type<endian::little, c>::read(*this) & bytes_type<endian::little, c>::read(x))};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    inline integer<c, zz>::operator double() const {
        return double(bytes_type<endian::little, c>::read(*this));
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r, math::number::complement c> 
    ptr<math::number::Z_bytes<r, c>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        if (hexidecimal::valid(s)) {
            auto z = hexidecimal::read<r>(s);
            if (z == nullptr) return nullptr;
            auto x = std::make_shared<math::number::Z_bytes<r, c>>();
            x->resize(z->size());
            std::copy(z->begin(), z->end(), x->begin());
            return x;
        }
        
        if (negative(s)) {
            auto z = decimal::read<r>(s.substr(1));
            if (z == nullptr) return nullptr;
            return std::make_shared<math::number::Z_bytes<r, c>>(math::number::Z_bytes<r, c>(-*z));
        }
        
        auto z = decimal::read<r>(s);
        if (z == nullptr) return nullptr;
        return std::make_shared<math::number::Z_bytes<r, c>>(math::number::Z_bytes<r, c>(*z));
    }
    
    template <endian::order r, math::number::complement c> 
    std::ostream inline &write(std::ostream &o, const math::number::Z_bytes<r, c>& z) {
        if (sign(z) == math::negative) o << "-";
        return decimal::write(o, abs(z));
    }
    
    template <endian::order r, math::number::complement c> 
    string inline write(const math::number::Z_bytes<r, c> &z) {
        std::stringstream ss;
        write(ss, z);
        return ss.str();
    }
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream &o, const math::number::N_bytes<r>& z) {
        return decimal::write(o, z);
    }
    
    template <endian::order r> 
    string inline write(const math::number::N_bytes<r> &z) {
        std::stringstream ss;
        write(ss, z);
        return ss.str();
    }
    
}

namespace data::math::number {
    
    // explicit instantiations
    template struct N_bytes<endian::big>;
    template struct N_bytes<endian::little>;
    template struct Z_bytes<endian::big, ones>;
    template struct Z_bytes<endian::little, ones>;
    template struct Z_bytes<endian::big, twos>;
    template struct Z_bytes<endian::little, twos>;
    
    template std::weak_ordering operator<=>(const N_bytes<endian::big>&, const N_bytes<endian::big>&);
    template std::weak_ordering operator<=>(const N_bytes<endian::little>&, const N_bytes<endian::little>&);
    
    template std::weak_ordering operator<=>(const Z_bytes<endian::big, ones>&, const Z_bytes<endian::big, ones>&);
    template std::weak_ordering operator<=>(const Z_bytes<endian::little, ones>&, const Z_bytes<endian::little, ones>&);
    template std::weak_ordering operator<=>(const Z_bytes<endian::big, twos>&, const Z_bytes<endian::big, twos>&);
    template std::weak_ordering operator<=>(const Z_bytes<endian::little, twos>&, const Z_bytes<endian::little, twos>&);
    
    template std::weak_ordering operator<=>(const Z_bytes<endian::big, ones>&, const N_bytes<endian::big>&);
    template std::weak_ordering operator<=>(const Z_bytes<endian::big, twos>&, const N_bytes<endian::big>&);
    template std::weak_ordering operator<=>(const Z_bytes<endian::little, ones>&, const N_bytes<endian::little>&);
    template std::weak_ordering operator<=>(const Z_bytes<endian::little, twos>&, const N_bytes<endian::little>&);
    
    template std::weak_ordering operator<=>(const N_bytes<endian::big>&, int64);
    template std::weak_ordering operator<=>(const N_bytes<endian::little>&, int64);
    
    template std::weak_ordering operator<=>(const Z_bytes<endian::big, ones>&, int64);
    template std::weak_ordering operator<=>(const Z_bytes<endian::big, twos>&, int64);
    template std::weak_ordering operator<=>(const Z_bytes<endian::little, ones>&, int64);
    template std::weak_ordering operator<=>(const Z_bytes<endian::little, twos>&, int64);
    
    template bool operator==(const N_bytes<endian::big>&, int64);
    template bool operator==(const Z_bytes<endian::big, ones>&, int64);
    template bool operator==(const Z_bytes<endian::big, twos>&, int64);
    
    template bool operator==(const N_bytes<endian::little>&, int64);
    template bool operator==(const Z_bytes<endian::little, ones>&, int64);
    template bool operator==(const Z_bytes<endian::little, twos>&, int64);
    
    template Z_bytes<endian::big, ones> operator~(const N_bytes<endian::big>&);
    template Z_bytes<endian::big, ones> operator~(const Z_bytes<endian::big, ones>&);
    
    template Z_bytes<endian::big, ones> operator^(const N_bytes<endian::big>&, const N_bytes<endian::big>&);
    template Z_bytes<endian::big, ones> operator^(const Z_bytes<endian::big, ones>&, const Z_bytes<endian::big, ones>&);
    template Z_bytes<endian::big, ones> operator^(const Z_bytes<endian::big, ones>&, const N_bytes<endian::big>&);
    template Z_bytes<endian::big, ones> operator^(const N_bytes<endian::big>&, const Z_bytes<endian::big, ones>&);
    
    template N_bytes<endian::big> operator&(const N_bytes<endian::big>&, const N_bytes<endian::big>&);
    template N_bytes<endian::big> operator|(const N_bytes<endian::big>&, const N_bytes<endian::big>&);
    template Z_bytes<endian::big, ones> operator-(const N_bytes<endian::big>&);
    
    template Z_bytes<endian::big, ones> operator&(const Z_bytes<endian::big, ones>&, const Z_bytes<endian::big, ones>&);
    template Z_bytes<endian::big, ones> operator&(const N_bytes<endian::big>&, const Z_bytes<endian::big, ones>&);
    template Z_bytes<endian::big, ones> operator&(const Z_bytes<endian::big, ones>&, const N_bytes<endian::big>&);
    
    template Z_bytes<endian::big, ones> operator|(const Z_bytes<endian::big, ones>&, const Z_bytes<endian::big, ones>&);
    template Z_bytes<endian::big, ones> operator|(const N_bytes<endian::big>&, const Z_bytes<endian::big, ones>&);
    template Z_bytes<endian::big, ones> operator|(const Z_bytes<endian::big, ones>&, const N_bytes<endian::big>&);
    
    template Z_bytes<endian::big, ones> operator-(const Z_bytes<endian::big, ones>&);
    
    template Z_bytes<endian::big, twos> operator&(const Z_bytes<endian::big, twos>&, const Z_bytes<endian::big, twos>&);
    template Z_bytes<endian::big, twos> operator&(const N_bytes<endian::big>&, const Z_bytes<endian::big, twos>&);
    template Z_bytes<endian::big, twos> operator&(const Z_bytes<endian::big, twos>&, const N_bytes<endian::big>&);
    
    template Z_bytes<endian::big, twos> operator|(const Z_bytes<endian::big, twos>&, const Z_bytes<endian::big, twos>&);
    template Z_bytes<endian::big, twos> operator|(const N_bytes<endian::big>&, const Z_bytes<endian::big, twos>&);
    template Z_bytes<endian::big, twos> operator|(const Z_bytes<endian::big, twos>&, const N_bytes<endian::big>&);
    
    template Z_bytes<endian::big, twos> operator-(const Z_bytes<endian::big, twos>&);
    
    template Z_bytes<endian::little, ones> operator~(const N_bytes<endian::little>&);
    template Z_bytes<endian::little, ones> operator~(const Z_bytes<endian::little, ones>&);
    
    template Z_bytes<endian::little, ones> operator^(const N_bytes<endian::little>&, const N_bytes<endian::little>&);
    template Z_bytes<endian::little, ones> operator^(const Z_bytes<endian::little, ones>&, const Z_bytes<endian::little, ones>&);
    template Z_bytes<endian::little, ones> operator^(const Z_bytes<endian::little, ones>&, const N_bytes<endian::little>&);
    template Z_bytes<endian::little, ones> operator^(const N_bytes<endian::little>&, const Z_bytes<endian::little, ones>&);
    
    template N_bytes<endian::little> operator&(const N_bytes<endian::little>&, const N_bytes<endian::little>&);
    template N_bytes<endian::little> operator|(const N_bytes<endian::little>&, const N_bytes<endian::little>&);
    template Z_bytes<endian::little, ones> operator-(const N_bytes<endian::little>&);
    
    template Z_bytes<endian::little, ones> operator&(const Z_bytes<endian::little, ones>&, const Z_bytes<endian::little, ones>&);
    template Z_bytes<endian::little, ones> operator&(const N_bytes<endian::little>&, const Z_bytes<endian::little, ones>&);
    template Z_bytes<endian::little, ones> operator&(const Z_bytes<endian::little, ones>&, const N_bytes<endian::little>&);
    
    template Z_bytes<endian::little, ones> operator|(const Z_bytes<endian::little, ones>&, const Z_bytes<endian::little, ones>&);
    template Z_bytes<endian::little, ones> operator|(const N_bytes<endian::little>&, const Z_bytes<endian::little, ones>&);
    template Z_bytes<endian::little, ones> operator|(const Z_bytes<endian::little, ones>&, const N_bytes<endian::little>&);
    
    template Z_bytes<endian::little, ones> operator-(const Z_bytes<endian::little, ones>&);
    
    template Z_bytes<endian::little, twos> operator&(const Z_bytes<endian::little, twos>&, const Z_bytes<endian::little, twos>&);
    template Z_bytes<endian::little, twos> operator&(const N_bytes<endian::little>&, const Z_bytes<endian::little, twos>&);
    template Z_bytes<endian::little, twos> operator&(const Z_bytes<endian::little, twos>&, const N_bytes<endian::little>&);
    
    template Z_bytes<endian::little, twos> operator|(const Z_bytes<endian::little, twos>&, const Z_bytes<endian::little, twos>&);
    template Z_bytes<endian::little, twos> operator|(const N_bytes<endian::little>&, const Z_bytes<endian::little, twos>&);
    template Z_bytes<endian::little, twos> operator|(const Z_bytes<endian::little, twos>&, const N_bytes<endian::little>&);
    
    template Z_bytes<endian::little, twos> operator-(const Z_bytes<endian::little, twos>&);
    
}

#endif
