// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_LAZY_WRITER
#define DATA_TOOLS_LAZY_WRITER

#include <data/stack.hpp>
#include <concepts>

namespace data {

    // something that can be move-constructed via a vector.
    template <typename bytes>
    concept moved_vector_constructible = requires (std::vector<unref<decltype (std::declval<bytes> ()[0])>> &&v) {
            { bytes {std::move (v)} };
        };

    // lazy writer can be used without knowing the size
    // of the data to be written beforehand.
    template <moved_vector_constructible bytes, std::integral word = unref<decltype (std::declval<bytes> ()[0])>>
    class lazy_writer : public writer<word> {
        bytes &Bytes;
        size_t Capacity;
        stack<std::vector<word>> Parts;

    public:
        size_t TotalSize {0};

        lazy_writer (bytes &b, size_t capacity = 1024 / sizeof (word)) noexcept: Bytes {b}, Capacity {capacity}, Parts {} {
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
            std::vector<word> v (TotalSize);
            iterator_writer vv {v.begin (), v.end ()};
            for (const std::vector<word> &b : reverse (Parts)) vv.write (b.data (), b.size ());
            Bytes = bytes {std::move (v)};
        }
    };

    /*
     * use write<result> to write some data with a lazy writer.
     *
     *     write an empty result
     *         write<result> ();
     *
     *     provide a function that takes a Writer as an argument and
     *     the result will be this funciton applied to a lazy_bytes_writer
     *         write<result> (function that takes a writer);
     *
     *     Provide a series of serializable values and write them in
     *     sequence to the result.
     *         write<result> (series, of, serializable, values...);
     *
     *     Put a size first if you know how big the sequence will be.
     *     (size_t is not serializable so there is no confusing here.)
     *         write<result> (size_t, series, of, serializable, values...);
     *
     */

    template <moved_vector_constructible bytes>
    bytes inline write () {
        return bytes {};
    }

    // apply function provided with a writer that will write the result.
    template <moved_vector_constructible bytes, typename F>
    requires std::invocable<F, lazy_writer<bytes> &> && (!Serializable<F>)
    bytes inline write (F &&f) {
        return build_with<bytes, lazy_writer<bytes>> (std::forward<F> (f));
    }

    template <moved_vector_constructible bytes, typename... P>
    requires (Serializable<P> && ...) && (sizeof...(P) > 0)
    bytes inline write (P &&...p) {
        return build<bytes, lazy_writer<bytes>> (p...);
    }

    template <moved_vector_constructible bytes, typename... P>
    bytes inline write (size_t size, P &&...p) {
        std::vector<byte> b (size);
        iterator_writer w (b.begin (), b.end ());
        write_to_writer (w, std::forward<P> (p)...);
        if (w.Begin != w.End) throw exception {} << "failed to fill written data with the expected size.";
        return b;
    }

}

#endif
