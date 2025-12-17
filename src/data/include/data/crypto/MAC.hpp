// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_MAC
#define DATA_CRYPTO_MAC

#include <data/hash.hpp>

namespace data::crypto::MAC {

    template <typename W>
    concept Writer = requires (typename W::digest &d, byte_slice key) {
        W {d, key};
    } && data::Writer<W, byte>;

    template <typename W> concept Engine = requires (byte_slice k) {
        W {k};
    } && requires (W &w, const byte *b, size_t size) {
        { w.Update (b, size) };
    } && requires (W &w, byte b[W::DigestSize]) {
        { w.Final (b) };
    } && requires (W &w) {
        { w.Restart () };
    };

    template <Writer W>
    W::digest inline calculate (byte_slice key, byte_slice b) {
        typename W::digest Digest;
        W {Digest, key}.write (b.data (), b.size ());
        return Digest;
    }

    template <Engine W>
    hash::digest<W::DigestSize> inline calculate (byte_slice key, byte_slice b) {
        hash::digest<W::DigestSize> Digest;
        W w {key};
        w.Update (b.data (), b.size ());
        hash::digest<W::DigestSize> d;
        w.Final (d.data ());
        return d;
    }

    template <Engine W>
    struct writer : data::writer<byte> {
        using digest = hash::digest<W::DigestSize>;
        writer (digest &d, byte_slice key) noexcept: Digest {d}, MAC {key} {}

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
    template <Engine W, size_t key_size>
    struct append_MAC_writer final : data::session<byte> {
        data::session<byte> &Output;
        W MACEngine;

        void write (const byte *, size_t size) final override;

        // add the final padding.
        void complete () final override;
    };

}

#endif
