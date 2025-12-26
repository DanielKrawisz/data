// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HASH
#define DATA_HASH

/*
 *  data/hash.hpp
 *  --------------
 *
 *  This header defines the core abstractions for hash functions used throughout
 *  the data::hash namespace.  These abstractions are intentionally generic and
 *  are not limited to cryptographic hash functions; weak or non-cryptographic
 *  hashes may also be modeled for testing, demonstration, or non-adversarial
 *  use cases.
 *
 *  Overview:
 *     * digest<size>            — fixed-size hash output value type
 *     * Writer                  — RAII streaming hash interface
 *     * calculate<Writer>       — one-shot hashing via Writer
 *     * Engine                  — explicit stateful hash interface
 *     * calculate<Engine>       — one-shot hashing via Engine
 *     * writer<Engine>          — adapter from Engine to Writer
 *
 *  ---------------------------------------------------------------------------
 *  Design summary
 *  ---------------------------------------------------------------------------
 *
 *  Writer is the preferred, safe, user-facing abstraction. Finalization is
 *  tied to scope, not method calls.
 *
 *  Engine exists for low-level and backend integration.
 *
 *  hash::calculate is defined in terms of Writers and Engines
 *
 *  This separation allows the library to support both simple hashing use cases
 *  and advanced cryptographic constructions without forcing a single usage
 *  style on all consumers. The design favors correctness, composability, and
 *  clear ownership.
 *
 *  ---------------------------------------------------------------------------
 *  Digest type
 *  ---------------------------------------------------------------------------
 *
 *  The primary value type associated with hashing is:
 *
 *      data::hash::digest<size>
 *
 *  A data::hash::digest<size> represents a fixed-size hash output of `size`
 *  bytes. It inherits from data::math::uint_little<size>, which behaves like a
 *  built-in unsigned integer type with little-endian storage.  This allows
 *  digests to be compared, copied, serialized, and manipulated using familiar
 *  numeric semantics, while still representing raw hash output.
 *
 *  Hash functions are *not required* to return digest<size>, but it is the
 *  standard return type used by this library.
 *
 *  ---------------------------------------------------------------------------
 *  Hash Writer concept
 *  ---------------------------------------------------------------------------
 *
 *  The primary user-facing abstraction for hashing is the Writer concept:
 *
 *      data::hash::Writer
 *
 *  A hash Writer models a streaming hash computation using RAII semantics.
 *  It behaves like an output stream (it refines data::writer<byte>) and is
 *  written to using operator<< or write () calls.
 *
 *  Key properties of a Writer W:
 *
 *  W is constructed with a reference to a W::digest
 *  Bytes written to W contribute to the hash computation
 *  The digest is finalized and written *exactly once*, when W is destroyed
 *  W is typically non-copyable and non-movable, like std::ostream
 *
 *  Example usage:
 *
 *      data::hash::digest<32> d;
 *      {
 *          my_hash_writer w {d};
 *          w << bytes1 << bytes2;
 *      }   // digest finalized here
 *      // d now contains the hash
 *
 *  This design deliberately avoids explicit `finalize()` or `reset()` calls.
 *  Finalization is tied to object lifetime, which:
 *
 *  Prevents accidental reuse after finalization
 *  Eliminates forgotten finalize/reset bugs
 *  Matches standard C++ stream usage patterns
 *  Encourages correct one-shot hashing by default
 *
 *  The Writer abstraction is intended to be the *natural* way users interact
 *  with hash functions.
 *
 *  ---------------------------------------------------------------------------
 *  hash::calculate<Writer>
 *  ---------------------------------------------------------------------------
 *
 *  For convenience, the function:
 *
 *      data::hash::calculate<Writer> (bytes) -> Writer::digest
 *
 *  performs a complete hash computation using a Writer internally.
 *
 *  ---------------------------------------------------------------------------
 *  Hash Engine abstraction
 *  ---------------------------------------------------------------------------
 *
 *  In addition to Writer, this header defines a lower-level abstraction:
 *
 *      data::hash::Engine
 *
 *  An Engine represents a traditional stateful hash API with explicit control
 *  over lifecycle:
 *
 *  Update (data, size)
 *  Final (output)
 *  Restart ()
 *
 *  Engines exist primarily to support protocol-level and backend integrations,
 *  such as adapting libraries like Crypto++ which expose similar interfaces.
 *
 *  Engines are useful when:
 *
 *  A hash must be reused across multiple messages
 *  Hashing is embedded inside a larger protocol state machine
 *  Explicit restart/finalization boundaries are required
 *  Interfacing with existing cryptographic libraries
 *
 *  Engines are intentionally *not* the primary user interface, as they are
 *  easier to misuse (e.g. forgetting to finalize, reusing state incorrectly).
 *  Instead, Engines are typically adapted into Writers, allowing higher-level
 *  code to retain RAII safety while still leveraging lower-level implementations.
 *
 *  ---------------------------------------------------------------------------
 *  data::hash::calculate<Engine>
 *  ---------------------------------------------------------------------------
 *
 *  For convenience, we also have
 *
 *      data::hash::calculate<Engine> (bytes) -> Writer::digest
 *
 *  to perform a hash computation as a single function call.
 *
 *  ---------------------------------------------------------------------------
 *  data::hash::writer<Engine>
 *  ---------------------------------------------------------------------------
 *
 *  Construct a Writer from an Engine.
 *
 */

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
        writer &operator = (const writer &) = delete;
        writer (writer &&) = delete;
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

    template <Writer W, typename ...X> W::digest inline all (X &&...x) {
        return build<W::digest, byte, W> (std::forward<X> (x)...);
    }

    template <Engine E, typename ...X> digest<E::DigestSize> inline all (X &&...x) {
        return all<digest<E::DigestSize>, byte, writer<E>> (std::forward<X> (x)...);
    }

    
}

#endif

