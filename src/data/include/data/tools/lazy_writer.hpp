// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_LAZY_WRITER
#define DATA_TOOLS_LAZY_WRITER

#include <data/bytes.hpp>
#include <data/stack.hpp>
#include <concepts>

namespace data {

    // lazy writer can be used without knowing the size
    // of the data to be written beforehand.
    template <std::integral word> class lazy_writer : public writer<word> {
        bytestring<word> &Bytes;
        size_t Capacity;
        stack<std::vector<word>> Parts;

    public:
        size_t TotalSize {0};

        lazy_writer (bytestring<word> &b, size_t capacity = 1024 / sizeof (word)): Bytes {b}, Capacity {capacity}, Parts {} {
            Parts >>= std::vector<word> {};
            Parts.first ().reserve (capacity);
        }

        void write (const word* b, size_t size) final override {
            std::vector<word> *current = &first (Parts);
            std::size_t remaining = current->capacity () - current->size ();
            if (remaining < size) {
                current->insert (current->end (), b, b + remaining);
                size -= remaining;
                Parts >>= std::vector<word> {};
                current = &first (Parts);
                current->reserve (size > Capacity ? size : Capacity);
            }
            current->insert (current->end (), b, b + size);
            TotalSize += size;
        }

        ~lazy_writer () {
            Bytes.resize (TotalSize);
            iterator_writer w {Bytes.begin (), Bytes.end ()};
            for (const std::vector<word> &b : reverse (Parts)) w.write (b.data (), b.size ());
        }
    };

    using lazy_bytes_writer = lazy_writer<byte>;

    template <std::invocable<lazy_bytes_writer &> F>
    bytes inline write (F &&f) {
        return build_with<bytes, byte, lazy_bytes_writer> (std::forward<F> (f));
    }

    template <typename X, typename... P>
    bytes write_all (X &&x, P &&...p) {
        bytes b; {
            lazy_bytes_writer w {b};
            write_to_writer (w, std::forward<X> (x), std::forward<P> (p)...);
        }
        return b;
    }


}

#endif
