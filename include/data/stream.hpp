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
    protected:
        slice<X> Slice;
        typename slice<X>::iterator It;
    public:
        void operator<<(X x) {
            if (It == Slice.end()) throw end_of_stream{};
            *It = x;
            It++;
        }

        explicit ostream(slice<X> s) : Slice{s}, It{Slice.begin()} {}

        explicit ostream(std::vector<X> &v) : ostream{slice<X>{v}} {}
    };
    
    template <typename X>
    struct istream {
    protected:
        const slice<X> Slice;
        typename slice<X>::const_iterator It;
    public:

        void operator>>(X& x) {
            if (It == Slice.end()) throw end_of_stream{};
            x = *It;
            It++;
        }
        
        explicit istream(slice<X> s) : Slice{s}, It{Slice.begin()} {}
        explicit istream(std::vector<X>& v) : istream{slice<X>{v}} {}

    };
    
    class writer : public ostream<byte> {
        void bytesIntoIterator(const char*,int);
    public:
        using ostream<byte>::operator<<;
        void operator<<(const ordered<uint16, big>);
        void operator<<(const ordered<uint32, big>);
        void operator<<(const ordered<uint64, big>);
        void operator<<(const ordered<int16, big>);
        void operator<<(const ordered<int32, big>);
        void operator<<(const ordered<int64, big>);
        void operator<<(const ordered<uint16, little>);
        void operator<<(const ordered<uint32, little>);
        void operator<<(const ordered<uint64, little>);
        void operator<<(const ordered<int16, little>);
        void operator<<(const ordered<int32, little>);
        void operator<<(const ordered<int64, little>);
        void operator<<(bytes&);
        
        writer(slice<byte> s) : ostream<byte>{s} {}
        writer(std::vector<byte>& v) : ostream<byte>{v} {}
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
        
        reader(slice<byte> s) : istream<byte>{s} {}
        reader(std::vector<byte>& v) : istream<byte>{v} {}
    };

}

#endif
