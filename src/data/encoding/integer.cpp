// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/encoding/digits.hpp>
#include <data/numbers.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        // x is automatically converted to GMP.
        string::string (uint64 x) : string {write (x)} {}
        
        // TODO it should be possible to compare decimal strings 
        // with basic functions in math::arithmetic.
        std::strong_ordering N_compare (string_view a, string_view b) {
            std::strong_ordering cmp_size = a.size () <=> b.size ();
            if (cmp_size != std::strong_ordering::equal) return cmp_size;
            
            for (int i = 0; i < a.size (); i++) {
                std::strong_ordering cmp_chr = digit (a[i]) <=> digit (b[i]);
                if (cmp_chr != std::strong_ordering::equal) return cmp_chr;
            }
            
            return std::strong_ordering::equal;
        }
        
        std::strong_ordering operator <=> (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            if (!n.valid ()) throw exception {} << "invalid decimal string: " << n;
            return N_compare (m, n);
        }
        
        char N_increment (string &x) {
            auto characters = decimal::characters ();
            
            auto i = x.rbegin ();
            auto e = x.rend ();
            char remainder = 1;
            while (i != e) {
                auto d = digit (*i) + remainder;
                *i = characters[d % 10];
                remainder = d / 10;
                if (remainder == 0) return '0';
                i++;
            }
            return characters[remainder];
        }
        
        string &operator ++ (string &x) {
            if (!x.valid ()) throw exception {} << "invalid decimal string: " << x;
            char remainder = N_increment (x);
            if (remainder == '0') return x;
            string new_x;
            new_x.resize (x.size () + 1);
            new_x[0] = remainder;
            std::copy (x.begin (), x.end (), new_x.begin () + 1);
            return x = new_x;
        }
        
        void N_decrement (string &x) {
            auto characters = decimal::characters ();
            
            auto i = x.rbegin ();
            auto e = x.rend ();
            while (i != e) {
                auto d = digit (*i);

                if (d != 0) {
                    *i = characters[d - 1];
                    break;
                }

                *i = characters[9];
                i++;
            }
        }
        
        string &operator -- (string &x) {
            if (!x.valid ()) throw exception {} << "invalid decimal string: " << x;
            
            if (x == "0") return x;
            if (x == "1") return x = string {"0"};
            
            N_decrement (x);
            
            if (!valid (x)) return x = string {x.substr (1)};
            return x;
        }
        
        string operator + (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            if (!n.valid ()) throw exception {} << "invalid decimal string: " << n;
            return decimal::write (N {m} + N {n});
        }
        
        string operator - (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            if (!n.valid ()) throw exception {} << "invalid decimal string: " << n;
            return decimal::write (N {m} - N {n});
        }
        
        string operator * (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            if (!n.valid ()) throw exception {} << "invalid decimal string: " << n;
            return decimal::write (N {m} * N {n});
        }
        
        string operator << (const string &m, int i) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            return decimal::write (N {m} << i);
        }
        
        string operator >> (const string &m, int i) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            return decimal::write (N {m} >> i);
        }
        
        string operator & (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            if (!n.valid ()) throw exception {} << "invalid decimal string: " << n;
            return decimal::write (math::N_bytes<endian::little>::read (m) & math::N_bytes<endian::little>::read (n));
        }
        
        string operator | (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid decimal string: " << m;
            if (!n.valid ()) throw exception {} << "invalid decimal string: " << n;
            return decimal::write(math::N_bytes<endian::little>::read (m) | math::N_bytes<endian::little>::read (n));
        }
        
        math::division<string, N> divide (const string &n, const N &x) {
            if (x == 0) throw math::division_by_zero {};
            // it is important to have this optimization. 
            // I can't say why or I'll be embarrassed. 
            if (x == 10) {
                int last = n.size () - 1;
                return math::division<string, N> {n.size () == 1 ? string {} : string (n.substr (0, last)), N (digit (n[last]))};
            }
            
            math::division<N> div = math::number::natural_divide (N {n}, x);
            
            return math::division<string, N> {decimal::write (div.Quotient), div.Remainder};
        }
        
        math::division<string, uint64> string::divide (uint64 x) const {
            math::division<string, N> div = decimal::divide (*this, N {x});
            return math::division<string, uint64> {div.Quotient, uint64 (div.Remainder)};
        }
        
        string operator / (const string &m, const string &x) {
            return decimal::write (math::number::natural_divide (N {m}, N {x}).Quotient);
        }
        
        string operator % (const string &m, const string &x) {
            return decimal::write (math::number::natural_divide (N {m}, N {x}).Remainder);
        }
        
        bool string::operator == (uint64 x) const {
            return *this == string {x};
        }
        
        std::strong_ordering string::operator <=> (uint64 x) const {
            return *this <=> string {x};
        }
        
        string::operator double () const {
            return double (N {*this});
        }
        
        string string::read (string_view x) {
            return decimal::write (N {x});
        }
    
        signed_decimal::string operator - (const string &x) {
            signed_decimal::string z;
            z.resize (x.size () + 1);
            z[0] = '-';
            std::copy (x.begin (), x.end (), z.begin () + 1);
            return z;
        }
    
    }
    
    namespace signed_decimal {
        
        // x is automatically converted to GMP.
        string::string (int64 x) : string {write (x)} {}
        
        // TODO it should be possible to compare decimal strings 
        // with basic functions in math::arithmetic.
        std::strong_ordering operator <=> (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid hexidecimal string: \"" << m << "\"";
            if (!n.valid ()) throw exception {} << "invalid hexidecimal string: \"" << n << "\"";
            
            math::signature na = sign (m);
            math::signature nb = sign (n);
            
            return na != nb ? na <=> nb :
                na == math::negative && nb == math::negative ? 
                decimal::N_compare (string_view {n.data () + 1, n.size () - 1}, string_view {m.data () + 1, m.size () - 1}) :
                decimal::N_compare (string_view {m.data (), m.size ()}, string_view {n.data (), n.size ()});
            
        }
        
        string &operator ++ (string &x) {
            if (is_negative (x)) {
                auto z = decimal::string {x.substr (1)};
                return x = -string {--z};
            }
            
            auto z = decimal::string {x};
            return x = string {++z};
        }
        
        string &operator -- (string &x) {
            if (!x.valid ()) throw exception {} << "invalid signed decimal string: \"" << x << "\"";
            
            if (!nonzero (x)) return x = string {"-1"};
            
            if (positive (x)) {
                auto z = decimal::string {x};
                return x = string {--z};
            }
            
            auto z = decimal::string {x.substr (1)};
            return x = -string {++z};
        }
        
        string operator + (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            if (!n.valid ()) throw exception {} << "invalid signed decimal string: \"" << n << "\"";
            return signed_decimal::write (Z::read (m) + Z::read (n));
        }
        
        string operator - (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            if (!n.valid ()) throw exception {} << "invalid signed decimal string: \"" << n << "\"";
            return signed_decimal::write (Z::read (m) - Z::read (n));
        }
        
        string operator * (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            if (!n.valid ()) throw exception {} << "invalid signed decimal string: \"" << n << "\"";
            return signed_decimal::write (Z::read (m) * Z::read (n));
        }
        
        string operator << (const string &m, int i) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            return signed_decimal::write (Z::read (m) << i);
        }
        
        string operator >> (const string &m, int i) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            return signed_decimal::write (Z::read (m) >> i);
        }
        
        string operator & (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            if (!n.valid ()) throw exception {} << "invalid signed decimal string: \"" << n << "\"";
            throw method::unimplemented {"signed_dec &"};
        }
        
        string operator | (const string &m, const string &n) {
            if (!m.valid ()) throw exception {} << "invalid signed decimal string: \"" << m << "\"";
            if (!n.valid ()) throw exception {} << "invalid signed decimal string: \"" << n << "\"";
            throw method::unimplemented {"signed_dec |"};
        }
        
        bool string::operator == (int64 x) const {
            return *this == string {x};
        }
        
        std::strong_ordering string::operator <=> (int64 x) const {
            return *this <=> string {x};
        }
        
        string::operator double () const {
            return double (N {*this});
        }
    
        string operator - (const string &n) {
            string x;
            if (negative (n)) {
                x.resize (n.size () - 1);
                std::copy (n.begin () + 1, n.end (), x.begin ());
            } if (positive (n)) {
                x[0] = '-';
                x.resize (n.size () + 1);
                std::copy (n.begin (), n.end (), x.begin () + 1);
            }
            return x;
        }
        
        string string::read (string_view x) {
            return signed_decimal::write (Z::read (x));
        }
    
    }
    
}

