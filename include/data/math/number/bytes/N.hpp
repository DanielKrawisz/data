// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <limits>

#include <data/tools/linked_stack.hpp>

#include <data/math/number/natural.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/bytestring.hpp>

namespace data::math::number {
    
    template <endian::order r>
    struct N_bytes : protected bytestring<r> {
        
        N_bytes() : bytestring<r>{} {}
        
        N_bytes(const uint64 x) : bytestring<r>(8, 0x00) {
            *(uint64*)(bytestring<r>::data()) = endian::native<uint64, r>{}.from(x);
        }
        
        static N_bytes read(string_view x) {
            if (x.size() == 0) return 0;
            ptr<bytes> b = encoding::natural::read(x, r);
            if (b == nullptr) return {};
            return N_bytes<r>{*b};
        }
        
        explicit N_bytes(string_view s) : N_bytes{read(s)} {}
        
        // A bit inefficient. 
        explicit N_bytes(const N& n) : N_bytes(data::encoding::hexidecimal::write(n)) {}
        
        explicit N_bytes(bytes_view b) : bytestring<r>{b} {}
        
        using bytestring<r>::operator bytes_view;
        
        math::sign sign() const {
            return operator==(0) ? math::zero : math::positive;
        }
        
        bool operator==(const N_bytes& n) const {
            return N{*this} == N{n}; // Inefficient
        }
        
        bool operator!=(const N_bytes& n) const {
            return !operator==(n);
        }
        
    private:
        
        N_bytes(size_t size, byte fill) : bytestring<r>(size, fill) {}
        
    public:
        
        using bytestring<r>::size;
        using bytestring<r>::begin;
        using bytestring<r>::end;
        using bytestring<r>::rbegin;
        using bytestring<r>::rend;
        using bytestring<r>::operator[];
        using bytestring<r>::valid;
        
        static N_bytes zero(size_t size) {
            return N_bytes(size, 0x00);
        }
        
        bool operator<(uint64 n) const {
            return operator<(N_bytes<r>{n});
        }
        
        bool operator<(const N_bytes& n) const;
        
        bool operator<(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<(N_bytes{z.Value});
        }
        
        bool operator<(const N& n) const {
            return N{*this} < n;
        }
        
        bool operator<(const Z& n) const {
            return N{*this} < n;
        }
        
        bool operator>(uint64 n) const {
            return !operator<=(n);
        }
        
        bool operator>(const N_bytes& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z_bytes<r>& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const N& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z& n) const {
            return !operator<=(n);
        }
        
        bool operator<=(uint64 n) const {
            return operator<=(N_bytes{n});
        }
        
        bool operator<=(const N_bytes& n) const;
        
        bool operator<=(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<=(N_bytes{z});
        }
        
        bool operator<=(const N& n) const {
            return N{*this} <= n;
        }
        
        bool operator<=(const Z& n) const {
            return N{*this} <= n;
        }
        
        bool operator>=(uint64 n) const {
            return operator>=(N_bytes{n});
        }
        
        bool operator>=(const N_bytes& n) const;
        
        bool operator>=(const Z_bytes<r>& z) const;
        
        bool operator>=(const N& n) const {
            return N{*this} >= n;
        }
        
        bool operator>=(const Z& n) const {
            return N{*this} >= n;
        }
        
        N_bytes operator~() const;
        
        N_bytes& operator++() {
            operator+=(1);
            return *this;
        }
        
        N_bytes& operator--() {
            operator+=(1);
            return *this;
        }
        
        N_bytes operator++(int) const {
            N_bytes z = *this;
            ++(*this);
            return z;
        }
        
        N_bytes operator--(int) const {
            N_bytes z = *this;
            ++(*this);
            return z;
        }
        
        N_bytes operator+(const N_bytes& n) const {
            // TODO very inefficient. 
            return N_bytes{N{*this} + N{n}};
        }
        
        N_bytes& operator+=(const N_bytes& n) {
            return operator=(operator+(n));
        }
        
        N_bytes operator-(const N_bytes& n) const {
            throw method::unimplemented{"N_bytes::-"};
        }
        
        N_bytes& operator-=(const N_bytes& n) {
            return operator=(operator-(n));
        }
        
        N_bytes operator*(const N_bytes&) const;
        
        N_bytes& operator*=(const N_bytes& n) {
            return operator=(operator*(n));
        }
        
        N_bytes operator^(uint32 n) const;
        
        N_bytes& operator^=(uint32 n) {
            return operator=(operator^(n));
        }
        
        math::division<N_bytes> divide(const N_bytes& n) const {
            return natural::divide<N_bytes>(*this, n);
        }
        
        bool operator|(const N_bytes& n) const {
            return divide(n).Remainder == 0;
        }
        
        N_bytes operator/(const N_bytes& n) const {
            return divide(n).Quotient;
        }
        
        N_bytes operator%(const N_bytes& n) const {
            return divide(n).Remainder;
        }
        
        N_bytes& operator/=(const N_bytes& n) {
            return operator=(operator/(n));
        }
        
        N_bytes& operator%=(const N_bytes& n) {
            return operator=(operator%(n));
        }
        
        N_bytes operator/(const N&) const;
        
        N_bytes operator%(const N&) const;
        
        N_bytes& operator/=(const N& n) {
            return operator=(operator/(n));
        }
        
        N_bytes& operator%=(const N& n) {
            return operator=(operator%(n));
        }
        
        N_bytes operator<<(int64 x) const {
            throw method::unimplemented{"N_bytes::<<"};
        }
        
        N_bytes operator>>(int64 x) const {
            throw method::unimplemented{"N_bytes::>>"};
        }
        
        N_bytes& operator<<=(int64 x) {
            return operator=(operator<<(x));
        }
        
        N_bytes& operator>>=(int64 x) {
            return operator=(operator>>(x));
        }
        
        bytes write(endian::order) const; 
        
        N_bytes trim() const;
        
        template <size_t size, endian::order o> 
        explicit N_bytes(const bounded<size, o, false>& b) : N_bytes{bytes_view(b), o} {}

    private:
        N_bytes(bytes_view b, endian::order o) : bytestring<r>{b.size()} {
            std::copy(b.begin(), b.end(), begin());
            if (o != r) std::reverse(begin(), end());
        }
        
        N_bytes(const Z_bytes<r>& z) {
            throw method::unimplemented{"N_bytes{Z_bytes}"};
        }
        
        friend struct abs<N_bytes, Z_bytes<r>>;
    };
    
    // Inefficient
    template <endian::order r>
    Z_bytes<r>::Z_bytes(const N_bytes<r>& n) : Z_bytes{N{n}} {}
    
    namespace low {
        template <typename B, typename E, typename O>
        void trim(uint32 size, B b, E e, O o) {
            while(*b == 0) {
                size--;
                b++;
            }
            std::copy(b, e, o);
        }
    }
    
    template <endian::order r> 
    N_bytes<r> N_bytes<r>::trim() const {
        uint32 s = size();
        if (s == 0) return N_bytes{bytes{}};
        N_bytes re{};
        if (r == endian::big) {
            auto b = begin();
            while (*b == 0) {
                s--;
                b++;
            }
            re.Value = bytes(s);
            std::copy(b, end(), re.begin());
        } else {
            auto b = rbegin();
            while (*b == 0) {
                s--;
                b++;
            }
            re.Value = bytes(size);
            std::copy(b, rend(), re.rend());
        }
        return re;
    }
}

namespace data::math::number {
    template <endian::order r> 
    struct abs<N_bytes<r>, N_bytes<r>> {
        N_bytes<r> operator()(const N_bytes<r>& i) {
            return i;
        }
    };
}

namespace data::math {
    // Declare that the plus and times operation on N are commutative. 
    template <endian::order r> struct commutative<data::plus<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    template <endian::order r> struct associative<data::plus<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    template <endian::order r> struct commutative<data::times<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    template <endian::order r> struct associative<data::times<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
}

namespace data::encoding::hexidecimal { 
    
    template <endian::order r>
    std::ostream& write(std::ostream& o, const math::number::N_bytes<r>& n) {
         return encoding::hexidecimal::write(o, bytes_view(n), r);
    }
    
    template <endian::order r>
    std::string write(const math::number::N_bytes<r>& n){
        std::stringstream s;
        encoding::hexidecimal::write(s, bytes_view(n), r);
        return s.str();
    }
    
}

namespace data::encoding::integer {
    /*
    template <endian::order r>
    std::ostream& write(std::ostream& o, const math::number::N_bytes<r>& n) {
        static const cross<math::number::N_bytes<r>> digits{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        struct to_digit{
            char operator()(const math::number::N_bytes<r>& n) {
                if (n == digits[0]) return '0';
                if (n == digits[1]) return '1';
                if (n == digits[2]) return '2';
                if (n == digits[3]) return '3';
                if (n == digits[4]) return '4';
                if (n == digits[5]) return '5';
                if (n == digits[6]) return '6';
                if (n == digits[7]) return '7';
                if (n == digits[8]) return '8';
                if (n == digits[9]) return '9';
                return '\0';
            }
        };
        
        functional::stack::linked<char> dig{};
        math::number::N_bytes<r> x = n;
        while(x > 0) {
            math::division<math::number::N_bytes<r>> d = n.divide(10);
            dig = dig << to_digit{}(d.Remainder);
            x = d.Quotient;
        }
        while(dig.size() > 0) {
            o << dig.first();
            dig = dig.rest();
        }
        return o;
    }
    */
    template <endian::order r>
    std::string write(const math::number::N_bytes<r>& n){
        std::stringstream ss;
        write(ss, data::math::number::gmp::N{n}); // Inefficient.
        return ss.str();
    }
    
}

namespace data::math::number {
    template <endian::order r>
    std::ostream& operator<<(std::ostream& o, const N_bytes<r>& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        // TODO for dec, we convert N_bytes to N. This is inefficient but it works for now. 
        if (o.flags() & std::ios::dec) return encoding::integer::write(o, gmp::N{n});
        return o;
    }
}

#endif

