// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_COMMON_H
#define BITCOIN_CRYPTO_COMMON_H

#include <cstdint>
#include <cstring>

#include <boost/endian/conversion.hpp>

#define ReadLE16(p) (boost::endian::endian_load<uint16_t, 2, boost::endian::order::little> (p))
#define ReadLE32(p) (boost::endian::endian_load<uint32_t, 4, boost::endian::order::little> (p))
#define ReadLE64(p) (boost::endian::endian_load<uint64_t, 8, boost::endian::order::little> (p))
#define ReadBE32(p) (boost::endian::endian_load<uint32_t, 4, boost::endian::order::big> (p))
#define ReadBE64(p) (boost::endian::endian_load<uint64_t, 8, boost::endian::order::big> (p))

#define WriteLE16(p, x) (boost::endian::endian_store<uint16_t, 2, boost::endian::order::little> (p, x))
#define WriteLE32(p, x) (boost::endian::endian_store<uint32_t, 4, boost::endian::order::little> (p, x))
#define WriteLE64(p, x) (boost::endian::endian_store<uint64_t, 8, boost::endian::order::little> (p, x))
#define WriteBE32(p, x) (boost::endian::endian_store<uint32_t, 4, boost::endian::order::big> (p, x))
#define WriteBE64(p, x) (boost::endian::endian_store<uint64_t, 8, boost::endian::order::big> (p, x))

/**
 * Return the smallest number n such that (x >> n) == 0 (or 64 if the highest
 * bit in x is set.
 */
uint64_t static inline CountBits (uint64_t x) {
#ifdef HAVE_DECL___BUILTIN_CLZL
    if (sizeof (unsigned long) >= sizeof (uint64_t)) {
        return x ? 8 * sizeof (unsigned long) - __builtin_clzl (x) : 0;
    }
#endif
#ifdef HAVE_DECL___BUILTIN_CLZLL
    if (sizeof (unsigned long long) >= sizeof (uint64_t)) {
        return x ? 8 * sizeof (unsigned long long) - __builtin_clzll (x) : 0;
    }
#endif
    int ret = 0;
    while (x) {
        x >>= 1;
        ++ret;
    }
    return ret;
}

#endif // BITCOIN_CRYPTO_COMMON_H
