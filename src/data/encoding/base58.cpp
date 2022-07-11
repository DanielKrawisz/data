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
    
    uint32_little check::sum(bytes_view b) {
        uint32_little x;
        crypto::digest<32> digest = crypto::hash::Bitcoin_256(b);
        std::copy(digest.begin(), digest.begin() + 4, x.begin());
        return x;
    }

    bytes append_checksum(bytes_view b) {
        bytes checked(b.size() + 4);
        bytes_writer w(checked.begin(), checked.end());
        w << b << check::sum(b);
        return checked;
    }
    
    ptr<bytes> remove_checksum(bytes_view b) {
        if (b.size() < 4) return nullptr;
        uint32_little x;
        std::copy(b.end() - 4, b.end(), x.begin());
        bytes_view without = b.substr(0, b.size() - 4);
        if (x != check::sum(without)) return nullptr;
        return std::make_shared<bytes>(without);
    }
    
    ptr<bytes> check::decode(string_view s) {
        size_t leading_ones = 0;
        while(leading_ones < s.size() && s[leading_ones] == '1') leading_ones++;
        encoding::base58::view b58(s.substr(leading_ones));
        if (!b58.valid()) return {};
        bytes_view decoded = bytes_view(b58);
        return remove_checksum(bytes::write(leading_ones + decoded.size(), bytes(leading_ones, 0x00), decoded));
    }
    
    check check::encode(bytes_view b) {
        bytes data = append_checksum(b);
        size_t leading_zeros = 0;
        while (leading_zeros < data.size() && data[leading_zeros] == 0) leading_zeros++;
        std::string b58 = data::encoding::base58::write(bytes_view(data).substr(leading_zeros));
        std::string ones(leading_zeros, '1');
        std::stringstream ss;
        ss << ones << b58;
        return check{ss.str()};
    }

    check check::recover(const string_view invalid) {
        
        {
            check x(invalid);
            if (x.valid()) return x;
        }
        
        std::string test{invalid};
        
        std::string characters = data::encoding::base58::characters();
        
        // replacements
        for (int i = 0; i < test.size(); i++) {
            std::string replace = test;
            
            for (char c : characters) {
                if (replace[i] == c) continue;
                replace[i] = c;
                check x(replace);
                if (x.valid()) return x;
            }
        }
        
        // insertions
        for (int i = 0; i <= test.size(); i++) {
            std::string insert{};
            insert.resize(test.size() + 1);
            std::copy(test.begin(), test.begin() + i, insert.begin());
            std::copy(test.begin() + i, test.end(), insert.begin() + i + 1);
            
            for (char c : characters) {
                insert[i] = c;
                check x(insert);
                if (x.valid()) return x;
            }
        }
        
        // deletions 
        for (int i = 0; i < test.size(); i++) {
            std::string deletions{};
            deletions.resize(test.size() - 1);
            
            std::copy(test.begin(), test.begin() + i, deletions.begin());
            std::copy(test.begin() + i + 1, test.end(), deletions.begin() + i);
            
            check x(deletions);
            if (x.valid()) return x;
            
        }
        
        return {};
        
    }

}
