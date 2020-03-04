// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_BYTESTRING
#define DATA_BYTESTRING

#include <data/math/arithmetic.hpp>

namespace data {
    
    template <endian::order r, size_t ... sizes> struct bytestring : public oriented<byte, r, sizes...> {
        using oriented<byte, r, sizes...>::oriented;
        
        bytestring(string_view hexidecimal);
        bytestring(bytes_view v) : oriented<byte, r, sizes...>(v, 0, v.size()) {}
        
        bytestring operator~() const;
        
        bytestring operator<<(int32) const;
        bytestring operator>>(int32) const;
        
        bytestring operator-(const int64&) const;
        bytestring operator+(const int64&) const;
        bytestring operator*(const int64&) const;
        
        bytestring operator-(const uint64&) const;
        bytestring operator+(const uint64&) const;
        bytestring operator*(const uint64&) const;
    };
    
    namespace encoding::hexidecimal {
    
        template<endian::order r>
        bytestring<r> read(string_view);
        
        template<endian::order r, uint32 size>
        bytestring<r, size> read(string_view);
    
    }
    
    template <endian::order r, size_t ... sizes>
    inline bytestring<r, sizes...>::bytestring(string_view hexidecimal) : 
        bytestring{encoding::hexidecimal::read<r, sizes...>(hexidecimal)} {}
    
}

template <data::endian::order r>
data::bytestring<r> operator|(
    const data::bytestring<r>& a, 
    const data::bytestring<r>& b);
        
template <data::endian::order r>
data::bytestring<r> operator^(
    const data::bytestring<r>& a, 
    const data::bytestring<r>& b);

template <data::endian::order r>
data::bytestring<r> operator-(
    const data::bytestring<r>& a, 
    const data::bytestring<r>& b);

template <data::endian::order r>
data::bytestring<r> operator+(
    const data::bytestring<r>& a, 
    const data::bytestring<r>& b);

template <data::endian::order r>
data::bytestring<r> operator*(
    const data::bytestring<r>& a, 
    const data::bytestring<r>& b);

template <data::endian::order r, size_t size>
data::bytestring<r, size> operator|(
    const data::bytestring<r, size>& a, 
    const data::bytestring<r, size>& b);
        
template <data::endian::order r, size_t size>
data::bytestring<r, size> operator^(
    const data::bytestring<r, size>& a, 
    const data::bytestring<r, size>& b);

template <data::endian::order r, size_t size>
data::bytestring<r, size> operator-(
    const data::bytestring<r, size>& a, 
    const data::bytestring<r, size>& b);

template <data::endian::order r, size_t size>
data::bytestring<r, size> operator+(
    const data::bytestring<r, size>& a, 
    const data::bytestring<r, size>& b);

template <data::endian::order r, size_t size>
data::bytestring<r, size> operator*(
    const data::bytestring<r, size>& a, 
    const data::bytestring<r, size>& b);

#endif


