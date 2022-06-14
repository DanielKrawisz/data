// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/encoding/digits.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        ptr<bytes> read(string_view s, endian::order r) {
            if (!valid(s)) return nullptr;

            N_bytes_big n = N_bytes_big(N::read(s));
            if (r == endian::little) std::reverse(n.begin(), n.end());
            return std::make_shared<bytes>(static_cast<bytes>(n));
        }
        
        template <typename N>
        string write_decimal(const N& n) {
            if (n == 0) return string{"0"};
            static std::string Characters = characters();
            return string{write_base<N>(n, Characters)};
        }
        
        string::string(uint64 x) : string{write_decimal(x)} {}
        
        inline N read_num(const string& n) {
            return read_base<N>(n, 10, &digit);
        }
        
        // TODO it should be possible to compare decimal strings 
        // with basic functions in math::arithmetic.
        std::strong_ordering string::operator<=>(const string& x) const {
            if (!this->valid() || !x.valid()) throw std::invalid_argument{"invalid dec string"};
            auto n = read_num(*this) <=> read_num(x);
            if (n == std::weak_ordering::greater) return std::strong_ordering::greater;
            if (n == std::weak_ordering::less) return std::strong_ordering::less;
            return std::strong_ordering::equal;
        }
        
        string string::operator+(const string& n) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return write_decimal(read_num(*this) + read_num(n));
        }
        
        string string::operator-(const string& n) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return write_decimal(read_num(*this) - read_num(n));
        }
        
        string string::operator*(const string& n) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            return write_decimal(read_num(*this) * read_num(n));
        }
        
        string string::operator<<(int i) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            return write_decimal(read_num(*this) << i);
        }
        
        string string::operator>>(int i) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            return write_decimal(read_num(*this) >> i);
        }
        
        string string::operator&(const string& n) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            throw method::unimplemented{"dec &"};
        }
        
        string string::operator|(const string& n) const {
            if (!this->valid()) throw std::invalid_argument{"invalid dec string"};
            if (!n.valid()) throw std::invalid_argument{"invalid dec string"};
            throw method::unimplemented{"dec |"};
        }
        
        math::division<string, N> divide(const string &n, const N &x) {
            if (x == 0) throw math::division_by_zero{};
            // it is important to have this optimization. 
            // I can't say why or I'll be embarrassed. 
            if (x == 10) {
                int last = n.size() - 1;
                return math::division<string, N>{n.size() == 1 ? string{} : string(n.substr(0, last)), N(digit(n[last]))};
            }
            
            math::division<N> div = math::divide_unsigned(read_num(n), x);
            
            return math::division<string, N>{write(div.Quotient), div.Remainder};
        }
        
        math::division<string, uint64> string::divide(uint64 x) const {
            math::division<string, N> div = decimal::divide(*this, N{x});
            return math::division<string, uint64>{div.Quotient, uint64(div.Remainder)};
        }
        
        string string::operator/(const string &x) const {
            return decimal::divide(*this, N::read(x)).Quotient;
        }
        
        string string::operator%(const string &x) const {
            return write(decimal::divide(*this, N::read(x)).Remainder);
        }
        
        bool string::operator==(uint64 x) const {
            return *this == string{x};
        }
        
        std::strong_ordering string::operator<=>(uint64 x) const {
            return *this <=> string{x};
        }
        
        string string::operator+(uint64 x) const {
            return *this + string{x};
        }
        
        string string::operator-(uint64 x) const {
            return *this - string{x};
        }
        
        string string::operator*(uint64 x) const {
            return *this * string{x};
        }
        
        string &string::operator+=(uint64 x) {
            return *this += string{x};
        }
        
        string &string::operator-=(uint64 x) {
            return *this -= string{x};
        }
        
        string &string::operator*=(uint64 x) {
            return *this *= string{x};
        }
        
        string::operator double() const {
            return double(N::read(*this));
        }
    
    }
    
    namespace hexidecimal {
        
        hex::letter_case read_case(string_view s) {
            bool u = ctre::match<upper_case_pattern>(s);
            bool l = ctre::match<upper_case_pattern>(s);
            if ((u && l) || (!u && !l)) return hex::unknown;
            return u ? hex::upper : hex::lower;
        }
        
    }
    
}
