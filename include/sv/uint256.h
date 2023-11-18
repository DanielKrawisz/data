// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UINT256_H
#define BITCOIN_UINT256_H

#include "crypto/common.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "boost/functional/hash.hpp"

namespace Satoshi {

    /** Template base class for fixed-sized opaque blobs. */
    template <unsigned int BITS> class base_blob {
    protected:
        enum { WIDTH = BITS / 8 };
        uint8_t data[WIDTH];

    public:
        base_blob () { memset (data, 0, sizeof (data)); }

        explicit base_blob (const std::vector<uint8_t> &vch) {
            assert (vch.size () == sizeof (data));
            memcpy (data, &vch[0], sizeof (data));
        }

        bool IsNull () const {
            for (int i = 0; i < WIDTH; i++)
                if (data[i] != 0) return false;
            return true;
        }

        void SetNull () { memset (data, 0, sizeof (data)); }

        inline int Compare (const base_blob &other) const {
            return memcmp (data, other.data, sizeof (data));
        }

        friend inline bool operator == (const base_blob &a, const base_blob &b) {
            return a.Compare (b) == 0;
        }
        friend inline bool operator != (const base_blob &a, const base_blob &b) {
            return a.Compare (b) != 0;
        }
        friend inline bool operator < (const base_blob &a, const base_blob &b) {
            return a.Compare (b) < 0;
        }

        uint8_t *begin () { return &data[0]; }

        uint8_t *end () { return &data[WIDTH]; }

        const uint8_t *begin () const { return &data[0]; }

        const uint8_t *end () const { return &data[WIDTH]; }

        unsigned int size () const { return sizeof (data); }

        uint64_t GetUint64 (int pos) const {
            const uint8_t *ptr = data + pos * 8;
            return ((uint64_t) ptr[0]) | ((uint64_t) ptr[1]) << 8 |
                ((uint64_t) ptr[2]) << 16 | ((uint64_t) ptr[3]) << 24 |
                ((uint64_t) ptr[4]) << 32 | ((uint64_t) ptr[5]) << 40 |
                ((uint64_t) ptr[6]) << 48 | ((uint64_t) ptr[7]) << 56;
        }

        template <typename Stream> void Serialize (Stream &s) const {
            s.write ((char *) data, sizeof (data));
        }

        template <typename Stream> void Unserialize (Stream &s) {
            s.read ((char *) data, sizeof (data));
        }
    };

    /**
    * 160-bit opaque blob.
    * @note This type is called uint160 for historical reasons only. It is an
    * opaque blob of 160 bits and has no integer operations.
    */
    class uint160 : public base_blob<160> {
    public:
        uint160 () {}
        uint160 (const base_blob<160> &b) : base_blob<160> (b) {}
        explicit uint160 (const std::vector<uint8_t> &vch) : base_blob<160> (vch) {}
    };

    /**
    * 256-bit opaque blob.
    * @note This type is called uint256 for historical reasons only. It is an
    * opaque blob of 256 bits and has no integer operations. Use arith_uint256 if
    * those are required.
    */
    class uint256 : public base_blob<256> {
    public:
        uint256 () {}
        uint256 (const base_blob<256> &b) : base_blob<256> (b) {}
        explicit uint256 (const std::vector<uint8_t> &vch) : base_blob<256> (vch) {}

        /**
        * A cheap hash function that just returns 64 bits from the result, it can
        * be used when the contents are considered uniformly random. It is not
        * appropriate when the value can easily be influenced from outside as e.g.
        * a network adversary could provide values to trigger worst-case behavior.
        */
        uint64_t GetCheapHash () const { return ReadLE64 (data); }
    };

    inline std::size_t hash_value (const uint256& i) {
        return boost::hash_range (i.begin (), i.end ());
    }

}

/**
 * Specialise std::hash for uint256.
 */
namespace std {
    template<>
    class hash<Satoshi::uint256> {
      public:
        size_t operator () (const Satoshi::uint256& u) const {
            return static_cast<size_t> (u.GetCheapHash ());
        }
    };
}

#endif // BITCOIN_UINT256_H
