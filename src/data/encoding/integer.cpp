// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/digits.hpp>
#include <data/numbers.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        string::string(uint64 x) : string{decimal::write(x)} {}
        
        // TODO it should be possible to compare decimal strings 
        // with basic functions in math::arithmetic.
        std::strong_ordering N_compare(string_view a, string_view b) {
            std::strong_ordering cmp_size = a.size() <=> b.size();
            if (cmp_size != std::strong_ordering::equal) return cmp_size;
            
            for (int i = 0; i < a.size(); i++) {
                std::strong_ordering cmp_chr = digit(a[i]) <=> digit(b[i]);
                if (cmp_chr != std::strong_ordering::equal) return cmp_chr;
            }
            
            return std::strong_ordering::equal;
        }
        
        std::strong_ordering operator<=>(const string &m, const string &n) {
            if (!m.valid() || !n.valid()) throw std::invalid_argument{"invalid decimal string"};
            return N_compare(m, n);
        }
        
        char N_increment(string &x) {
            auto characters = decimal::characters();
            
            auto i = x.rbegin();
            auto e = x.rend();
            char remainder = 1;
            while (i != e) {
                auto d = digit(*i) + remainder;
                *i = characters[d % 10];
                remainder = d / 10;
                if (remainder == 0) return '0';
                i++;
            }
            return characters[remainder];
        }
        
        string &operator++(string &x) {
            if (!x.valid()) throw std::invalid_argument{"invalid dec string"};
            char remainder = N_increment(x);
            if (remainder == '0') return x;
            string new_x;
            new_x.resize(x.size() + 1);
            new_x[0] = remainder;
            std::copy(x.begin(), x.end(), new_x.begin() + 1);
            return x = new_x;
        }
        
        void N_decrement(string &x) {
            auto characters = decimal::characters();
            
            auto i = x.rbegin();
            auto e = x.rend();
            while (i != e) {
                auto d = digit(*i);
                if (d != 0) {
                    *i = characters[d - 1];
                    break;
                }
                *i = characters[9];
                i++;
            }
        }
        
        string &operator--(string &x) {
            if (!x.valid()) throw std::invalid_argument{"invalid dec string"};
            if (x == "0") return x;
            if (x == "1") return x = string{"0"};
            
            N_decrement(x);
            
            if (!valid(x)) return x = string{x.substr(1)};
            return x;
        }
        
        string operator+(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(N::read(m) + N::read(n));
        }
        
        string operator-(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(N::read(m) - N::read(n));
        }
        
        string operator*(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(N::read(m) * N::read(n));
        }
        
        string operator<<(const string &m, int i) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(N::read(m) << i);
        }
        
        string operator>>(const string &m, int i) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(N::read(m) >> i);
        }
        
        string operator&(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(math::N_bytes<endian::little>::read(m) & math::N_bytes<endian::little>::read(n));
        }
        
        string operator|(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return decimal::write(math::N_bytes<endian::little>::read(m) | math::N_bytes<endian::little>::read(n));
        }
        
        math::division<string, N> divide(const string &n, const N &x) {
            if (x == 0) throw math::division_by_zero{};
            // it is important to have this optimization. 
            // I can't say why or I'll be embarrassed. 
            if (x == 10) {
                int last = n.size() - 1;
                return math::division<string, N>{n.size() == 1 ? string{} : string(n.substr(0, last)), N(digit(n[last]))};
            }
            
            math::division<N> div = math::number::natural::divide(N::read(n), x);
            
            return math::division<string, N>{decimal::write(div.Quotient), div.Remainder};
        }
        
        math::division<string, uint64> string::divide(uint64 x) const {
            math::division<string, N> div = decimal::divide(*this, N{x});
            return math::division<string, uint64>{div.Quotient, uint64(div.Remainder)};
        }
        
        string operator/(const string &m, const string &x) {
            return decimal::write(math::number::natural::divide(N::read(m), N::read(x)).Quotient);
        }
        
        string operator%(const string &m, const string &x) {
            return decimal::write(math::number::natural::divide(N::read(m), N::read(x)).Remainder);
        }
        
        bool string::operator==(uint64 x) const {
            return *this == string{x};
        }
        
        std::strong_ordering string::operator<=>(uint64 x) const {
            return *this <=> string{x};
        }
        
        string::operator double() const {
            return double(N::read(*this));
        }
        
        string string::read(string_view x) {
            return decimal::write(N::read(x));
        }
    
        signed_decimal::string operator-(const string &x) {
            signed_decimal::string z;
            z.resize(x.size() + 1);
            z[0] = '-';
            std::copy(x.begin(), x.end(), z.begin() + 1);
            return z;
        }
    
    }
    
    namespace signed_decimal {
        
        string::string(int64 x) : string{signed_decimal::write(x)} {}
        
        // TODO it should be possible to compare decimal strings 
        // with basic functions in math::arithmetic.
        std::strong_ordering operator<=>(const string& m, const string& n) {
            if (!m.valid() || !n.valid()) throw std::invalid_argument{"invalid hexidecimal string"};
            math::sign na = sign(m);
            math::sign nb = sign(n);
            
            return na != nb ? na <=> nb :
                na == math::negative && nb == math::negative ? 
                decimal::N_compare(string_view{n.data() + 1, n.size() - 1}, string_view{m.data() + 1, m.size() - 1}) : 
                decimal::N_compare(string_view{m.data(), m.size()}, string_view{n.data(), n.size()});
            
        }
        
        string &operator++(string &x) {
            if (math::number::is_negative(x)) {
                auto z = decimal::string{x.substr(1)};
                return x = -string{--z};
            }
            
            auto z = decimal::string{x};
            return x = string{++z};
        }
        
        string &operator--(string &x) {
            if (!x.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            
            if (!nonzero(x)) return x = string{"-1"};
            
            if (positive(x)) {
                auto z = decimal::string{x};
                return x = string{--z};
            }
            
            auto z = decimal::string{x.substr(1)};
            return x = -string{++z};
        }
        
        string operator+(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            return signed_decimal::write(Z::read(m) + Z::read(n));
        }
        
        string operator-(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            return signed_decimal::write(Z::read(m) - Z::read(n));
        }
        
        string operator*(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            return signed_decimal::write(Z::read(m) * Z::read(n));
        }
        
        string operator<<(const string &m, int i) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            return signed_decimal::write(Z::read(m) << i);
        }
        
        string operator>>(const string &m, int i) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            return signed_decimal::write(Z::read(m) >> i);
        }
        
        string operator&(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            throw method::unimplemented{"signed_dec &"};
        }
        
        string operator|(const string &m, const string& n) {
            if (!m.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid +/-dec string"};
            throw method::unimplemented{"signed_dec |"};
        }
        
        bool string::operator==(int64 x) const {
            return *this == string{x};
        }
        
        std::strong_ordering string::operator<=>(int64 x) const {
            return *this <=> string{x};
        }
        
        string::operator double() const {
            return double(N::read(*this));
        }
    
        string operator-(const string &n) {
            string x;
            if (negative(n)) {
                x.resize(n.size() - 1);
                std::copy(n.begin() + 1, n.end(), x.begin());
            } if (positive(n)) {
                x[0] = '-';
                x.resize(n.size() + 1);
                std::copy(n.begin(), n.end(), x.begin() + 1);
            }
            return x;
        }
        
        string string::read(string_view x) {
            return signed_decimal::write(Z::read(x));
        }
    
    }
    
    namespace hexidecimal {
        
        using nat = math::N;
        
        template <typename N>
        std::string write_hexidecimal(const N& n) {
            static std::string Characters = hex::characters_lower();
            if (n == 0) return "0x";
            std::string p = write_base<N>(n, Characters);
            if ((p.size() % 2) == 1) return std::string{"0x0"} + p;
            return std::string{"0x"} + p;
        }
    
        std::weak_ordering N_compare_same_size(string_view a, string_view b) {
            for (int i = 0; i < a.size(); i++) 
                if (digit(a[i]) > digit(b[i])) return std::weak_ordering::greater; 
                else if (digit(a[i]) < digit(b[i])) return std::weak_ordering::less;
            
            return std::weak_ordering::equivalent;
        }
        
        std::weak_ordering N_compare(string_view a, string_view b) {
            if (a.size() < b.size()) return 0 <=> N_compare(b, a);
            size_t size_difference = a.size() - b.size();
            for (int i = 0; i < size_difference; i++) if (a[i] != '0') return std::weak_ordering::greater; 
            return N_compare_same_size(a.substr(size_difference, a.size()), b);
        }
        
        bool string::operator==(const string &b) const {
            return (*this <=> b) == 0;
        }
         
        std::weak_ordering string::operator<=>(const string &b) const {
            if (!this->valid() || !b.valid()) throw std::invalid_argument{"invalid hexidecimal string"};
            return N_compare(string_view{this->data() + 2, this->size() - 2}, string_view{b.data() + 2, b.size() - 2});
        }
        
        string::string(uint64 x) : std::string{write_hexidecimal(nat{x})} {}
        
        inline nat read_num(const std::string& n) {
            return read_base<nat>(n.substr(2), 16, &digit);
        }
        
        string string::operator+(const string& n) const {
            return string{write_hexidecimal(read_num(*this) + read_num(n))};
        }
        
        string string::operator-(const string& n) const {
            return string{write_hexidecimal(read_num(*this) - read_num(n))};
        }
        
        string string::operator*(const string& n) const {
            return string{write_hexidecimal(read_num(*this) * read_num(n))};
        }
    
        string& string::operator++() {
            return *this = *this + 1;
        }
        
        string& string::operator--() {
            return *this = *this - 1;
        }
        
        string string::operator++(int) {
            string n = *this;
            ++(*this);
            return n;
        }
        
        string string::operator--(int) {
            string n = *this;
            --(*this);
            return n;
        }
        
        string string::operator<<(int i) const {
            return string{write_hexidecimal(read_num(*this) << i)};
        }
        
        string string::operator>>(int i) const {
            return string{write_hexidecimal(read_num(*this) >> i)};
        }
        
        string& string::operator+=(const string& n) {
            return *this = *this + n;
        }
        
        string& string::operator-=(const string& n) {
            return *this = *this - n;
        }
        
        string& string::operator*=(const string& n) {
            return *this = *this * n;
        }
        
        string& string::operator<<=(int i) {
            return *this = *this << i;
        }
        
        string& string::operator>>=(int i) {
            return *this = *this >> i;
        }
        
        math::division<string, uint64> string::divide(uint64 x) const {
            if (x == 0) throw math::division_by_zero{};
            // it is important to have this optimization. 
            if (x == 16) {
                int last = string::size() - 1;
                return math::division<string, uint64>{
                    string{std::string{"0x0"} + string::substr(2, last - 2)}, 
                    static_cast<uint64>(digit(string::operator[](last)))};
            }
            
            math::division<nat> div = read_num(*this).divide(nat{x});
            return math::division<string, uint64>{string{write_hexidecimal(div.Quotient)}, uint64(div.Remainder)};
        }
        
        string string::read(string_view x) {
            return hexidecimal::write(N::read(x));
        }
        
    }
    
}

