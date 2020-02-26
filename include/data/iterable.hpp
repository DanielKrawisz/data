// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ITERABLE
#define DATA_ITERABLE

#include <data/interface.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian.hpp>
#include <data/encoding/words.hpp>

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
        
        cross(view<X> b) : std::vector<X>(b.size()) {
            std::copy(b.begin(), b.end(), std::vector<X>::begin());
        };
        
        operator view<X>() const {
            return view<X>{std::vector<X>::data(), std::vector<X>::size()};
        }
        
        explicit operator slice<X>() {
            return slice<X>(static_cast<std::vector<X>&>(*this));
        }
        
        view<X> range(int64) const;
        view<X> range(int64, int64) const;
        
        slice<X> range(int64);
        slice<X> range(int64, int64);
    };
    
    using bytes = cross<byte>;
    using bytes_view = view<byte>;

    // A type for treating sequences of bytes as numbers.
    template <typename X, endian::order r>
    struct ordered : public cross<X> {
        
        constexpr static endian::order endian = r;
        constexpr static endian::order opposite_endian = endian::opposite(r);
        
        ordered() : cross<X>{} {}
        ordered(size_t size) : cross<X>(size) {}
        ordered(size_t size, X fill) : cross<X>(size, fill) {}
        ordered(view<X> v) : cross<X>{v} {}
        
        explicit operator ordered<X, opposite_endian>() {
            return ordered<X, opposite_endian>{*this};
        }
        
    private:
        ordered(ordered<X, opposite_endian> reversed) : bytes{} {
            std::reverse_copy(reversed.begin(), reversed.end(), cross<X>::begin());
        }
    
    };

    // A type for treating sequences of bytes as numbers.
    template <
        size_t n,         // size (number of bytes)
        typename bit32,   // either int32 or uint32
        endian::order o>    // endianness.
    struct array : public ordered<byte, o> {
        using ordered<byte, o>::ordered;
        using ordered<byte, o>::endian;
        using ordered<byte, o>::opposite_endian;
        
        array operator~() const;
        array operator|(const array&) const;
        array operator^(const array&) const;
        
        bool operator>(const array& d) const;
        bool operator>=(const array& d) const;
        
        array operator-(const array&) const;
        array operator+(const array&) const;
        array operator*(const array&) const;
        
        array operator-(const bit32&) const;
        array operator+(const bit32&) const;
        array operator*(const bit32&) const;
        
        array operator<<(int32) const;
        array operator>>(int32) const;
        
    protected:
        using word = boost::endian::endian_arithmetic<o, bit32, 32>;
        using words_type = arithmetic::fixed_words<n, n / 4, n % 4, bit32, o>;
        using methods = arithmetic::unoriented<words_type, word>;
        
        words_type words() {
            return words_type{slice<byte, n>{bytes::data()}};
        }
        
        const words_type words() const {
            return words_type::make(slice<byte, n> {const_cast<byte*>(bytes::data())});
        }
    };
    
    template <size_t size, typename bit32, endian::order o>
    inline bool array<size, bit32, o>::operator>(const array& d) const {
        return d <= *this;
    }
    
    template <size_t size, typename bit32, endian::order o>
    inline bool array<size, bit32, o>::operator>=(const array& d) const {
        return d < *this;
    }
    
    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator~() const {
        array n{};
        methods::bit_not(words_type::Last, words(), n.words());
        return n;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator-(const array& n) const {
        array result;
        methods::minus(words_type::Last, words(), n.words(), result.words());
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator+(const array& n) const {
        array result;
        methods::plus(words_type::Last, words(), n.words(), result.words());
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator*(const array& n) const {
        array result;
        methods::times(words_type::Last, words(), n.words(), result.words());
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator-(const bit32& n) const {
        array result;
        methods::minus(*this, n, result);
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator+(const bit32& n) const {
        array<size, bit32, o> result;
        methods::plus(words_type::Last, words(), n, result.words());
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator*(const bit32& n) const {
        array result;
        methods::times(words(), n, result.words());
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator<<(int32 bits) const {
        array result;
        words_type w = result.words();
        if ((bits < 0 && o == endian::little) || (bits >= 0 && o == endian::big))
            methods::up(size, words(), bits, w);
        else methods::down(size, words(), bits, w);
        return result;
    }

    template <size_t size, typename bit32, endian::order o>
    inline array<size, bit32, o> array<size, bit32, o>::operator>>(int32 bits) const {
        array result;
        words_type w = result.words();
        if ((bits < 0 && o == endian::little) || (bits >= 0 && o == endian::big))
            methods::down(size, words(), bits, w);
        else methods::up(size, words(), bits, w);
        return result;
    }
}

template <typename X>
bool operator==(const data::ordered<X, data::endian::big>& x, const data::ordered<X, data::endian::little>& y) {
    return x == data::ordered<X, data::endian::big>(y);
}

template <typename X>
bool operator==(const data::ordered<X, data::endian::little>& x, const data::ordered<X, data::endian::big>& y) {
    return x == data::ordered<X, data::endian::little>(y);
}

#endif

