// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>

#include <data/stack.hpp>
#include "slice.hpp"

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
        
        ostream operator<<(X x) const;
    };
    
    template <typename X, typename it>
    struct istream {
        it Begin;
        it End;
        istream(it b, it e) : Begin{b}, End{e} {}
        
        bool empty() const {
            return Begin == End;
        }

        istream operator>>(X& x) const {
            if (empty()) throw end_of_stream{};
            it b = Begin;
            x = *b;
            return istream{++b, End};
        }
    };
    
    template <typename it>
    struct writer {
        ostream<byte, it> Writer;
        
        writer(ostream<byte, it> w) : Writer{w} {};
        writer(it b, it e) : Writer{b, e} {}
        writer(const writer& w) : Writer{w.Writer} {}
        
        writer operator<<(bytes_view) const;
        writer operator<<(const byte b) const {
            return writer{Writer << b};
        }

        writer operator<<(const char& c) const {
            return operator<<(static_cast<const byte&>(c));
        }
        
        writer operator<<(string& b) const {
            return operator<<(bytes_view{(const byte*)b.data(), b.size()});
        }
    
        template <boost::endian::order Order, class T, std::size_t n_bits>
        writer operator<<(const endian::arithmetic<Order, T, n_bits> x) const {
            return operator<<(bytes_view{(const byte*)x.data(), n_bits / 8});
        }
        
    };
    
    template <typename it>
    struct reader {
        istream<byte, it> Reader;
        
        reader(istream<byte, it> r) : Reader{r} {}
        reader(it b, it e) : Reader{b, e} {}
        reader(const reader& r) : Reader{r.Reader} {}
        
        template <boost::endian::order Order, class T, std::size_t n_bits>
        reader operator>>(endian::arithmetic<Order, T, n_bits>&) const;

        reader operator>>(bytes&) const;
        reader operator>>(byte& b) const {
            return reader{Reader >> b};
        }

        reader operator>>(char& x) const {
            byte b;
            auto r = operator>>(b);
            x = b;
            return r;
        }
        
        reader operator>>(string& b) const {
            throw method::unimplemented{"reader >> string"};
        }
        
        template <size_t size>
        reader operator>>(std::array<byte, size>&) const {
            throw method::unimplemented{"reader >> array"};
        }
        
        bool empty() const {
            return Reader.empty();
        }
    };
    
    namespace stream {
    
        template <typename it>
        inline writer<it> write_all(writer<it> w) {
            return w;
        }
        
        template <typename it, typename X, typename ... P>
        inline writer<it> write_all(writer<it> w, X x, P... p) {
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
            bytes Data{size};
            write_all(writer{Data.begin(), Data.end()}, p...);
            return Data;
        };
    
    }
        
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
    reader<it> reader<it>::operator>>(bytes &x) const {
        istream<byte, it> is = Reader;
        for(int i=0;i<x.size();i++) is = is >> x[i];
        return reader{is};
    }
    
    namespace low {
    
        template <typename it>
        reader<it> forward(istream<byte, it> is, int amount, byte* to) {
            for(int i=0;i<amount;i++)
            {
                is = is >> *to;
                to++;
            }
            return reader{is};
        }
    
    }
    
    template <typename it>
    template <boost::endian::order Order, class T, std::size_t n_bits>
    reader<it> reader<it>::operator>>(endian::arithmetic<Order, T, n_bits>& x) const {
        return low::forward(Reader, n_bits / 8, (byte*)(x.data()));
    }
    
}

#endif

