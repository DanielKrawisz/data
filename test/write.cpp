// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/bytes.hpp"
#include "data/array.hpp"
#include "data/numbers.hpp"
#include "data/crypto/hash.hpp"
#include "data/crypto/MAC.hpp"
#include "data/crypto/MAC/HMAC.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (Write, Bytes) {

        // write a list of bytes
        EXPECT_EQ (write<bytes> (), bytes {});
        std::cout << " error case: " << std::endl;
        EXPECT_EQ ((write<bytes> (byte (0x76))), bytes {0x76});
        EXPECT_EQ ((write<bytes> (byte (0x76), byte (0x01))), (bytes {0x76, 0x01}));
        EXPECT_EQ ((write<bytes> (byte (0x76), uint32_little (12))), (bytes {0x76, 0x0c, 0x00, 0x00, 0x00}));

        // write to bytes with function
        EXPECT_EQ (write<bytes> ([](auto &&w) {}), bytes {});
        EXPECT_EQ ((write<bytes> ([](auto &&w) { w << byte (0x76); })), bytes {0x76});
        EXPECT_EQ ((write<bytes> ([](auto &&w) { w << byte (0x76) << byte (0x01); })), (bytes {0x76, 0x01}));
        EXPECT_EQ ((write<bytes> ([](auto &&w) { w << byte (0x76) << uint32_little (12); })), (bytes {0x76, 0x0c, 0x00, 0x00, 0x00}));

        // write a size followed by a list of bytes
        EXPECT_EQ (write<bytes> (0), bytes {});
        EXPECT_EQ ((write<bytes> (1, byte (0x76))), bytes {0x76});
        EXPECT_EQ ((write<bytes> (2, byte (0x76), byte (0x01))), (bytes {0x76, 0x01}));
        EXPECT_EQ ((write<bytes> (5, byte (0x76), uint32_little (12))), (bytes {0x76, 0x0c, 0x00, 0x00, 0x00}));

        // test that an exception gets thrown if we write too much or too little
        EXPECT_THROW ((write<bytes> (0, byte (0x76))), exception);
        EXPECT_THROW ((write<bytes> (2, byte (0x76))), exception);

        // check the unusual case of a function that is also serializable.

    }

    namespace crypto::hash {

        // we don't have to test that the hash is correct, just that the interface works.
        TEST (Write, Hash) {

            // write a list of bytes
            EXPECT_NO_THROW (write<MD5> ());
            EXPECT_NO_THROW ((write<MD5> (byte (0x76))));
            EXPECT_NO_THROW ((write<MD5> (byte (0x76), byte (0x01))));
            EXPECT_NO_THROW ((write<MD5> (byte (0x76), uint32_little (12))));

            // write to bytes with function
            EXPECT_NO_THROW (write<MD5> ([](auto &&w) {}));
            EXPECT_NO_THROW ((write<MD5> ([](auto &&w) { w << byte (0x76); })));
            EXPECT_NO_THROW ((write<MD5> ([](auto &&w) { w << byte (0x76) << byte (0x01); })));
            EXPECT_NO_THROW ((write<MD5> ([](auto &&w) { w << byte (0x76) << uint32_little (12); })));

        }
    }

    namespace crypto::MAC {

        TEST (Write, HMAC) {

            const symmetric_key<5> key {encoding::hex::fixed<5> {"abcdef0102"}};

            // write a list of bytes
            EXPECT_NO_THROW (write<HMAC<hash::MD5>> (key));
            EXPECT_NO_THROW ((write<HMAC<hash::MD5>> (key, byte (0x76))));
            EXPECT_NO_THROW ((write<HMAC<hash::MD5>> (key, byte (0x76), byte (0x01))));
            EXPECT_NO_THROW ((write<HMAC<hash::MD5>> (key, byte (0x76), uint32_little (12))));

            // write to bytes with function
            EXPECT_NO_THROW (write<writer<HMAC<hash::MD5>>> (key, [](auto &&w) {}));
            EXPECT_NO_THROW ((write<writer<HMAC<hash::MD5>>> (key, [](auto &&w) { w << byte (0x76); })));
            EXPECT_NO_THROW ((write<writer<HMAC<hash::MD5>>> (key, [](auto &&w) { w << byte (0x76) << byte (0x01); })));
            EXPECT_NO_THROW ((write<writer<HMAC<hash::MD5>>> (key, [](auto &&w) { w << byte (0x76) << uint32_little (12); })));

        }
    }
/*
    TEST (Write, Array) {

        EXPECT_EQ (write<byte_array<0>> (), byte_array<0> {});
        EXPECT_EQ ((write<byte_array<1>> (byte (0x76))), byte_array<1> {0x76});
        EXPECT_EQ ((write<byte_array<2>> (byte (0x76), byte (0x01))), (byte_array<2> {0x76, 0x01}));
        EXPECT_EQ ((write<byte_array<5>> (byte (0x76), uint32_little (12))), (byte_array<5> {0x76, 0x0c, 0x00, 0x00, 0x00}));

        // write to bytes with function
        EXPECT_EQ (write<byte_array<0>> ([](auto &&w) {}), bytes {});
        EXPECT_EQ ((write<byte_array<1>> ([](auto &&w) { w << byte (0x76); })), bytes {0x76});
        EXPECT_EQ ((write<byte_array<2>> ([](auto &&w) { w << byte (0x76) << byte (0x01); })), (bytes {0x76, 0x01}));
        EXPECT_EQ ((write<byte_array<5>> ([](auto &&w) { w << byte (0x76) << uint32_little (12); })), (bytes {0x76, 0x0c, 0x00, 0x00, 0x00}));

        // test that an exception gets thrown if we write too much or too little
        EXPECT_THROW ((write<byte_array<0>> (byte (0x76))), exception);
        EXPECT_THROW ((write<byte_array<2>> (byte (0x76))), exception);

    }

    TEST (Write, NBytes) {

        // write a list of bytes
        EXPECT_EQ (write<N_bytes_big> (), N_bytes_big {});
        EXPECT_EQ ((write<N_bytes_big> (byte (0x76))), N_bytes_big {0x76});
        EXPECT_EQ ((write<N_bytes_big> (byte (0x76), byte (0x01))), (N_bytes_big {0x7601}));
        EXPECT_EQ ((write<N_bytes_big> (byte (0x76), uint32_little (12))), (N_bytes_big {0x760c000000}));

        // write to bytes with function
        EXPECT_EQ (write<N_bytes_big> ([](auto &&w) {}), N_bytes_big {});
        EXPECT_EQ ((write<N_bytes_big> ([](auto &&w) { w << byte (0x76); })), N_bytes_big {0x76});
        EXPECT_EQ ((write<N_bytes_big> ([](auto &&w) { w << byte (0x76) << byte (0x01); })), (N_bytes_big {0x7601}));
        EXPECT_EQ ((write<N_bytes_big> ([](auto &&w) { w << byte (0x76) << uint32_little (12); })), (N_bytes_big {0x760c000000}));

        // write a size followed by a list of bytes
        EXPECT_EQ (write<N_bytes_big> (0), N_bytes_big {});
        EXPECT_EQ ((write<N_bytes_big> (1, byte (0x76))), N_bytes_big {0x76});
        EXPECT_EQ ((write<N_bytes_big> (2, byte (0x76), byte (0x01))), (N_bytes_big {0x7601}));
        EXPECT_EQ ((write<N_bytes_big> (5, byte (0x76), uint32_little (12))), (N_bytes_big {0x760c000000}));

        // test that an exception gets thrown if we write too much or too little
        EXPECT_THROW ((write<N_bytes_big> (0, byte (0x76))), exception);
        EXPECT_THROW ((write<N_bytes_big> (2, byte (0x76))), exception);
    }

    TEST (Write, UInt) {

        EXPECT_EQ (write<uint_big<0>> (), uint_big<0> {});
        EXPECT_EQ ((write<uint_big<1>> (byte (0x76))), uint_big<1> {"76"});
        EXPECT_EQ ((write<uint_big<2>> (byte (0x76), byte (0x01))), (uint_big<2> {"7601"}));
        EXPECT_EQ ((write<uint_big<5>> (byte (0x76), uint32_little (12))), (uint_big<5> {"760c000000"}));

        // write to bytes with function
        EXPECT_EQ (write<uint_big<0>> ([](auto &&w) {}), bytes {});
        EXPECT_EQ ((write<uint_big<1>> ([](auto &&w) { w << byte (0x76); })), bytes {"76"});
        EXPECT_EQ ((write<uint_big<2>> ([](auto &&w) { w << byte (0x76) << byte (0x01); })), (bytes {"7601"}));
        EXPECT_EQ ((write<uint_big<5>> ([](auto &&w) { w << byte (0x76) << uint32_little (12); })), (bytes {76, 0x0c, 0x00, 0x00, 0x00}));

        // test that an exception gets thrown if we write too much or too little
        EXPECT_THROW ((write<uint_big<0>> (byte (0x76))), exception);
        EXPECT_THROW ((write<uint_big<2>> (byte (0x76))), exception);

    }*/

}


