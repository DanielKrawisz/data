// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_MAC
#define DATA_CRYPTO_MAC

#include <data/crypto/digest.hpp>

namespace data::crypto::MAC {

    template <typename W, size_t key_size>
    concept writer = std::derived_from<W, message_writer<digest<W::size>, byte>> && requires (const symmetric_key<key_size> &k) {
        { W {k} };
        { W::size };
    };

    template <size_t key_size, writer<key_size> W>
    digest<W::size> inline calculate (const symmetric_key<key_size> &k, bytes_view b) {
        W w {k};
        w.write (b.data (), b.size ());
        return w.complete ();
    }

}

#endif
