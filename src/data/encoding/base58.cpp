    // Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/encoding/digits.hpp>

namespace data::encoding::base58 {
    
    string write(const bytes_view b) {
        return write<math::number::gmp::N>(math::number::gmp::N(math::number::N_bytes<endian::big>(b)));
    }
    
    view::view(string_view s) : string_view{s}, Bytes{}, ToBytes{nullptr} {
        if (base58::valid(s)) {
            Bytes = bytes(*hex::read(data::encoding::hexidecimal::write(read<math::number::gmp::N>(s)).substr(2)));
            ToBytes = &Bytes;
        }
    }
    
    template <typename N>
    std::string write_b58(const N& n) {
        static std::string Characters = characters();
        if (n == 0) return "1";
        return write_base<N>(n, Characters);
    }

    string::string() : std::string{"1"} {}
        
    string::string(const std::string& x) : std::string{base58::valid(x) ? x : ""} {}
    
    using nat = math::number::N;
    
    string::string(uint64 x) : std::string{write_b58(nat{x})} {}
    
    inline nat read_num(const string& n) {
        return read_base<nat>(n, 58, &digit);
    }
    
    bool string::operator<=(const string& n) const {
        return read_num(*this) <= read_num(n);
    }
    
    bool string::operator>=(const string& n) const {
        return read_num(*this) >= read_num(n);
    }
    
    bool string::operator<(const string& n) const {
        return read_num(*this) < read_num(n);
    }
    
    bool string::operator>(const string& n) const {
        return read_num(*this) > read_num(n);
    }
    
    string string::operator+(const string& n) const {
        return string{write_b58(read_num(*this) + read_num(n))};
    }
    
    string string::operator-(const string& n) const {
        return string{write_b58(read_num(*this) - read_num(n))};
    }
    
    string string::operator*(const string& n) const {
        return string{write_b58(read_num(*this) * read_num(n))};
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
        return string{write_b58(read_num(*this) << i)};
    }
    
    string string::operator>>(int i) const {
        return string{write_b58(read_num(*this) >> i)};
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
        if (x == 58) {
            int last = string::size() - 1;
            return math::division<string, uint64>{
                string{std::string::substr(0, last)}, 
                static_cast<uint64>(digit(std::string::operator[](last)))};
        }
        
        math::division<nat> div = read_num(*this).divide(nat{static_cast<uint64>(x)});
        return math::division<string, uint64>{write_b58(div.Quotient), uint64(div.Remainder)};
    }

}
