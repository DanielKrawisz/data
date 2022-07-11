// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/integer.hpp>
#include <data/numbers.hpp>
#include <data/crypto/hash/hash.hpp>

namespace data::encoding::base58 {
    
    template <typename N>
    string write_b58(const N& n) {
        static std::string Characters = characters();
        if (n == 0) return string{"1"};
        return string{write_base<N>(n, Characters)};
    }
    
    string write(const bytes_view b) {
        return write<N>(N(math::number::N_bytes_big::read(b)));
    }
    
    view::view(string_view s) : string_view{s}, Bytes{}, ToBytes{nullptr} {
        if (base58::valid(s)) {
            Bytes = bytes(*hex::read(data::encoding::hexidecimal::write(read<N>(s)).substr(2)));
            ToBytes = &Bytes;
        }
    }
    
    string::string(uint64 x) : std::string{write_b58(N{x})} {}
    
    template <typename N>
    inline N read_num(const string& n) {
        return read_base<N>(n, 58, &digit);
    }
    
    std::strong_ordering string::operator<=>(const string& x) const {
        if (!this->valid() || !x.valid()) throw std::invalid_argument{"invalid base 58 string"};
        auto a = read_num<N>(*this);
        auto b = read_num<N>(x);
        return a == b ? std::strong_ordering::equal : a < b ? std::strong_ordering::less : std::strong_ordering::greater;
        /*
        auto n = read_num<N>(*this) <=> read_num<N>(x);
        if (n == std::weak_ordering::greater) return std::strong_ordering::greater;
        if (n == std::weak_ordering::less) return std::strong_ordering::less;
        return std::strong_ordering::equal;
        */
    }
    
    string string::operator<<(int i) const {
        if (!this->valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N>(read_num<N>(*this) << i)};
    }
    
    string string::operator>>(int i) const {
        if (!this->valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N>(read_num<N>(*this) >> i)};
    }
    
    string string::operator+(const string& n) const {
        if (!this->valid() || !n.valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N>(read_num<N>(*this) + read_num<N>(n))};
    }
    
    string string::operator-(const string& n) const {
        if (!this->valid() || !n.valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N>(read_num<N>(*this) - read_num<N>(n))};
    }
    
    string string::operator*(const string& n) const {
        if (!this->valid() || !n.valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N>(read_num<N>(*this) * read_num<N>(n))};
    }
    
    // these next two should work with N eventually. 
    string string::operator|(const string& n) const {
        if (!this->valid() || !n.valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N_bytes_little>(read_num<N_bytes_little>(*this) | read_num<N_bytes_little>(n))};
    }
    
    string string::operator&(const string& n) const {
        if (!this->valid() || !n.valid()) throw std::invalid_argument{"invalid base 58 string"};
        return string{write_b58<N_bytes_little>(read_num<N_bytes_little>(*this) & read_num<N_bytes_little>(n))};
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
        
        math::division<N> div = math::number::natural::divide(read_num<N>(*this), N{static_cast<uint64>(x)});
        return math::division<string, uint64>{write_b58<N>(div.Quotient), uint64(div.Remainder)};
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
        return double(read_num<N>(*this));
    }
    
    math::division<string> string::divide(const string &x) const {
        return math::number::natural::divide(*this, x);
    }

}
