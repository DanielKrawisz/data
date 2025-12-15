// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sv/crypto/hmac_sha512.h>

#include <cstring>

CHMAC_SHA512::CHMAC_SHA512 (const data::byte *key, size_t keylen) {
    data::byte rkey[128];
    if (keylen <= 128) {
        memcpy (rkey, key, keylen);
        memset (rkey + keylen, 0, 128 - keylen);
    } else {
        CSHA512 ().Update (key, keylen).Final (rkey);
        memset (rkey + 64, 0, 64);
    }

    for (int n = 0; n < 128; n++)
        rkey[n] ^= 0x5c;
    outer.Update (rkey, 128);

    for (int n = 0; n < 128; n++)
        rkey[n] ^= 0x5c ^ 0x36;
    inner.Update (rkey, 128);
}

void CHMAC_SHA512::Final (uint8_t hash[Size]) {
    uint8_t temp[64];
    inner.Final (temp);
    outer.Update (temp, 64).Final (hash);
}
