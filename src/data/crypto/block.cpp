
#include <data/crypto/block.hpp>
#include <data/crypto/transform.hpp>

namespace data::crypto::cipher::block {

    void add_padding_session::complete () {
        switch (Padding) {
            case (padding::NO_PADDING): {
                pad<padding::NO_PADDING> {}.write (Next, BlockSize, BytesWritten);
                return;
            }
            case (padding::ZEROS_PADDING): {
                pad<padding::ZEROS_PADDING> {}.write (Next, BlockSize, BytesWritten);
                break;
            }
            case (padding::PKCS_PADDING): {
                pad<padding::PKCS_PADDING> {}.write (Next, BlockSize, BytesWritten);
                break;
            }
            case (padding::W3C_PADDING): {
                pad<padding::W3C_PADDING> {}.write (Next, BlockSize, BytesWritten);
                break;
            }
            case (padding::ONE_AND_ZEROS_PADDING):
            default: pad<padding::ONE_AND_ZEROS_PADDING> {}.write (Next, BlockSize, BytesWritten);
        }
        Next.complete ();
    }

    void remove_padding_reader::complete () {
        switch (Padding) {
            case (padding::NO_PADDING): {
                pad<padding::NO_PADDING> {}.read (Previous, BlockSize, BytesRead);
                return;
            }
            case (padding::ZEROS_PADDING): {
                pad<padding::ZEROS_PADDING> {}.read (Previous, BlockSize, BytesRead);
                break;
            }
            case (padding::PKCS_PADDING): {
                pad<padding::PKCS_PADDING> {}.read (Previous, BlockSize, BytesRead);
                break;
            }
            case (padding::W3C_PADDING): {
                pad<padding::W3C_PADDING> {}.read (Previous, BlockSize, BytesRead);
                break;
            }
            case (padding::ONE_AND_ZEROS_PADDING):
            default: pad<padding::ONE_AND_ZEROS_PADDING> {}.read (Previous, BlockSize, BytesRead);
        }
        Previous.complete ();
    }

    bytes add_padding (padding_scheme scheme, size_t block_size, const bytes &msg) {
        buffer_session br;
        add_padding_session w {br, block_size, scheme};
        w << msg << end_message {};
        return br.Results.first ();
    }

    bytes remove_padding (padding_scheme scheme, size_t block_size, const bytes &bb) {
        switch (scheme) {
            case (padding::NO_PADDING): return bb;

            case (padding::PKCS_PADDING): {
                if (bb.size () == 0) throw invalid_padding {};
                if (bb.size () % block_size != 0) throw invalid_padding {};
                byte last = bb[bb.size () - 1];
                // last is the expected number of padding digits.
                // all padding digits must be equal to last.
                if (last == 0) throw invalid_padding {};
                if (last > bb.size ()) throw invalid_padding {};
                for (int i = bb.size () - 2; i >= int64 (bb.size () - last); i--) if (bb[i] != last) throw invalid_padding {};
                return take (bb, bb.size () - last);
            }

            case (padding::ONE_AND_ZEROS_PADDING): {
                if (bb.size () == 0) throw invalid_padding {};
                if (bb.size () % block_size != 0) throw invalid_padding {};
                uint32 padding_size = 1;
                int i = bb.size () - 1;
                while (i > 0) {
                    if (bb[i] != 0) break;
                    padding_size++;
                    i--;
                }
                if (bb[i] != 0x80) throw invalid_padding {};
                return take (bb, bb.size () - padding_size);
            }

            case (padding::ZEROS_PADDING):
            case (padding::W3C_PADDING):
            default: throw exception {} <<
                "Try to remove padding scheme that cannot be unambigously identified. This occurs when we use W3C or pad with zeros.";
        }
    }

    data::writer<byte> &pad<padding::ZEROS_PADDING>::write (data::writer<byte> &w, size_t block_size, size_t &bytes_written) {
        size_t bytes_to_write = (block_size - (bytes_written % block_size)) % block_size;
        bytes_written += bytes_to_write;
        for (int i = 0; i < bytes_to_write; i++) w << byte (0);
        return w;
    }

    data::reader<byte> &pad<padding::ZEROS_PADDING>::read (data::reader<byte> &r, size_t block_size, size_t &bytes_read) {
        size_t bytes_to_read = (block_size - (bytes_read % block_size)) % block_size;
        bytes_read += bytes_to_read;
        for (int i = 0; i < bytes_to_read; i++) {
            byte b;
            r >> b;
            if (b != 0) throw invalid_padding {};
        }
        return r;
    }

    data::writer<byte> &pad<padding::PKCS_PADDING>::write (data::writer<byte> &w, size_t block_size, size_t &bytes_written) {
        size_t bytes_to_write = block_size - (bytes_written % block_size);
        if (bytes_to_write > 255) throw exception {} << "block size must be less than 256 for PKCS";
        bytes_written += bytes_to_write;
        for (int i = 0; i < bytes_to_write; i++) w << byte (bytes_to_write);
        return w;
    }

    data::reader<byte> &pad<padding::PKCS_PADDING>::read (data::reader<byte> &r, size_t block_size, size_t &bytes_read) {
        size_t bytes_to_read = block_size - (bytes_read % block_size);
        if (bytes_to_read > 255) throw exception {} << "block size must be less than 256 for PKCS";
        bytes_read += bytes_to_read;
        for (int i = 0; i < bytes_to_read; i++) {
            byte b;
            r >> b;
            if (b != bytes_to_read) throw invalid_padding {};
        }
        return r;
    }

    data::writer<byte> &pad<padding::ONE_AND_ZEROS_PADDING>::write (data::writer<byte> &w, size_t block_size, size_t &bytes_written) {
        // we always write at least one byte of padding.
        size_t bytes_to_write = block_size - (bytes_written % block_size);
        bytes_written += bytes_to_write;
        w << byte (0x80);
        for (int i = 1; i < bytes_to_write; i++) w << byte (0);
        return w;
    }

    data::reader<byte> &pad<padding::ONE_AND_ZEROS_PADDING>::read (data::reader<byte> &r, size_t block_size, size_t &bytes_read) {
        size_t bytes_to_read = block_size - (bytes_read % block_size);
        if (bytes_to_read == 0) bytes_to_read = block_size;
        bytes_read += bytes_to_read;
        byte first;
        r >> first;
        if (first != 0x80) throw invalid_padding {};
        for (int i = 1; i < bytes_to_read; i++) {
            byte b;
            r >> b;
            if (b != 0) throw invalid_padding {};
        }
        return r;
    }

    data::writer<byte> &pad<padding::W3C_PADDING>::write (data::writer<byte> &w, size_t block_size, size_t &bytes_written) {
        size_t bytes_to_write = (block_size - (bytes_written % block_size)) % block_size;
        if (bytes_to_write > 255) throw exception {} << "block size must be less than 256 for W3C";
        bytes_written += bytes_to_write;
        for (int i = 0; i < bytes_to_write; i++) w << byte (bytes_to_write);
        return w;
    }

    data::reader<byte> &pad<padding::W3C_PADDING>::read (data::reader<byte> &r, size_t block_size, size_t &bytes_read) {
        size_t bytes_to_read = (block_size - (bytes_read % block_size)) % block_size;
        if (bytes_to_read > 255) throw exception {} << "block size must be less than 256 for W3C";
        bytes_read += bytes_to_read;
        for (int i = 0; i < bytes_to_read; i++) {
            byte b;
            r >> b;
            if (b != bytes_to_read) throw invalid_padding {};
        }
        return r;
    }

}
