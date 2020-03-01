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
        cross();
        cross(size_t size);
        cross(size_t size, X fill);
        
        cross(std::initializer_list<X> x);
        
        cross(view<X> b);
        
        explicit operator slice<X>();
        
        view<X> range(int) const;
        view<X> range(int, int) const;
        
        slice<X> range(int);
        slice<X> range(int, int);
        
        slice<X> range(data::range r);
    };
    
    struct bytes : cross<byte> {
        using cross<byte>::cross;
        operator bytes_view() const;
    };
    
    // A section is both a container of data and a view of data. 
    // It allows us to construct data and then show less 
    // without having to resize. 
    template <typename X, size_t ...> struct section;
    
    template <typename X> struct section<X> : slice<X> {
        ptr<cross<X>> Data;
        
        bool valid() const {
            return slice<X>::valid() && Data != nullptr;
        }
        
        section() : slice<X>(), Data() {} 
        
        section(ptr<cross<X>> d, int begin, int end) : 
            section(d, d == nullptr ? 
                slice<X>() : 
                d->range(range(begin, end))) {}
        
        section(const cross<X>& d, int begin, int end) : section(d, range(begin, end) % d.size()) {}
        
        section(const cross<X>& d, range r) : 
            section(r.size() < 0 || r.Begin < 0 || r.End < 0 || r.Begin > d.size() || r.End > d.size() ? 
                section() :
                section(std::make_shared<cross<X>>(d), r)) {}
        
        section(size_t s, X fill) : section(std::make_shared<cross<X>>(s, fill)) {}
        
        section(view<X> v) : section(std::make_shared<cross<X>>(v)) {}
        
    private:
        section(ptr<cross<X>> d, slice<X> s) : slice<X>(s), Data(d) {}
        section(ptr<cross<X>> d, range r) : slice<X>(d->range(r)), Data(d) {}
        section(ptr<cross<X>> d) : slice<X>(slice<X>(*d)), Data(d) {}
    };
    
    template <typename X, size_t size> struct section<X, size> : slice<X, size> {
        ptr<cross<X>> Data;
        
        bool valid() const {
            return slice<X, size>::valid() && Data != nullptr;
        }
        
        section() : slice<X>(), Data() {} 
        
        section(ptr<cross<X>> d, size_t begin) : 
            section(d, d == nullptr || d.size() - begin < size ? 
                slice<X, size>() : 
                slice<X, size>(d->data())) {}
        
        section(view<X> d, size_t begin) : 
            section(d.size() - begin < size ? 
                section() :
                section(std::make_shared<cross<X>>(d), begin)) {}
        
        section(view<X> d, size_t begin, size_t end) : 
            section(end - begin != size ? 
                section() :
                section(std::make_shared<cross<X>>(d), begin)) {}
        
        section(const slice<X, size>);
        
        section(X fill) : section(std::make_shared<cross<X>>(size, fill)) {}
        
    private:
        section(ptr<cross<X>> d) : slice<X>(slice<X>(*d)), Data(d) {}
        
    };
    
    template <typename X, endian::order r, size_t ... sizes> struct oriented;
    
    template <typename X, size_t ... sizes> struct oriented<X, endian::big, sizes...> : section<X, sizes...> {
        using section<X, sizes...>::section;
        
        constexpr static endian::order endian = endian::big;
        constexpr static endian::order opposite = endian::little;
        
        explicit operator oriented<X, endian::little, sizes...>() const; // TODO
    };
    
    template <typename X, size_t ... sizes> struct oriented<X, endian::little, sizes...> : section<X, sizes...> {
        using section<X, sizes...>::section;
        
        constexpr static endian::order endian = endian::little;
        constexpr static endian::order opposite = endian::big;
        
        explicit operator oriented<X, endian::big, sizes...>() const; // TODO
        
    };
    
    template <typename X> 
    oriented<X, endian::little> trim(oriented<X, endian::little>, X fill);
    
    template <typename X> 
    oriented<X, endian::big> trim(oriented<X, endian::big>, X fill);
    
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
}

template <typename X>
inline bool operator==(
    const data::oriented<X, data::endian::big>& a, 
    const data::oriented<X, data::endian::little>& b) {
    return static_cast<data::slice<X>>(a) == static_cast<data::slice<X>>(data::oriented<X, data::endian::big>(b));
}

template <typename X>
inline bool operator==(
    const data::oriented<X, data::endian::little>& a, 
    const data::oriented<X, data::endian::big>& b) {
    return static_cast<data::slice<X>>(a) == static_cast<data::slice<X>>(data::oriented<X, data::endian::little>(b));
}

template <typename X, size_t size>
inline bool operator==(
    const data::oriented<X, data::endian::big, size>& a, 
    const data::oriented<X, data::endian::little, size>& b) {
    return static_cast<data::slice<X>>(a) == static_cast<data::slice<X>>(data::oriented<X, data::endian::big, size>(b));
}

template <typename X, size_t size>
inline bool operator==(
    const data::oriented<X, data::endian::little, size>& a, 
    const data::oriented<X, data::endian::big, size>& b) {
    return static_cast<data::slice<X>>(a) == static_cast<data::slice<X>>(data::oriented<X, data::endian::little, size>(b));
}

namespace data {
    template <typename X>
    inline cross<X>::cross() : std::vector<X>{} {}
    
    template <typename X>
    inline cross<X>::cross(size_t size) : std::vector<X>(size) {}
    
    template <typename X>
    inline cross<X>::cross(size_t size, X fill) : std::vector<X>(size) {
        for (auto it = std::vector<X>::begin(); it < std::vector<X>::end(); it++) *it = fill;
    }
        
    template <typename X>
    inline cross<X>::cross(std::initializer_list<X> x) : std::vector<X>{x} {}
        
    template <typename X>
    inline cross<X>::cross(view<X> b) : std::vector<X>(b.size()) {
        std::copy(b.begin(), b.end(), std::vector<X>::begin());
    }
        
    template <typename X>
    inline cross<X>::operator slice<X>() {
        return slice<X>(static_cast<std::vector<X>&>(*this));
    }
    
    template <typename X>
    inline view<X> cross<X>::range(int e) const {
        return operator slice<X>().range(e);
    }
    
    template <typename X>
    inline view<X> cross<X>::range(int b, int e) const {
        return operator slice<X>().range(b, e);
    }
        
    template <typename X>
    inline slice<X> cross<X>::range(int e) {
        return operator slice<X>().range(e);
    }
    
    template <typename X>
    inline slice<X> cross<X>::range(int b, int e) {
        return operator slice<X>().range(e);
    }
        
    template <typename X>
    inline slice<X> cross<X>::range(data::range r) {
        return operator slice<X>().range(r);
    }
}

#endif

