// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_STREAM_ONE_WAY
#define DATA_CRYPTO_STREAM_ONE_WAY

#include <data/crypto/block/mode.hpp>
#include <data/crypto/hash.hpp>

namespace data::crypto {

    // a stream cipher based on a one-way function. Note: you won't necessarily
    // produce a secure stream simply by putting any hash function in here.
    // You can use mode_output_feedback for some block cipher and you can use SHA3.
    template <size_t block_size> struct one_way_stream;

    template <size_t block_size, size_t key_size, block_cipher<block_size, key_size> cipher>
    one_way_stream<block_size> output_feedback_stream (
        const byte_array<block_size> &IV,
        const symmetric_key<key_size> &Key);

    template <size_t block_size>
    one_way_stream<block_size> SHA3_stream (const byte_array<block_size> &IV);

    template <size_t block_size>
    struct one_way_stream {
        function<byte_array<block_size> (const byte_array<block_size> &)> OneWay;
        byte_array<block_size> Next;
        byte Index;

        one_way_stream (
            function<byte_array<block_size> (const byte_array<block_size> &)> one_way,
            const byte_array<block_size> &IV): OneWay {one_way}, Next {OneWay (IV)}, Index {0} {}

        // both encrypt and decrypt are the same operation.
        byte crypt (byte);

    };

    template <size_t block_size, size_t key_size, block_cipher<block_size, key_size> cipher>
    one_way_stream<block_size> inline output_feedback_stream (
        const byte_array<block_size> &IV,
        const symmetric_key<key_size> &Key) {
        return one_way_stream<block_size> {[key = Key] (const byte_array<block_size> &in) -> byte_array<block_size> {
            return mode_output_feedback<block_size>::template update<cipher, key_size> (key, in);
        }, IV};
    }

    template <size_t block_size>
    one_way_stream<block_size> inline SHA3_stream (const byte_array<block_size> &IV) {
        return one_way_stream<block_size> {[] (const byte_array<block_size> &in) -> byte_array<block_size> {
            hash::digest<block_size> ddd = SHA3<block_size> (in);
            return static_cast<byte_array<block_size>> (ddd);
        }, IV};
    }

    template <size_t block_size>
    byte inline one_way_stream<block_size>::crypt (byte b) {
        if (Index == block_size) {
            Next = OneWay (Next);
            Index = 0;
        }

        return Next[Index++] ^= b;
    }

}

#endif
