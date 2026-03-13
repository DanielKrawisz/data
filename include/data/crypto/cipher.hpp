// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_CIPHER
#define DATA_CRYPTO_CIPHER

#include <data/array.hpp>
/*
 * ===============================================================================
 * Cipher Architecture Overview
 * ===============================================================================
 *
 * This file defines the core abstractions used for symmetric encryption and
 * decryption in this library. The design is intentionally layered and protocol-
 * oriented rather than stream-oriented, reflecting the fact that real-world
 * encryption is a stateful process with well-defined message boundaries.
 *
 * At a high level, the library provides:
 *
 * Algorithm types describing concrete ciphers and modes
 * One-shot encryption/decryption functions for simple use cases
 * Stateful reader/writer types for incremental, protocol-level processing
 *
 * The same algorithm types are used consistently across all interfaces.
 *
 * -------------------------------------------------------------------------------
 * 1. Algorithm Types
 * -------------------------------------------------------------------------------
 *
 * An *algorithm* is a type that fully describes how encryption or decryption
 * is performed. Algorithm types contain no secret material and may be freely
 * copied. They typically encode:
 *
 * The underlying cipher
 * The mode of operation (if any)
 * Parameters such as endianness or padding behavior
 * Any required initialization vector (IV) format
 *
 * Algorithm types are passed by const reference to encryption and decryption
 * functions, or embedded into reader/writer session types.
 *
 * -------------------------------------------------------------------------------
 * 2. One-Shot Encryption and Decryption
 * -------------------------------------------------------------------------------
 *
 * For simple use cases, the library provides functional interfaces:
 *
 *    bytes encrypt (
 *        const algorithm &,
 *        const symmetric_key<key_size> &,
 *        bytes plaintext
 *    );
 *
 *    bytes decrypt (
 *        const algorithm &,
 *        const symmetric_key<key_size> &,
 *        bytes ciphertext
 *    );
 *
 * These functions operate on whole messages at once. They are appropriate when:
 *
 * The entire message is already available in memory
 * Message boundaries are known in advance
 * Padding and authentication can be validated immediately
 *
 * Internally, these functions are implemented in terms of the same session
 * machinery used by the streaming interfaces.
 *
 * -------------------------------------------------------------------------------
 * 3. Direction
 * -------------------------------------------------------------------------------
 *
 *    enum class direction { encryption, decryption };
 *
 * The direction parameter is used to select whether data is being transformed
 * from plaintext to ciphertext or vice versa. Direction is encoded at the type
 * level to prevent accidental misuse (e.g., decrypting with an encryptor).
 *
 * -------------------------------------------------------------------------------
 * 4. Stateful Cipher Sessions (reader / writer)
 * -------------------------------------------------------------------------------
 *
 * For protocol-level use, the library provides incremental cipher interfaces:
 *
 *    template <direction D, typename algorithm, size_t key_size>
 *    struct writer;
 *
 *    template <direction D, typename algorithm, size_t key_size>
 *    struct reader;
 *
 * These types model *message-scoped cipher sessions*.
 *
 * A writer consumes plaintext or ciphertext via operator << and emits the
 *    transformed data to an underlying sink.
 * A reader pulls transformed data via operator >> from an underlying source.
 *
 * Destructors may attempt implicit completion when it is safe to do so
 * (i.e. when no exception is already propagating), allowing RAII-style usage
 * for simple cases while still supporting explicit completion in protocols.
 *
 * -------------------------------------------------------------------------------
 * 5. Stream Ciphers
 * -------------------------------------------------------------------------------
 *
 * Stream ciphers are defined in:
 *
 *    namespace data::crypto::cipher::stream
 *
 * Available stream cipher algorithms:
 *
 * XChaCha20
 * XSalsa20
 * Salsa20
 * HC128
 * HC256
 * Panama
 *
 * Each stream cipher algorithm is constructed with an initialization vector:
 *
 *    initialization_vector<size>
 *
 * Stream ciphers are inherently streamable:
 *
 * Encryption and decryption are symmetric
 * No padding is required
 * Data may be processed byte-by-byte
 *
 * -------------------------------------------------------------------------------
 * 6. Block Ciphers
 * -------------------------------------------------------------------------------
 *
 * Block ciphers are described using:
 *
 *    block_cipher<cipher, block::mode, auto... parameters>
 *
 * Available block ciphers:
 *
 * Rijndael / AES
 * Serpent
 * Twofish
 * RC6
 * MARS
 * DES
 *
 * Block ciphers operate on fixed-size blocks and must be combined with a mode
 * of operation to encrypt messages longer than a single block.
 *
 * -------------------------------------------------------------------------------
 * 7. Block Modes
 * -------------------------------------------------------------------------------
 *
 * Supported block modes:
 *
 * ECB
 * CBC
 * CFB
 * OFB
 * CTR
 *
 * CTR mode requires an additional parameter specifying counter endianness:
 *
 *    endian::big
 *    endian::little
 *
 * -------------------------------------------------------------------------------
 * 8. Initialization Vectors
 * -------------------------------------------------------------------------------
 *
 * The type of the initialization vector depends on the mode:
 *
 * CTR mode:
 *        math::uint<endian::{big|little}, cipher::BlockSize, byte>
 *
 * All other block modes:
 *        initialization_vector<cipher::BlockSize>
 *
 * The IV represents both the initial input to the mode and its evolving internal
 * state across block processing.
 *
 * -------------------------------------------------------------------------------
 * 9. Padding
 * -------------------------------------------------------------------------------
 *
 * Block ciphers may require padding depending on the mode:
 *
 * Streamable modes (CFB, OFB, CTR):
 *        No padding by default
 *
 * Non-streamable modes (ECB, CBC):
 *        Default padding is "one-and-zeros" padding
 *
 * A custom padding scheme may be supplied explicitly when constructing the
 * block_cipher algorithm type.
 *
 * Padding is validated during session completion or decryption finalization.
 *
 * -------------------------------------------------------------------------------
 * 10. Design Philosophy
 * -------------------------------------------------------------------------------
 *
 * This library intentionally distinguishes between:
 *
 * Stateless algorithm descriptions
 * Stateful encryption/decryption sessions
 * One-shot convenience interfaces
 *
 * Encryption is treated as a protocol with structure, not merely as a byte
 * transform. This allows correct handling of padding, authentication, IVs,
 * and message framing in realistic applications.
 *
 * ===============================================================================
 */

namespace data::crypto::cipher {

    enum direction {
        encryption,
        decryption
    };

    template <direction dir, typename algorithm, auto ...> struct reader;
    template <direction dir, typename algorithm, auto ...> struct writer;

    template <size_t key_size> using symmetric_key = byte_array<key_size>;

    template <size_t block_size>
    using initialization_vector = byte_array<block_size>;

    std::ostream inline &operator << (std::ostream &o, direction d) {
        return d == encryption ? (o << "encryption") : (o << "decryption");
    }

}

#endif

