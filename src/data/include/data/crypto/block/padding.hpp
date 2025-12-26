// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_PADDING
#define DATA_CRYPTO_BLOCK_PADDING

#include <data/stream.hpp>
#include <data/bytes.hpp>
#include <data/crypto/block/mode.hpp>
#include <data/io/exception.hpp>
#include <cryptopp/filters.h>

namespace data::crypto::cipher::block::CryptoPP {
    using namespace ::CryptoPP;
}

namespace data::crypto::cipher::block {
    // padding scheme is the type, padding contains the values.
    using padding_scheme = CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme;
    using padding = CryptoPP::BlockPaddingSchemeDef;

    struct invalid_padding : exception {
        invalid_padding (): exception {"invalid padding detected"} {}
    };

    template <typename W, typename mode, size_t key_size>
    concept Writer = data::Writer<W, byte> && requires (data::writer<byte> &out, const mode &m, const symmetric_key<key_size> &k, padding_scheme p) {
        { W {out, m, k, p} };
    };

    template <typename cipher, size_t key_size, Mode<cipher, key_size> mode> struct encryptor {
        encryptor (data::writer<byte> &out, const mode &m, const symmetric_key<key_size> &key, padding_scheme = padding::DEFAULT_PADDING);
        ~encryptor ();

        data::writer<byte> &Out;
        mode Mode;
        symmetric_key<key_size> Key;
        padding_scheme Padding;
        size_t BytesWritten;
    };

    template <typename cipher, size_t key_size, Mode<cipher, key_size> mode> struct decryptor {
        decryptor (data::writer<byte> &out, const mode &m, const symmetric_key<key_size> &key, padding_scheme = padding::DEFAULT_PADDING);
        ~decryptor ();

        data::writer<byte> &Out;
        mode Mode;
        symmetric_key<key_size> Key;
        padding_scheme Padding;
        size_t BytesWritten;
    };

    bytes add_padding (padding_scheme, size_t block_size, const bytes &);
    bytes remove_padding (padding_scheme, size_t block_size, const bytes &);

    struct add_padding_session final : out_session<byte> {
        data::writer<byte> &Next;

        size_t BlockSize;
        padding_scheme Padding;
        size_t BytesWritten;

        add_padding_session (out_session<byte> &next, size_t block_size, padding_scheme padding);

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
        padding_scheme Padding;
        size_t BytesRead;

        remove_padding_reader (reader<byte> &p, padding_scheme pp);

        void read (byte *b, size_t size) final override;
        void skip (size_t size) final override;
        void complete () final override;
    };

    template <padding_scheme padding> struct pad;

    template <> struct pad<padding::NO_PADDING> {
        reader<byte> &read (reader<byte> &r, size_t block_size, size_t &bytes_read) {
            return r;
        }

        writer<byte> &write (writer<byte> &w, size_t block_size, size_t &bytes_written) {
            return w;
        }
    };

    template <> struct pad<padding::ZEROS_PADDING> {
        data::writer<byte> &write (data::writer<byte> &w, size_t block_size, size_t &bytes_written);
        data::reader<byte> &read (data::reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    template <> struct pad<padding::PKCS_PADDING> {
        data::writer<byte> &write (data::writer<byte> &w, size_t block_size, size_t &bytes_written);
        data::reader<byte> &read (data::reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    template <> struct pad<padding::ONE_AND_ZEROS_PADDING> {
        data::writer<byte> &write (data::writer<byte> &w, size_t block_size, size_t &bytes_written);
        data::reader<byte> &read (data::reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    template <> struct pad<padding::W3C_PADDING> {
        data::writer<byte> &write (data::writer<byte> &w, size_t block_size, size_t &bytes_written);
        data::reader<byte> &read (data::reader<byte> &r, size_t block_size, size_t &bytes_read);
    };

    inline add_padding_session::add_padding_session (out_session<byte> &next, size_t block_size, padding_scheme padding):
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
