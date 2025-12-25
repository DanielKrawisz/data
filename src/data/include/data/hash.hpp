// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HASH
#define DATA_HASH

#include <data/stream.hpp>
#include <data/math/number/bounded.hpp>

namespace data::hash {

    // the type returned by a hash function.
    template <size_t s> struct digest;

    template <size_t size> std::ostream inline &operator << (std::ostream &o, const digest<size> &s);
    
    template <size_t s> struct digest : public math::uint_little<s, byte> {
        using math::uint_little<s, byte>::uint_little;
        digest (const math::uint_little<s, byte> &u) : math::uint_little<s, byte> {u} {}
        bool valid () const;
    };

    using digest128 = digest<16>;
    using digest160 = digest<20>;
    using digest224 = digest<28>;
    using digest256 = digest<32>;
    using digest320 = digest<40>;
    using digest384 = digest<48>;
    using digest448 = digest<56>;
    using digest512 = digest<64>;

    // provide a reference to a hash and it gets written to when the writer is destructed.
    template <typename W> concept Writer = data::Builder<W, typename W::digest, byte>;

    // calculate a hash using a hash builder.
    template <Writer W> W::digest inline calculate (byte_slice b) {
        return build_with<typename W::digest, byte, W> ([b] (W &w) {
            w << b;
        });
    }

    template <Writer W> W::digest inline calculate (string_view b) {
        return build_with<typename W::digest, byte, W> ([b] (W &w) {
            w.write ((byte *) b.data (), b.size ());
        });
    }

    // Engine is a more primitive concept than Writer,
    // designed to work with cryptopp
    template <typename W> concept Engine = requires () {
        W {};
    } && requires (W &w, const byte *b, size_t size) {
        { w.Update (b, size) };
    } && requires (W &w, byte b[W::DigestSize]) {
        { w.Final (b) };
    } && requires (W &w) {
        { w.Restart () };
    };

    template <Engine W> digest<W::DigestSize> calculate (byte_slice b) {
        W w {};
        w.Update (b.data (), b.size ());
        digest<W::DigestSize> d;
        w.Final (d.data ());
        return d;
    }

    template <Engine W> digest<W::DigestSize> calculate (string_view b) {
        W w {};
        w.Update ((byte *) b.data (), b.size ());
        digest<W::DigestSize> d;
        w.Final (d.data ());
        return d;
    }

    // given an engine, construct a writer.
    template <Engine engine> struct writer : data::writer<byte> {
        using digest = hash::digest<engine::DigestSize>;
        writer (digest &d) noexcept: Digest {d}, Hash {} {}

        void write (const byte *b, size_t bytes) noexcept final override {
            Hash.Update (b, bytes);
        }

        ~writer () {
            Hash.Final (Digest.data ());
        }

        writer (const writer &) = delete;
        writer &operator = (const writer&) = delete;
        writer (writer&&) = delete;
        writer &operator = (writer &&) = delete;

    private:
        digest &Digest;
        engine Hash;
    };

    template<size_t s>
    bool inline digest<s>::valid () const {
        return *this != digest {0};
    }

    template <size_t size>
    std::ostream inline &operator << (std::ostream &o, const digest<size> &s) {
        return o << encoding::hexidecimal::write (s);
    }

    
}

#endif

