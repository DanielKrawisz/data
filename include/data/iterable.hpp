// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ITERABLE
#define DATA_ITERABLE

#include <data/interface.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian.hpp>

namespace data {
    
    namespace interface {
    
        template <typename X>
        struct iterable {
            using iterator = decltype(std::declval<X>().begin());
            using const_iterator = decltype(std::declval<const X>().begin());
        private:
            using require_iterable = typename std::enable_if<meta::is_iterable<X, iterator>::value && meta::is_const_iterable<X, const_iterator>::value, bool>::type;
        };
    
        template <typename X>
        struct const_iterable {
            using const_iterator = decltype(std::declval<const X>().begin());
        private:
            using require_const_iterable = typename std::enable_if<meta::is_const_iterable<X, const_iterator>::value, bool>::type;
        };
        
        template <typename X, typename e>
        struct array : iterable<X>, indexed<X, e> {};
    }
    
    // The cartesian product. 
    // it is the same as a vector except that it can be
    // automatically cast to view. 
    template <typename X> struct cross : public std::vector<X> {
        cross() : std::vector<X>{} {}
        cross(size_t size) : std::vector<X>(size) {}
        cross(size_t size, X fill) : std::vector<X>(size) {
            for (auto it = std::vector<X>::begin(); it < std::vector<X>::end(); it++) *it = fill;
        }
        
        cross(std::initializer_list<X> x) : std::vector<X>{x} {}
        
        cross(view<X> b) : std::vector<X>(b.size()) {
            std::copy(b.begin(), b.end(), std::vector<X>::begin());
        };
        
        operator view<X>() const {
            return view<X>{std::vector<X>::data(), std::vector<X>::size()};
        }
        
        explicit operator slice<X>() {
            return slice<X>(static_cast<std::vector<X>&>(*this));
        }
        
        view<X> range(int) const;
        view<X> range(int, int) const;
        
        slice<X> range(int);
        slice<X> range(int, int);
    };
    
    using bytes = cross<byte>;
    
    // An array is like a slice but it includes the data 
    // that it refers to. 
    template <typename X, size_t ...> struct section;
    
    template <typename X> struct section<X> : slice<X> {
        cross<X> Data;
        section() : slice<X>(), Data() {} // invalid value
        section(const cross<X>& d) : slice<X>(), Data() {
            Data = d;
            slice<X>::operator=(slice<X>(Data));
        }
        
        section(const cross<X>& d, int begin, int end) : section(d, range(begin, end) % d.size()) {}
        section(size_t size, X fill) : section(cross<X>(size, fill), 0, size) {}
        section(view<X> v) : section(cross<X>(v)) {}
        
        bool valid() const {
            return slice<X>::valid() && slice<X>::size() <= Data.size();
        }
        
    private:
        section(const cross<X>& d, slice<X> s) : slice<X>(s), Data{d} {}
        section(const cross<X>& d, range r) : section{r.size() < 0 || r.Begin < 0 || r.End < 0 || r.Begin > d.size() || r.End > d.size(), section{}, section{d, slice<X>{d.data() + r.Begin, static_cast<size_t>(r.size())}}} {}
    };
    
    template <typename X, size_t size> struct section<X, size> : section<X> {
        section() : section<X>{size} {}
        section(X fill) : section<X>{size, fill} {}
        section(const cross<X>& d, int begin);
        bool valid() const {
            return section<X>::valid() && section<X>::size() == size;
        }
    };
    
    template <size_t ... sizes> 
    struct bytestring : section<byte, sizes...> {
        using section<byte, sizes...>::section;
        bytestring(string_view s) : section<byte, sizes...>(s.size(), 0, s.size()) {
            std::copy(s.data(), s.data() + s.size(), cross<byte>::data());
        }
        
        bytestring operator~() const;
    };
    
    template <typename X, uint32 n> struct tensor;
    
    template <typename X> struct tensor<X, 1> : slice<X> {
        template <uint32>
        uint32 dimension() const;
        const X operator[](uint32 x) const;
        X operator[](uint32 x);
    };
    
    template <typename X, uint32 n> struct tensor : slice<X> {
        template <uint32>
        uint32 dimension() const;
        const tensor<X, n - 1> operator[](uint32 x) const;
        tensor<X, n - 1> operator[](uint32 x);
    };
    
    // a bytestring with a concept of endian ordering. 
    template <endian::order r, size_t ... sizes>
    struct ordered : public section<byte, sizes...> {
        
        constexpr static endian::order endian = r;
        constexpr static endian::order opposite = endian::opposite(r);
        
        ordered() : section<byte, sizes...>{} {}
        ordered(const section<byte, sizes...>&);
        ordered(size_t size) : section<byte, sizes...>(size) {}
        ordered(size_t size, byte fill) : section<byte, sizes...>(size, fill) {}
        ordered(bytes_view v) : section<byte, sizes...>{v} {}
        
        ordered(const string_view hex);
        
        ordered operator~() const;
        
        ordered operator<<(int32) const;
        ordered operator>>(int32) const;
        
        ordered(ordered<opposite> reversed) : section<byte, sizes...>(reversed.size()) {
            std::reverse_copy(reversed.begin(), reversed.end(), section<byte, sizes...>::begin());
        }
    
    };

    // A type for treating sequences of bytes as numbers.
    template <typename bit32, endian::order o, size_t ... sizes>
    struct array : public ordered<o> {
        using ordered<o>::ordered;
        using ordered<o>::endian;
        using ordered<o>::opposite;
        
        array(const ordered<o>&);
        
        array operator-(const bit32&) const;
        array operator+(const bit32&) const;
        array operator*(const bit32&) const;
    };
}

template <typename X>
bool operator==(const data::ordered<data::endian::big>& x, const data::ordered<data::endian::little>& y) {
    return x == data::ordered<data::endian::big>(y);
}

template <typename X>
bool operator==(const data::ordered<data::endian::little>& x, const data::ordered<data::endian::big>& y) {
    return x == data::ordered<data::endian::little>(y);
}

template <data::endian::order r, size_t ... sizes>
data::ordered<r, sizes...> operator|(
    const data::ordered<r, sizes...>&, 
    const data::ordered<r, sizes...>&);
        
template <data::endian::order r, size_t ... sizes>
data::ordered<r, sizes...> operator^(
    const data::ordered<r, sizes...>&, 
    const data::ordered<r, sizes...>&);

template <typename bit32, data::endian::order o, size_t ... sizes>
data::array<bit32, o, sizes...> operator-(
    const data::array<bit32, o, sizes...>&, 
    const data::array<bit32, o, sizes...>&);

template <typename bit32, data::endian::order o, size_t ... sizes>
data::array<bit32, o, sizes...> operator+(
    const data::array<bit32, o, sizes...>&, 
    const data::array<bit32, o, sizes...>&);

template <typename bit32, data::endian::order o, size_t ... sizes>
data::array<bit32, o, sizes...> operator*(
    const data::array<bit32, o, sizes...>&, 
    const data::array<bit32, o, sizes...>&);

#endif

