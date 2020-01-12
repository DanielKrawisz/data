// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>
#include "list.hpp"
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
            throw method::unimplemented{"writer << string"};
        }
        
        writer operator<<(const uint8_big) const;
        writer operator<<(const uint16_big) const;
        writer operator<<(const uint24_big) const;
        writer operator<<(const uint32_big) const;
        writer operator<<(const uint40_big) const;
        writer operator<<(const uint48_big) const;
        writer operator<<(const uint56_big) const;
        writer operator<<(const uint64_big) const;

        writer operator<<(const int8_big) const;
        writer operator<<(const int16_big) const;
        writer operator<<(const int24_big) const;
        writer operator<<(const int32_big) const;
        writer operator<<(const int40_big) const;
        writer operator<<(const int48_big) const;
        writer operator<<(const int56_big) const;
        writer operator<<(const int64_big) const;

        writer operator<<(const uint8_little) const;
        writer operator<<(const uint16_little) const;
        writer operator<<(const uint24_little) const;
        writer operator<<(const uint32_little) const;
        writer operator<<(const uint40_little) const;
        writer operator<<(const uint48_little) const;
        writer operator<<(const uint56_little) const;
        writer operator<<(const uint64_little) const;

        writer operator<<(const int8_little) const;
        writer operator<<(const int16_little) const;
        writer operator<<(const int24_little) const;
        writer operator<<(const int32_little) const;
        writer operator<<(const int40_little) const;
        writer operator<<(const int48_little) const;
        writer operator<<(const int56_little) const;
        writer operator<<(const int64_little) const;
    };
    
    template <typename it>
    struct reader {
        istream<byte, it> Reader;
        
        reader(istream<byte, it> r) : Reader{r} {}
        reader(it b, it e) : Reader{b, e} {}
        reader(const reader& r) : Reader{r.Reader} {}
        
        reader operator>>(uint8_big&) const;
        reader operator>>(uint16_big&) const;
        reader operator>>(uint24_big&) const;
        reader operator>>(uint32_big&) const;
        reader operator>>(uint40_big&) const;
        reader operator>>(uint48_big&) const;
        reader operator>>(uint56_big&) const;
        reader operator>>(uint64_big&) const;

        reader operator>>(int8_big&) const;
        reader operator>>(int16_big&) const;
        reader operator>>(int24_big&) const;
        reader operator>>(int32_big&) const;
        reader operator>>(int40_big&) const;
        reader operator>>(int48_big&) const;
        reader operator>>(int56_big&) const;
        reader operator>>(int64_big&) const;

        reader operator>>(uint8_little&) const;
        reader operator>>(uint16_little&) const;
        reader operator>>(uint24_little&) const;
        reader operator>>(uint32_little&) const;
        reader operator>>(uint40_little&) const;
        reader operator>>(uint48_little&) const;
        reader operator>>(uint56_little&) const;
        reader operator>>(uint64_little&) const;

        reader operator>>(int8_little&) const;
        reader operator>>(int16_little&) const;
        reader operator>>(int24_little&) const;
        reader operator>>(int32_little&) const;
        reader operator>>(int40_little&) const;
        reader operator>>(int48_little&) const;
        reader operator>>(int56_little&) const;
        reader operator>>(int64_little&) const;

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
    writer<it> writer<it>::operator<<(const uint8_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint16_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint24_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint32_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint40_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint48_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint56_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint64_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int8_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int16_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int24_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int32_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int40_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int48_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int56_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int64_big x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint8_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint16_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint24_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint32_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint40_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint48_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint56_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const uint64_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int8_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int16_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int24_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int32_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int40_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int48_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int56_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
    }

    template <typename it>
    writer<it> writer<it>::operator<<(const int64_little x) const {
        return operator<<(bytes_view{(const byte*)x.data(), sizeof(x)});
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
    reader<it> reader<it>::operator>>(uint8_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint16_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint24_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint32_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint40_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint48_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint56_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint64_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int8_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int16_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int24_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int32_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int40_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int48_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int56_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int64_big& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint8_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint16_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint24_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint32_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint40_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint48_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint56_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(uint64_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int8_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int16_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int24_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int32_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int40_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int48_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int56_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }

    template <typename it>
    reader<it> reader<it>::operator>>(int64_little& x) const {
        return low::forward(Reader, sizeof(x), (byte*)(x.data()));
    }
}

#endif

