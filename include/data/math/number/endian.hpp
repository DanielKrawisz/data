// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <boost/endian/buffers.hpp>

namespace data {
    namespace number::endian {
        template <typename X> struct big;
        template <typename X> struct little;
    }
    namespace endian {
        template <typename X> number::endian::big<X> big(X x);
        template <typename X> number::endian::little<X> little(X x);
    }
    namespace number::endian {

        template <typename X>
        struct big {
            X Value;
            
            big() : big{0} {}
            big(X x) : Value{x} {}
            
            big& operator=(big& b);
            
            bool operator==(const big&) const;
            bool operator!=(const big&) const;
            bool operator<(const big&) const;
            bool operator>(const big&) const;
            bool operator<=(const big&) const;
            bool operator>=(const big&) const;
            
            big operator~() const;
            big operator|(const big&) const;
            big operator^(const big&) const;
            
            big operator+(const big&) const;
            big operator-(const big&) const;
            big operator*(const big&) const;
            big operator/(const big&) const;
            big operator%(const big&) const;
            
            big operator<<(const big&) const;
            big operator>>(const big&) const;
            
            big& operator|=(const big&);
            big& operator^=(const big&);
            
            big& operator+=(const big&);
            big& operator-=(const big&);
            big& operator*=(const big&);
            big& operator/=(const big&);
            big& operator%=(const big&);
            
            big& operator<<=(const big&);
            big& operator>>=(const big&);
        };
            
        template <typename X>
        struct little {
            X Value;
            
            little() : little{0} {}
            little(X x) : Value{x} {}
            
            little& operator=(little& b);
            
            bool operator==(const little&) const;
            bool operator!=(const little&) const;
            bool operator<(const little&) const;
            bool operator>(const little&) const;
            bool operator<=(const little&) const;
            bool operator>=(const little&) const;
            
            little operator~() const;
            little operator|(const little&) const;
            little operator^(const little&) const;
            
            little operator+(const little&) const;
            little operator-(const little&) const;
            little operator*(const little&) const;
            little operator/(const little&) const;
            little operator%(const little&) const;
            
            little operator<<(const little&) const;
            little operator>>(const little&) const;
            
            little& operator|=(const little&);
            little& operator^=(const little&);
            
            little& operator+=(const little&);
            little& operator-=(const little&);
            little& operator*=(const little&);
            little& operator/=(const little&);
            little& operator%=(const little&);
            
            little& operator<<=(const little&);
            little& operator>>=(const little&);
        };
        
        template <typename X>
        inline big<X>& big<X>::operator=(big& n) {
            Value = n.Value;
            return *this;
        }
        
        template <typename X>
        inline bool big<X>::operator==(const big& n) const {
            return Value == n.Value;
        }
        
        template <typename X>    
        inline bool big<X>::operator!=(const big& n) const {
            return Value != n.Value;
        }
        
        template <typename X>
        inline bool big<X>::operator<(const big& n) const {
            return boost::endian::big_to_native(Value) < boost::endian::big_to_native(n.Value);
        }
        
        template <typename X>
        inline bool big<X>::operator>(const big& n) const {
            return boost::endian::big_to_native(Value) < boost::endian::big_to_native(n.Value);
        }
        
        template <typename X>
        inline bool big<X>::operator<=(const big& n) const {
            return boost::endian::big_to_native(Value) < boost::endian::big_to_native(n.Value);
        }
        
        template <typename X>
        inline bool big<X>::operator>=(const big& n) const {
            return boost::endian::big_to_native(Value) < boost::endian::big_to_native(n.Value);
        }
        
        template <typename X>
        inline bool little<X>::operator==(const little& n) const {
            return Value == n.Value;
        }
        
        template <typename X>    
        inline bool little<X>::operator!=(const little& n) const {
            return Value != n.Value;
        }
        
        template <typename X>
        inline bool little<X>::operator<(const little& n) const {
            return boost::endian::little_to_native(Value) < boost::endian::little_to_native(n.Value);
        }
        
        template <typename X>
        inline bool little<X>::operator>(const little& n) const {
            return boost::endian::little_to_native(Value) < boost::endian::little_to_native(n.Value);
        }
        
        template <typename X>
        inline bool little<X>::operator<=(const little& n) const {
            return boost::endian::little_to_native(Value) < boost::endian::little_to_native(n.Value);
        }
        
        template <typename X>
        inline bool little<X>::operator>=(const little& n) const {
            return boost::endian::little_to_native(Value) < boost::endian::little_to_native(n.Value);
        }
        
        template <typename X>
        inline big<X> big<X>::operator~() const {
            return ~Value;
        }
        
        template <typename X>
        inline big<X> big<X>::operator|(const big& n) const {
            return Value | n.Value;
        }
        
        template <typename X>
        inline big<X> big<X>::operator^(const big& n) const {
            return Value ^ n.Value;
        }
        
        template <typename X>
        inline little<X> little<X>::operator~() const {
            return ~Value;
        }
        
        template <typename X>
        inline little<X> little<X>::operator|(const little& n) const {
            return Value | n.Value;
        }
        
        template <typename X>
        inline little<X> little<X>::operator^(const little& n) const {
            return Value ^ n.Value;
        }
        
        template <typename X>
        inline big<X> big<X>::operator+(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) + boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X> big<X>::operator-(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) - boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X> big<X>::operator*(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) * boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X> big<X>::operator/(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) / boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X> big<X>::operator%(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) % boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator+(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) + boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator-(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) - boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator*(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) * boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator/(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) / boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator%(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) % boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X> big<X>::operator<<(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) << boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X> big<X>::operator>>(const big& n) const {
            return boost::endian::native_to_big(
                boost::endian::big_to_native(Value) >> boost::endian::big_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator<<(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) << boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline little<X> little<X>::operator>>(const little& n) const {
            return boost::endian::native_to_little(
                boost::endian::little_to_native(Value) >> boost::endian::little_to_native(n.Value));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator|=(const big& n) {
            return operator=(operator|(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator^=(const big& n) {
            return operator=(operator^(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator+=(const big& n) {
            return operator=(operator+(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator-=(const big& n) {
            return operator=(operator-(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator*=(const big& n) {
            return operator=(operator*(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator/=(const big& n) {
            return operator=(operator/(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator%=(const big& n) {
            return operator=(operator%(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator<<=(const big& n) {
            return operator=(operator<<(n));
        }
        
        template <typename X>
        inline big<X>& big<X>::operator>>=(const big& n) {
            return operator=(operator>>(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator|=(const little& n) {
            return operator=(operator|(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator^=(const little& n) {
            return operator=(operator^(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator+=(const little& n) {
            return operator=(operator+(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator-=(const little& n) {
            return operator=(operator-(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator*=(const little& n) {
            return operator=(operator*(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator/=(const little& n) {
            return operator=(operator/(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator%=(const little& n) {
            return operator=(operator%(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator<<=(const little& n) {
            return operator=(operator<<(n));
        }
        
        template <typename X>
        inline little<X>& little<X>::operator>>=(const little& n) {
            return operator=(operator>>(n));
        }
    }
    
    namespace endian {
        template <typename X>
        inline number::endian::big<X> big(X x) {
            return number::endian::big<X>{boost::endian::native_to_big(x)};
        }
        
        template <typename X>
        inline number::endian::little<X> little(X x) {
            return number::endian::little<X>{boost::endian::native_to_big(x)};
        }
    }

}

#endif

