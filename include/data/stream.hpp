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
    
    template <typename it>
    struct reader {
        istream<byte, it> Reader;
        
        reader(istream<byte, it> r) : Reader{r} {}
        reader(it b, it e) : Reader{b, e} {}
        
        reader operator>>(ordered<uint16, big>&) const;
        reader operator>>(ordered<uint32, big>&) const;
        reader operator>>(ordered<uint64, big>&) const;
        reader operator>>(ordered<int16, big>&) const;
        reader operator>>(ordered<int32, big>&) const;
        reader operator>>(ordered<int64, big>&) const;
        reader operator>>(ordered<uint16, little>&) const;
        reader operator>>(ordered<uint32, little>&) const;
        reader operator>>(ordered<uint64, little>&) const;
        reader operator>>(ordered<int16, little>&) const;
        reader operator>>(ordered<int32, little>&) const;
        reader operator>>(ordered<int64, little>&) const;
        reader operator>>(bytes&) const;
        reader operator>>(byte& b) const {
            return reader{Reader >> b};
        }
        
        bool empty() const {
            return Reader.empty();
        }
    };
    
    namespace stream {
    
        template <typename X>
        inline writer<char *> write_all(writer<char*> w) {
            return w;
        }
        
        template <typename X, typename ... P>
        inline writer<char*> write_all(writer<char*> w, X x, P... p) {
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
            bytes Data;
            Data.resize(size);
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
    writer<it> writer<it>::operator<<(const ordered<uint16, big> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(uint16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint16, little> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(uint16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint32, big> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(uint32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint32, little> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(uint32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint64, big> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(uint64)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<uint64, little> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(uint64)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int16, big> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(int16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int16, little> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(int16)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int32, big> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(int32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int32, little> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(int32)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int64, big> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(int64)});
    }
    
    template <typename it>
    writer<it> writer<it>::operator<<(const ordered<int64, little> x) const {
        return operator<<(bytes_view{(const byte*)&x.Value, sizeof(int64)});
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
    reader<it> reader<it>::operator>>(ordered<uint16, little>& x) const {
        return low::forward(Reader, sizeof(uint16), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<uint16, big>& x) const {
        return low::forward(Reader, sizeof(uint16), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<uint32, little>& x) const {
        return low::forward(Reader, sizeof(uint32), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<uint32, big>& x) const {
        return low::forward(Reader, sizeof(uint32), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<uint64, big>& x) const {
        return low::forward(Reader, sizeof(uint64), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<uint64, little>& x) const {
        return low::forward(Reader, sizeof(uint64), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<int16, big>& x) const {
        return low::forward(Reader, sizeof(int16), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<int16, little>& x) const {
        return low::forward(Reader, sizeof(int16), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<int32, big>& x) const {
        return low::forward(Reader, sizeof(int32), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<int32, little>& x) const {
        return low::forward(Reader, sizeof(int32), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<int64, big>& x) const {
        return low::forward(Reader, sizeof(int64), (byte*)(&x.Value));
    }
    
    template <typename it>
    reader<it> reader<it>::operator>>(ordered<int64, little>& x) const {
        return low::forward(Reader, sizeof(int64), (byte*)(&x.Value));
    }
    
}

#endif
