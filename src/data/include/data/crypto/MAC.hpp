// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_MAC
#define DATA_CRYPTO_MAC

#include <data/hash.hpp>
#include <data/crypto/encrypted.hpp>

namespace data::crypto::MAC {

    template <typename W, size_t key_size>
    concept Writer = requires (typename W::digest &d, symmetric_key<key_size> key) {
            W {d, key};
        } && data::Writer<W, byte>;

    template <typename W, size_t key_size>
    concept Engine = requires (const symmetric_key<key_size> &k) {
            W {k};
        } && requires (W &w, const byte *b, size_t size) {
            { w.Update (b, size) };
        } && requires (W &w, byte b[W::DigestSize]) {
            { w.Final (b) };
        } && requires (W &w) {
            { w.Restart () };
        };

    template <typename W, size_t key_size> requires Writer<W, key_size>
    W::digest inline calculate (const symmetric_key<key_size> &key, byte_slice b) {
        typename W::digest Digest;
        W {Digest, key}.write (b.data (), b.size ());
        return Digest;
    }

    template <typename W, size_t key_size> requires Engine<W, key_size>
    hash::digest<W::DigestSize> inline calculate (const symmetric_key<key_size> &key, byte_slice b) {
        hash::digest<W::DigestSize> Digest;
        W w {key};
        w.Update (b.data (), b.size ());
        hash::digest<W::DigestSize> d;
        w.Final (d.data ());
        return d;
    }

    // make a writer out of an engine.
    template <typename W>
    struct writer : data::writer<byte> {
        using digest = hash::digest<W::DigestSize>;
        template <size_t key_size> requires Engine<W, key_size>
        writer (digest &d, const symmetric_key<key_size> &key) noexcept: Digest {d}, MAC {key} {}

        void write (const byte *b, size_t bytes) noexcept final override {
            MAC.Update (b, bytes);
        }

        ~writer () {
            MAC.Final (Digest.data ());
        }

        writer (const writer &) = delete;
        writer &operator = (const writer &) = delete;
        writer (writer&&) = delete;
        writer &operator = (writer &&) = delete;

    private:
        digest &Digest;
        W MAC;
    };

    // attach a mack to every message.
    template <typename W>
    struct append_MAC_writer final : data::out_session<byte> {
        data::out_session<byte> &Output;
        W MAC;

        template <size_t key_size> requires Engine<W, key_size>
        append_MAC_writer (data::out_session<byte> &o, const symmetric_key<key_size> &k):
        Output {o}, MAC {k} {}

        void write (const byte *, size_t size) final override;

        // add the final padding.
        void complete () final override;
    };

    template <typename W, size_t key_size, std::invocable<W &> F>
    requires Writer<W, key_size> && std::invocable<F, W &> && (!Serializable<F>)
    W::digest inline write (const symmetric_key<key_size> &k, F &&f) {
        return build_with<typename W::digest, W> (std::forward<F> (f), k);
    }

    template <typename W, size_t key_size, std::invocable<W &> F>
    requires Engine<W, key_size> && std::invocable<F, writer<W> &> && (!Serializable<F>)
    hash::digest<W::DigestSize> inline write (const symmetric_key<key_size> &k, F &&f) {
        return write<writer<W>> (std::forward<F> (f), k);
    }

    template <typename W, size_t key_size, Serializable ...X>
    requires Writer<W, key_size>
    W::digest inline write (const symmetric_key<key_size> &k, X &&...x) {
        return build_with<typename W::digest, W> ([&](auto &&w) {
            (w << ... << std::forward<X> (x));
        }, k);
    }

    template <typename E, size_t key_size, Serializable ...X> requires Engine<E, key_size>
    hash::digest<E::DigestSize> inline write (const symmetric_key<key_size> &k, X &&...x) {
        return write<writer<E>> (k, std::forward<X> (x)...);
    }

}

#endif
