// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_WRITER
#define DATA_CRYPTO_BLOCK_WRITER

#include <data/slice.hpp>

namespace data::crypto {

    template <typename W, uint16 block_size>
    concept block_writer = requires (W &w, slice<const byte, block_size> x) {
        { w.write (x) };
    };

    using padding = function <writer<byte> &(writer<byte> &, uint16 Index)>;

    template <uint16 block_size, block_writer<block_size> W>
    struct padded_block_writer {
        void write (byte *, size_t size);
        void complete ();

    private:
        W &Writer;
        padding Padding;
        byte_array<block_size> Block;
        uint16 Index;
    };

    template <uint16 block_size, block_writer<block_size> W>
    void padded_block_writer<block_size, W>::write (byte *b, size_t size) {
        if (size + Index >= block_size) {
            size_t step_size = block_size - Index;
            write (b, step_size);
            write (b + step_size, size - step_size);
        }

        std::copy (b, Block.begin () + Index, Block.begin () + Index + size);
        Index += size;
        if (Index == block_size) {
            Index = 0;
            Writer.write (Block);
        }
    }

    template <uint16 block_size, block_writer<block_size> W>
    void inline padded_block_writer<block_size, W>::complete () {
        padding (*this, Index);
    }

}

#endif
