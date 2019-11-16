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
    
    template <typename X, typename it>
    struct ostream {
        it Begin;
        it End;
        ostream(it b, it e) : Begin{b}, End{e} {}
        
        ostream operator<<(X x) const;
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
    
    template <typename it>
    struct writer {
        ostream<byte, it> Writer;
        
        writer(ostream<byte, it> w) : Writer{w} {};
        writer(it b, it e) : Writer{b, e} {}
        
        writer operator<<(bytes_view) const;
        writer operator<<(const byte b) const {
            return writer{Writer << b};
        }
        
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
    
    template <typename X, typename it>
    ostream<X, it> ostream<X, it>::operator<<(X x) const {
        it I = Begin;
        if (I == End) throw end_of_stream{};
        *I = x;
        I++;
        return ostream{I, End};
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(bytes_view x) const {
        ostream<byte, it> w = Writer;
        for(bytes_view::iterator i = x.begin(); i != x.end(); i++) w = w << *i;
        return writer{w};
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint16, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_uint16_buf_t{x.Value}.data(), sizeof(uint16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint16, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_uint16_buf_t{x.Value}.data(), sizeof(uint16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint32, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_uint32_buf_t{x.Value}.data(), sizeof(uint32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint32, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_uint32_buf_t{x.Value}.data(), sizeof(uint32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint64, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_uint64_buf_t{x.Value}.data(), sizeof(uint64)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint64, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_uint64_buf_t{x.Value}.data(), sizeof(uint64)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int16, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_int16_buf_t{x.Value}.data(), sizeof(int16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int16, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_int16_buf_t{x.Value}.data(), sizeof(int16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int32, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_int32_buf_t{x.Value}.data(), sizeof(int32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int32, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_int32_buf_t{x.Value}.data(), sizeof(int32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int64, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_int64_buf_t{x.Value}.data(), sizeof(int64)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int64, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_int64_buf_t{x.Value}.data(), sizeof(int64)});
    }

}

#endif
