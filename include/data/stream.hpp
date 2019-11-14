// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>
#include "list.hpp"
#include "slice.hpp"
#include <data/encoding/endian.hpp>

namespace data {
    
    constexpr endian::order big = endian::order::big;
    constexpr endian::order little = endian::order::little;
    
    template <typename X, endian::order o>
    using ordered = endian::ordered<X, o>;
    
    static const string EndOfStreamError = string{"End of stream"};
    
    struct end_of_stream : std::exception {
        const char* what() const noexcept final override {
            return EndOfStreamError.c_str();
        }
    };
    
    template <typename X>
    struct ostream {
        slice<X> Slice;
        using it = typename slice<X>::iterator;
        it It;
        ostream(slice<X> s, it i) : Slice{s}, It{i} {}
        
        ostream operator<<(X x) const;

        explicit ostream(slice<X> s) : Slice{s}, It{Slice.begin()} {}
    };
    
    template <typename X>
    struct istream {
        bytes_view Slice;
        using it = typename bytes_view::const_iterator;
        it It;
        istream(bytes_view s, it i) : Slice{s}, It{i} {}

        void operator>>(X& x) {
            if (It == Slice.end()) throw end_of_stream{};
            x = *It;
            It++;
        }
        
        explicit istream(bytes_view s) : Slice{s}, It{Slice.begin()} {}
    };
    
    class writer {
        ostream<byte> Writer;
        using it = ostream<byte>::it;
        writer(ostream<byte> w) : Writer{w} {};
    public:
        writer operator<<(const ordered<uint16, big>) const;
        writer operator<<(const ordered<uint32, big>) const;
        writer operator<<(const ordered<uint64, big>) const;
        writer operator<<(const ordered<int16, big>) const;
        writer operator<<(const ordered<int32, big>) const;
        writer operator<<(const ordered<int64, big>) const;
        writer operator<<(const ordered<uint16, little>) const;
        writer operator<<(const ordered<uint32, little>) const;
        writer operator<<(const ordered<uint64, little>) const;
        writer operator<<(const ordered<int16, little>) const;
        writer operator<<(const ordered<int32, little>) const;
        writer operator<<(const ordered<int64, little>) const;
        writer operator<<(const bytes_view) const;
        writer operator<<(const byte b) const {
            return writer{Writer << b};
        }
        
        writer(slice<byte> s) : Writer{s} {}
    };
    
    struct reader : public istream<byte> {
        using istream<byte>::operator>>;
        void operator>>(ordered<uint16, big>&);
        void operator>>(ordered<uint32, big>&);
        void operator>>(ordered<uint64, big>&);
        void operator>>(ordered<int16, big>&);
        void operator>>(ordered<int32, big>&);
        void operator>>(ordered<int64, big>&);
        void operator>>(ordered<uint16, little>&);
        void operator>>(ordered<uint32, little>&);
        void operator>>(ordered<uint64, little>&);
        void operator>>(ordered<int16, little>&);
        void operator>>(ordered<int32, little>&);
        void operator>>(ordered<int64, little>&);
        void operator>>(std::vector<byte>&);
        
        reader(bytes_view s) : istream<byte>{s} {}
    };
    
    template <typename X>
    ostream<X> ostream<X>::operator<<(X x) const {
        it I = It;
        if (I == Slice.end()) throw end_of_stream{};
        *I = x;
        I++;
        return ostream{Slice, I};
    }

}

#endif
