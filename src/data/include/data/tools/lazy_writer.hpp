// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_LAZY_WRITER
#define DATA_TOOLS_LAZY_WRITER

#include <data/bytes.hpp>
#include <data/list.hpp>

namespace data {

    // lazy writer can be used without knowing the size
    // of the data to be written beforehand.
    template <std::integral word> struct lazy_writer : message_writer<bytestring<word>, word> {
        list<bytestring<word>> Bytes;

        void write (const word* b, size_t size) final override {
            Bytes = Bytes << bytestring<word> (view<word> {b, size});
        }

        operator bytestring<word> () const {
            size_t size = 0;
            for (const bytestring<word> &b : Bytes) size += b.size ();
            bytestring<word> z (size);
            iterator_writer w {z.begin (), z.end ()};
            for (const bytestring<word> &b : Bytes) w << b;
            return z;
        }

        bytestring<word> complete () final override {
            return bytestring<word> (*this);
        }
    };

    using lazy_bytes_writer = lazy_writer<byte>;

}

#endif