// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_PADDING
#define DATA_CRYPTO_BLOCK_PADDING

#include <data/stream.hpp>
#include <data/bytes.hpp>
#include <data/io/exception.hpp>
#include <cryptopp/filters.h>

namespace data::crypto::CryptoPP {
    using namespace ::CryptoPP;
}

namespace data::crypto {
    using block_padding_scheme = CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme;
    using block_padding = CryptoPP::BlockPaddingSchemeDef;

    struct invalid_padding : std::runtime_error {
        invalid_padding (): std::runtime_error {"invalid padding detected"} {}
    };

    bytes add_padding (block_padding_scheme, size_t block_size, const bytes &);
    bytes remove_padding (block_padding_scheme, size_t block_size, const bytes &);

    struct add_padding_session final : session<byte> {
        session<byte> &Next;

        size_t BlockSize;
        block_padding_scheme Padding;
        size_t BytesWritten;

        add_padding_session (session<byte> &next, size_t block_size, block_padding_scheme padding);

        void write (const byte *b, size_t size) final override;

        // add the padding.
        void complete () final override;
    };

    // a new type of reader that lets you say when a message is done in order to skip padding.
    template <std::integral word>
    struct message_reader : reader<word> {
        virtual void complete () = 0;

        virtual ~message_reader () {}
    };

    struct remove_padding_reader final : message_reader<byte> {
        reader<byte> &Previous;

        size_t BlockSize;
        block_padding_scheme Padding;
        size_t BytesRead;

        remove_padding_reader (reader<byte> &p, block_padding_scheme pp);

        void read (byte *b, size_t size) final override;
        void skip (size_t size) final override;
        void complete () final override;
    };

    template <block_padding_scheme padding> struct pad;

    template <> struct pad<block_padding::NO_PADDING> {
        reader<byte> &read (reader<byte> &r, size_t block_size, size_t &bytes_read) {
            return r;
        }

        writer<byte> &write (writer<byte> &w, size_t block_size, size_t &bytes_written) {
            return w;
        }
    };

    template <> struct pad<block_padding::ZEROS_PADDING> {
        writer<byte> &write (writer<byte> &w, size_t block_size, size_t &bytes_written);
        reader<byte> &read (reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    template <> struct pad<block_padding::PKCS_PADDING> {
        writer<byte> &write (writer<byte> &w, size_t block_size, size_t &bytes_written);
        reader<byte> &read (reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    template <> struct pad<block_padding::ONE_AND_ZEROS_PADDING> {
        writer<byte> &write (writer<byte> &w, size_t block_size, size_t &bytes_written);
        reader<byte> &read (reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    template <> struct pad<block_padding::W3C_PADDING> {
        writer<byte> &write (writer<byte> &w, size_t block_size, size_t &bytes_written);
        reader<byte> &read (reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    inline add_padding_session::add_padding_session (session<byte> &next, size_t block_size, block_padding_scheme padding):
        Next {next}, BlockSize {block_size}, Padding {padding}, BytesWritten {0} {}

    inline void add_padding_session::write (const byte *b, size_t size) {
        Next.write (b, size);
        BytesWritten += size;
    }

    void inline remove_padding_reader::read (byte *b, size_t size) {
        Previous.read (b, size);
        BytesRead += size;
    }

    void inline remove_padding_reader::skip (size_t size) {
        Previous.skip (size);
        BytesRead += size;
    }

}

#endif
