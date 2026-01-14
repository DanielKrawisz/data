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

    bytes add_padding (padding_scheme, size_t block_size, const bytes &);
    bytes remove_padding (padding_scheme, size_t block_size, const bytes &);

    struct add_padding_writer final : data::writer<byte> {
        data::writer<byte> &Next;

        size_t BlockSize;
        padding_scheme Padding;
        size_t BytesWritten;

        add_padding_writer (data::writer<byte> &next, size_t block_size, padding_scheme padding);

        void write (const byte *b, size_t size) final override;

        ~add_padding_writer () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        // add the padding.
        void complete ();
    };

    struct remove_padding_reader final : in_session<byte> {
        in_session<byte> &Previous;

        size_t BlockSize;
        padding_scheme Padding;
        size_t BytesRead;

        remove_padding_reader (reader<byte> &p, padding_scheme pp);

        void read (byte *b, size_t size) final override;
        void skip (size_t size) final override;
        void complete () final override;
    };

    data::writer<byte> &add_padding (data::writer<byte> &, padding_scheme, size_t block_size, size_t bytes_written);
    data::reader<byte> &skip_padding (data::reader<byte> &, padding_scheme, size_t block_size, size_t bytes_read);

    void inline add_padding_writer::complete () {
        add_padding (Next, Padding, BlockSize, BytesWritten);
    }

    void inline remove_padding_reader::complete () {
        skip_padding (Previous, Padding, BlockSize, BytesRead);
    }

    bytes inline add_padding (padding_scheme scheme, size_t block_size, const bytes &msg) {
        return build_with<bytes, lazy_bytes_writer> ([&](auto &&w){
            add_padding_writer pp {w, block_size, scheme};
            pp << msg;
        });
    }

    template <padding_scheme padding> struct pad;

    template <> struct pad<padding::NO_PADDING> {
        data::reader<byte> &read (data::reader<byte> &r, size_t block_size, size_t &bytes_read) {
            return r;
        }

        data::writer<byte> &write (data::writer<byte> &w, size_t block_size, size_t &bytes_written) {
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

    inline add_padding_writer::add_padding_writer (data::writer<byte> &next, size_t block_size, padding_scheme padding):
        Next {next}, BlockSize {block_size}, Padding {padding}, BytesWritten {0} {}

    inline void add_padding_writer::write (const byte *b, size_t size) {
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
