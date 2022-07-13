// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/digits.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        ptr<bytes> read(string_view s, endian::order r) {
            if (!valid(s)) return nullptr;
            math::number::N_bytes<endian::big> n{math::number::N{s}};
            if (r == endian::little) std::reverse(n.begin(), n.end());
            return std::make_shared<bytes>(static_cast<bytes>(n));
        }
        
        N::N() : string{"0"} {}
        
        N::N(const string& x) : string{decimal::valid(x) ? x : ""} {}
        
        template <typename N>
        std::string write_decimal(const N& n) {
            static std::string Characters = characters();
            if (n == 0) return "0";
            return write_base<N>(n, Characters);
        }
        
        using nat = math::number::N;
        
        N::N(uint64 x) : string{write_decimal(nat{x})} {}
        
        inline nat read_num(const N& n) {
            return read_base<nat>(n, 10, &digit);
        }
        
        bool N::operator<=(const N& n) const {
            return read_num(*this) <= read_num(n);
        }
        
        bool N::operator>=(const N& n) const {
            return read_num(*this) >= read_num(n);
        }
        
        bool N::operator<(const N& n) const {
            return read_num(*this) < read_num(n);
        }
        
        bool N::operator>(const N& n) const {
            return read_num(*this) > read_num(n);
        }
        
        N N::operator+(const N& n) const {
            return N{write_decimal(read_num(*this) + read_num(n))};
        }
        
        N N::operator-(const N& n) const {
            return N{write_decimal(read_num(*this) - read_num(n))};
        }
        
        N N::operator*(const N& n) const {
            return N{write_decimal(read_num(*this) * read_num(n))};
        }
    
        N& N::operator++() {
            return *this = *this + 1;
        }
        
        N& N::operator--() {
            return *this = *this - 1;
        }
        
        N N::operator++(int) {
            N n = *this;
            ++(*this);
            return n;
        }
        
        N N::operator--(int) {
            N n = *this;
            --(*this);
            return n;
        }
        
        N N::operator<<(int i) const {
            return N{write_decimal(read_num(*this) << i)};
        }
        
        N N::operator>>(int i) const {
            return N{write_decimal(read_num(*this) >> i)};
        }
        
        N& N::operator+=(const N& n) {
            return *this = *this + n;
        }
        
        N& N::operator-=(const N& n) {
            return *this = *this - n;
        }
        
        N& N::operator*=(const N& n) {
            return *this = *this * n;
        }
        
        N& N::operator<<=(int i) {
            return *this = *this << i;
        }
        
        N& N::operator>>=(int i) {
            return *this = *this >> i;
        }
        
        math::division<N, uint64> N::divide(uint64 x) const {
            if (x == 0) throw math::division_by_zero{};
            // it is important to have this optimization. 
            if (x == 10) {
                int last = string::size() - 1;
                return math::division<N, uint64>{
                    N{string::substr(0, last)}, 
                    static_cast<uint64>(digit(string::operator[](last)))};
            }
            
            math::division<nat> div = read_num(*this).divide(nat{x});
            return math::division<N, uint64>{write_decimal(div.Quotient), uint64(div.Remainder)};
        }
    
    }
    
    namespace hexidecimal {
        
        N::N() : string{"0x00"} {}
        
        N::N(const string& x) : string{hexidecimal::valid(x) ? x : ""} {}
        
        template <typename N>
        std::string write_hexidecimal(const N& n) {
            static std::string Characters = hex::characters_lower();
            if (n == 0) return "0x00";
            std::string p = write_base<N>(n, Characters);
            if ((p.size() % 2) == 1) return std::string{"0x0"} + p;
            return std::string{"0x"} + p;
        }
        
        using nat = math::number::N;
        
        N::N(uint64 x) : string{write_hexidecimal(nat{x})} {}
        
        inline nat read_num(const N& n) {
            return read_base<nat>(n.substr(2), 16, &digit);
        }
        
        bool N::operator==(const N& n) const {
            return read_num(*this) == read_num(n);
        }
        
        bool N::operator!=(const N& n) const {
            return read_num(*this) != read_num(n);
        }
        
        bool N::operator<=(const N& n) const {
            return read_num(*this) <= read_num(n);
        }
        
        bool N::operator>=(const N& n) const {
            return read_num(*this) >= read_num(n);
        }
        
        bool N::operator<(const N& n) const {
            return read_num(*this) < read_num(n);
        }
        
        bool N::operator>(const N& n) const {
            return read_num(*this) > read_num(n);
        }
        
        N N::operator+(const N& n) const {
            return N{write_hexidecimal(read_num(*this) + read_num(n))};
        }
        
        N N::operator-(const N& n) const {
            return N{write_hexidecimal(read_num(*this) - read_num(n))};
        }
        
        N N::operator*(const N& n) const {
            return N{write_hexidecimal(read_num(*this) * read_num(n))};
        }
    
        N& N::operator++() {
            return *this = *this + 1;
        }
        
        N& N::operator--() {
            return *this = *this - 1;
        }
        
        N N::operator++(int) {
            N n = *this;
            ++(*this);
            return n;
        }
        
        N N::operator--(int) {
            N n = *this;
            --(*this);
            return n;
        }
        
        N N::operator<<(int i) const {
            return N{write_hexidecimal(read_num(*this) << i)};
        }
        
        N N::operator>>(int i) const {
            return N{write_hexidecimal(read_num(*this) >> i)};
        }
        
        N& N::operator+=(const N& n) {
            return *this = *this + n;
        }
        
        N& N::operator-=(const N& n) {
            return *this = *this - n;
        }
        
        N& N::operator*=(const N& n) {
            return *this = *this * n;
        }
        
        N& N::operator<<=(int i) {
            return *this = *this << i;
        }
        
        N& N::operator>>=(int i) {
            return *this = *this >> i;
        }
        
        math::division<N, uint64> N::divide(uint64 x) const {
            if (x == 0) throw math::division_by_zero{};
            // it is important to have this optimization. 
            if (x == 16) {
                int last = string::size() - 1;
                return math::division<N, uint64>{
                    N{std::string{"0x0"} + string::substr(2, last - 2)}, 
                    static_cast<uint64>(digit(string::operator[](last)))};
            }
            
            math::division<nat> div = read_num(*this).divide(nat{x});
            return math::division<N, uint64>{write_hexidecimal(div.Quotient), uint64(div.Remainder)};
        }
        
        std::ostream &write(std::ostream &o, const math::Z_bytes<endian::little> &z) {
            return o << "0x" << hex::write(z.words().reverse());
        }
        
        std::ostream &write(std::ostream &o, const math::Z_bytes<endian::big> &z) {
            return o << "0x" << hex::write(z.words().reverse());
        }
        
    }
    
    namespace integer {
        
        std::ostream &write(std::ostream &, const math::Z_bytes<endian::little> &) {
            throw "incomplete method";
        }
        
        std::ostream &write(std::ostream &, const math::Z_bytes<endian::big> &) {
            throw "incomplete method";
        }
        
    }
    
    /*
    namespace natural {
        
        ptr<bytes> read(string_view s, endian::order r) {
            if (!valid(s)) return nullptr;
            if (hexidecimal::valid(s)) return hexidecimal::read(s, r);
            return decimal::read(s, r);
        }
        
    }
    
    namespace integer {
        
        ptr<bytes> read(string_view s, endian::order r) {
            //std::cout << "reading in integer string \"" << s << "\"" << std::endl;
            if (!valid(s)) return nullptr;
            if (hexidecimal::valid(s)) return hexidecimal::read(s, r);
            if (negative(s)) {
                //std::cout << "reading in negative decimal integer string \"" << s << "\"" << std::endl;
                math::number::Z z{s};
                //std::cout << "reading in Z " << z << std::endl;
                math::number::Z_bytes<endian::little> n{z};
                //std::cout << "reading in Z_bytes " << n << std::endl;
                if (r == endian::big) std::reverse(n.begin(), n.end());
                //std::cout << "reading in negative decimal integer string ; about to return" << std::endl;
                return std::make_shared<bytes>(bytes_view(n));
            }
            return decimal::read(s, r);
        }
        
    }*/
    
}


