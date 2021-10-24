// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>

#include <data/encoding/endian/endian.hpp>
#include <data/cross.hpp>
#include <data/slice.hpp>

namespace data {
    static const std::string EndOfStreamError{"End of stream"};
    
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
        
        ostream &operator<<(X x);
    };
    
    template <typename X, typename it>
    struct istream {
        it Begin;
        it End;
        istream(const it b, const it e) : Begin{b}, End{e} {}
        
        bool empty() const {
            return Begin == End;
        }

        istream &operator>>(X& x) {
            if (empty()) throw end_of_stream{};
            x = *Begin;
            ++Begin;
            return *this;
        }
        
        istream &skip(uint64 n) {
            if (Begin + n > End) throw end_of_stream{};
            Begin += n;
            return *this;
        }
    };
    
    template <typename it>
    struct writer {
        ostream<byte, it> Writer;
        
        writer(ostream<byte, it> w) : Writer{w} {};
        writer(it b, it e) : Writer{b, e} {}
        
        writer &operator<<(bytes_view);
        writer &operator<<(const byte b) {
            Writer << b;
            return *this;
        }

        writer &operator<<(const char& c) {
            return operator<<(static_cast<const byte&>(c));
        }
    
        template <boost::endian::order Order, bool is_signed, std::size_t bytes>
        writer &operator<<(const endian::arithmetic<Order, is_signed, bytes> x) {
            return operator<<(bytes_view(x));
        }
        
    };
    
    template <typename it>
    struct reader {
        istream<byte, it> Reader;
        
        reader(istream<byte, it> r) : Reader{r} {}
        reader(const it b, const it e) : Reader{b, e} {}
        
        template <boost::endian::order Order, bool is_signed, std::size_t bytes>
        reader &operator>>(endian::arithmetic<Order, is_signed, bytes>&);
        
        reader &operator>>(bytes&);
        reader &operator>>(byte& b) {
            Reader >> b;
            return *this;
        }
        
        reader &operator>>(char& x) {
            byte b;
            operator>>(b);
            x = b;
            return *this;
        }
        
        bool empty() const {
            return Reader.empty();
        }
        
        reader &skip(uint32 n) {
            Reader.skip(n);
            return *this;
        }
    };
    
    namespace stream {
    
        template <typename it>
        inline writer<it> &write_all(writer<it> &w) {
            return w;
        }
        
        template <typename it, typename X, typename ... P>
        inline writer<it> &write_all(writer<it> &w, X x, P... p) {
            return write_all(w << x, p...);
        }
        
        template <typename ... P>
        string write_string(uint32 size, P... p) {
            string Data;
            Data.resize(size);
            write_all(writer{Data.begin(), Data.end()}, p...);
            return Data;
        };
        
        template <typename ... P>
        bytes write_bytes(uint32 size, P... p) {
            bytes Data(size);
            writer<bytes::iterator> w{Data.begin(), Data.end()};
            write_all(w, p...);
            return Data;
        };
    
    }
    
    template <typename X, typename it>
    ostream<X, it> &ostream<X, it>::operator<<(X x) {
        if (Begin == End) throw end_of_stream{};
        *Begin = x;
        ++Begin;
        return *this;
    }
    
    template <typename it>
    writer<it> &writer<it>::operator<<(bytes_view x) {
        for (auto i = x.begin(); i != x.end(); ++i) Writer << *i;
        return *this;
    }

    template <typename it>
    reader<it> &reader<it>::operator>>(bytes &x) {
        for (uint32 i = 0; i < x.size(); i++) Reader >> x[i];
        return *this;
    }
    
    namespace low {
    
        template <typename it>
        void forward(istream<byte, it> &is, uint32 amount, byte* to) {
            for (uint32 i = 0; i < amount; i++) {
                is >> *to;
                to++;
            }
        }
    
    }
    
    template <typename it>
    template <boost::endian::order Order, bool is_signed, std::size_t bytes>
    reader<it> &reader<it>::operator>>(endian::arithmetic<Order, is_signed, bytes>& x) {
        low::forward(Reader, bytes, (byte*)(x.data()));
        return *this;
    }
    
}

#endif

