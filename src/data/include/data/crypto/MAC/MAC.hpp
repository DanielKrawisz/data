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
    } && requires (W &w, byte b[W::Size]) {
        { w.Final (b) };
    } && requires (W &w) {
        { w.Restart () };
    };

    template <size_t key_size, Writer W>
    W::digest inline calculate (const symmetric_key<key_size> &k, byte_slice b) {
        typename W::digest Digest;
        W {Digest, k}.write (b.data (), b.size ());
        return Digest;
    }

    template <size_t key_size, Engine W>
    hash::digest<W::Size> inline calculate (const symmetric_key<key_size> &k, byte_slice b) {
        typename W::digest Digest;
        W w {k};
        w.Update (b.data (), b.size ());
        hash::digest<W::Size> d;
        w.Final (d.data ());
        return d;
    }

    template <Engine W, size_t key_size>
    struct writer : data::writer<byte> {
        using digest = hash::digest<W::Size>;
        writer (digest &d, byte_slice key): Digest {d}, MAC {key} {}

        void write (const byte *b, size_t bytes) final override {
            MAC.Update (b, bytes);
        }

        ~writer () {
            MAC.Final (Digest.data ());
        }

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
