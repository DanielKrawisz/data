// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_TRANSFORM
#define DATA_CRYPTO_TRANSFORM

#include <data/stream.hpp>
#include <data/bytes.hpp>
#include <data/tools.hpp>
#include <cryptopp/filters.h>

namespace data {

    struct buffer_writer final : reader<byte>, message_writer<bytes, byte>, CryptoPP::NotCopyable {
        bytes Result;
        CryptoPP::StringSinkTemplate<bytes> Sink;

        buffer_writer ();

        // Move semantics just to be super cool.
        buffer_writer (buffer_writer &&other) noexcept;
        buffer_writer &operator = (buffer_writer &&other) noexcept;

        void write (const byte *b, size_t size) final override;

        void read (byte *b, size_t size) final override;
        void skip (size_t size) final override;

        // there is a bit of a contradiction here because you shouldn't
        // use read/skip together with complete. Just make sure you're
        // using one or the other.
        bytes complete () final override;
    };

    // ensure that all << and >> operations defined with
    // reader and writer will work with this.
    template <std::integral word, typename T> requires requires (writer<word> &w, const T &x) {
        { w << x };
    } buffer_writer &operator << (buffer_writer &w, const T &x) {
        static_cast<writer<word> &> (w) << x;
        return w;
    }

    template <std::integral word, typename T> requires requires (reader<word> &w, T &x) {
        { w >> x };
    } buffer_writer &operator >> (buffer_writer &w, const T &x) {
        static_cast<reader<word> &> (w) >> x;
        return w;
    }

    struct buffer_session final : session<byte> {
        list<bytes> Results;
        buffer_writer Writer;

        buffer_session () : Results {}, Writer {} {}

        // Move semantics just to be super cool.
        buffer_session (buffer_session &&other) noexcept :
            Results {std::move (other.Results)}, Writer {std::move (other.Writer)} {}

        buffer_session &operator = (buffer_session &&other) noexcept {
            if (this != &other) {
                Results = std::move (other.Results);
                Writer = std::move (other.Writer);
            }

            return *this;
        }

        void write (const byte *b, size_t size) final override {
            Writer.write (b, size);
        }

        void complete () final override {
            Results <<= Writer.complete ();
        }
    };

    inline buffer_writer::buffer_writer () :
        reader<byte> {}, message_writer<bytes, byte> {}, CryptoPP::NotCopyable {}, Result {}, Sink {Result} {}

    // Move semantics just to be super cool.
    inline buffer_writer::buffer_writer (buffer_writer &&other) noexcept :
        reader<byte> {}, message_writer<bytes, byte> {}, CryptoPP::NotCopyable {},
        Result {std::move (other.Result)}, Sink {Result} {}

    inline buffer_writer &buffer_writer::operator = (buffer_writer &&other) noexcept {
        if (this != &other) {
            Result = std::move (other.Result);
            Sink = CryptoPP::StringSinkTemplate<bytes> {Result};
        }

        return *this;
    }

    void inline buffer_writer::write (const byte *b, size_t size) {
        Sink.Put (b, size);
    }

    void inline buffer_writer::read (byte *b, size_t size) {
        if (size != Sink.Get (b, size)) throw end_of_stream {};
    }

    void inline buffer_writer::skip (size_t size) {
        std::vector<byte> o;
        o.resize (size);
        read (o.data (), size);
    }

    // there is a bit of a contradiction here because you shouldn't
    // use read/skip together with complete. Just make sure you're
    // using one or the other.
    bytes inline buffer_writer::complete () {
        bytes b = Result;
        Result.clear ();
        return b;
    }

}

#endif


