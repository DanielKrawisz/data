// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>
#include <data/slice.hpp>

namespace data {
    static const std::string EndOfStreamError{"End of stream"};
    
    struct end_of_stream : std::exception {
        const char* what() const noexcept final override {
            return EndOfStreamError.c_str();
        }
    };
    
    template <typename word>
    struct writer {
        virtual void write(const word*, size_t size) = 0;
        virtual ~writer() {}
    };
    
    template <typename word>
    writer<word> &operator<<(writer<word> &w, const word &x);
    
    template <typename word>
    writer<word> &operator<<(writer<word> &w, std::basic_string_view<word> x);
    
    template <typename word>
    writer<word> &operator<<(writer<word> &w, const string &x);
    
    template <typename word>
    struct reader {
        virtual void read(word*, size_t size) = 0;
        virtual void skip(size_t) = 0;
        virtual ~reader() {}
    };
    
    template <typename word>
    reader<word> inline &operator>>(reader<word> &r, slice<word> x) {
        r.read(x.data(), x.size());
        return r;
    }
    
    template <typename word>
    reader<word> inline &operator>>(reader<word> &r, word &x) {
        r.read(&x, 1);
        return r;
    }
    
    template <typename it, typename word>
    struct iterator_writer : writer<word> {
        it Begin;
        it End;
        
        iterator_writer(it b, it e) : Begin{b}, End{e} {}
        
        void write(const word *b, size_t size) override {
            for (int i = 0; i < size; i++) {
                if (Begin == End) throw end_of_stream{};
                *Begin = b[i];
                ++Begin;
            }
        }
        
    };
    
    template <typename it, typename word>
    struct iterator_reader : reader<word> {
        it Begin;
        it End;
        iterator_reader(const it b, const it e) : Begin{b}, End{e} {}
        
        bool empty() const {
            return Begin == End;
        }

        void read(word *b, size_t size) override {
            for (int i = 0; i < size; i++) {
                if (empty()) throw end_of_stream{};
                b[i] = *Begin;
                ++Begin;
            }
        }
        
        void skip(size_t size) override {
            if (Begin + size > End) throw end_of_stream{};
            Begin += size;
        }
    };
    
    template <typename word, typename read>
    struct parser : public writer<word> {
        virtual ~parser() {}
        virtual void parsed(const read &) = 0;
        
        virtual void parse_error(const string &invalid) = 0;
    };
    
    using bytes_writer = data::iterator_writer<std::vector<byte>::iterator, byte>;
    using bytes_reader = data::iterator_reader<const byte*, byte>;

    template <typename word>
    writer<word> inline &operator<<(writer<word> &w, const word &x) {
        w.write(&x, 1);
        return w;
    }

    template <typename word>
    writer<word> inline &operator<<(writer<word> &w, std::basic_string_view<word> x) {
        w.write(x.data(), x.size());
        return w;
    }

    template <typename word>
    writer<word> inline &operator<<(writer<word> &w, const string &x) {
        w.write((const byte*)(x.data()), x.size());
        return w;
    }
    
    template writer<byte> &operator<<(writer<byte> &, const byte &);
    template writer<byte> &operator<<(writer<byte> &, bytes_view);
    template writer<byte> &operator<<(writer<byte> &, const string &);
    template reader<byte> &operator>>(reader<byte> &, slice<byte>);
    
}

#endif

