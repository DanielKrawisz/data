    // Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/encoding/digits.hpp>

namespace data::encoding::base58 {
    using nat = math::N;
    
    string::string() : std::string{""} {}
    
    string::string(const std::string &x) : std::string{base58::valid(x) ? x : ""} {}
    
    string::string(uint64 x) : string{encode(nat{x})} {}
    
    inline nat read_num(const string& n) {
        return read_base<nat>(n, 58, &digit);
    }
    
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
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        if (!n.valid()) throw exception{} << "invalid base 58 string: \"" << n << "\"";
        return N_compare(m, n);
    }
    
    char N_increment(string &x) {
        auto characters = base58::characters();
        
        auto i = x.rbegin();
        auto e = x.rend();
        char remainder = 1;
        while (i != e) {
            auto d = digit(*i) + remainder;
            *i = characters[d % 58];
            remainder = d / 58;
            if (remainder == 0) return '1';
            i++;
        }
        return characters[remainder];
    }
    
    string &operator++(string &x) {
        if (!x.valid()) throw exception{} << "invalid base 58 string: \"" << x << "\"";
        char remainder = N_increment(x);
        if (remainder == '1') return x;
        string new_x;
        new_x.resize(x.size() + 1);
        new_x[0] = remainder;
        std::copy(x.begin(), x.end(), new_x.begin() + 1);
        return x = new_x;
    }
    
    void N_decrement(string &x) {
        if (!x.valid()) throw exception{} << "invalid base 58 string: \"" << x << "\"";
        auto characters = base58::characters();
        
        auto i = x.rbegin();
        auto e = x.rend();
        while (i != e) {
            auto d = digit(*i);
            if (d != 0) {
                *i = characters[d - 1];
                break;
            }
            *i = characters[57];
            i++;
        }
    }
    
    string &operator--(string &x) {
        if (!x.valid()) throw exception{} << "invalid base 58 string: \"" << x << "\"";
        if (x == "") return x;
        if (x == "2") return x = string{""};
            
        N_decrement(x);
        
        if (!valid(x)) return x = string{x.substr(1)};
        return x;
    }
    
    string operator+(const string& m, const string& n) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        if (!n.valid()) throw exception{} << "invalid base 58 string: \"" << n << "\"";
        
        return encode(read_num(m) + read_num(n));
    }
    
    string operator-(const string& m, const string& n) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        if (!n.valid()) throw exception{} << "invalid base 58 string: \"" << n << "\"";
        
        return encode(read_num(m) - read_num(n));
    }
    
    string operator|(const string& m, const string& n) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        if (!n.valid()) throw exception{} << "invalid base 58 string: \"" << n << "\"";
        
        return encode(read_base<N_bytes_little>(m, 58, &digit) | read_base<N_bytes_little>(n, 58, &digit));
    }
    
    string operator&(const string& m, const string& n) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        if (!n.valid()) throw exception{} << "invalid base 58 string: \"" << n << "\"";
        
        return encode(read_base<N_bytes_little>(m, 58, &digit) & read_base<N_bytes_little>(n, 58, &digit));
    }
    
    string operator*(const string& m, const string& n) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        if (!n.valid()) throw exception{} << "invalid base 58 string: \"" << n << "\"";
        
        return encode(read_num(m) * read_num(n));
    }
    
    string operator<<(const string& m, int i) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        
        return encode(read_num(m) << i);
    }
    
    string operator>>(const string& m, int i) {
        if (!m.valid()) throw exception{} << "invalid base 58 string: \"" << m << "\"";
        
        return encode(read_num(m) >> i);
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
        return math::division<string, uint64>{encode(div.Quotient), uint64(div.Remainder)};
    }
    
    string string::read(const std::string &x) {
        return encode(nat::read(x));
    }

}
