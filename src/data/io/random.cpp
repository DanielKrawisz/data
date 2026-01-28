#include <data/io/random.hpp>
#include <data/crypto/block/cryptopp.hpp>
#include <data/crypto/NIST_DRBG.hpp>

namespace data::random {

    // statistical randomness, fast
    ptr<source> Casual {};

    // cryptographic randomness, slow
    ptr<source> Secure {};

}

namespace data::random {

    source &get () {
        return *Casual;
    }

}

namespace data::crypto::random {

    source &get () {
        return *Secure;
    }

}

namespace data::random {

    // our source of strong entropy
    ptr<source> StrongEntropy {};

    ptr<source> WeakEntropy {};

    source *SecureEntropy {};
    source *CasualEntropy {};

    void init (options o) {

        if (o.secure && o.strength != 112 && o.strength != 128 && o.strength != 192 && o.strength != 256)
            throw exception {} << "invalid value for secure RNG strength (" << o.strength << "). Allowed values are 112, 128, 192, and 256";

        if (bool (o.seed)) {
            WeakEntropy = std::static_pointer_cast<source> (
                std::make_shared<fixed_entropy> (
                    crypto::NIST::Hash_df<crypto::hash::SHA2_512, uint32_big {1016}> (*o.seed)));

            CasualEntropy = WeakEntropy.get ();

        } else {
            WeakEntropy = std::static_pointer_cast<source> (
                std::make_shared<crypto::random::OS_entropy> ());

            CasualEntropy = WeakEntropy.get ();
        }

        Casual = ptr<crypto::random::default_casual_random> (
            new crypto::random::default_casual_random {*CasualEntropy, 1 << 30});

        if (!o.secure) return;

        if (bool (o.seed)) {
            SecureEntropy = WeakEntropy.get ();

        } else {
            StrongEntropy = std::static_pointer_cast<source> (
                std::make_shared<crypto::random::OS_entropy_strong> ());

            SecureEntropy = StrongEntropy.get ();
        }

        bytes nonce;

        if (o.nonce == bytes {}) {
            nonce.resize (o.strength / 2);
            *WeakEntropy >> nonce;
        } else nonce = o.nonce;

        if (o.strength == 112) {
            using nist = crypto::NIST::CTR_DRBG<21, crypto::cipher::block::TDEA3>;
            if (o.additional) {
                using secure = automatic_reseed<crypto::NIST::auto_generate_with_additional_entropy<nist>>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30, *o.additional,
                            byte_slice (nonce), byte_slice (Personalization)}));
            } else {
                using secure = automatic_reseed<nist>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30,
                            data::byte_slice (nonce), data::byte_slice (Personalization)}));
            }
        } else if (o.strength == 128) {
            using nist = crypto::NIST::Hash_DRBG<crypto::hash::SHA2_256>;
            if (o.additional) {
                using secure = automatic_reseed<crypto::NIST::auto_generate_with_additional_entropy<nist>>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30, *o.additional,
                            byte_slice (nonce), byte_slice (Personalization)}));
            } else {
                using secure = automatic_reseed<nist>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30,
                            data::byte_slice (nonce), data::byte_slice (Personalization)}));
            }
        } else if (o.strength == 192) {
            using nist = crypto::NIST::HMAC_DRBG<crypto::hash::SHA2_384>;
            if (o.additional) {
                using secure = automatic_reseed<crypto::NIST::auto_generate_with_additional_entropy<nist>>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30, *o.additional,
                            byte_slice (nonce), byte_slice (Personalization)}));
            } else {
                using secure = automatic_reseed<nist>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30,
                            data::byte_slice (nonce), data::byte_slice (Personalization)}));
            }
        } else if (o.strength == 256) {
            using nist = crypto::NIST::Hash_DRBG<crypto::hash::SHA2_512>;
            if (o.additional) {
                using secure = automatic_reseed<crypto::NIST::auto_generate_with_additional_entropy<nist>>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30, *o.additional,
                            byte_slice (nonce), byte_slice (Personalization)}));
            } else {
                using secure = automatic_reseed<nist>;
                Secure = std::static_pointer_cast<source> (
                    std::shared_ptr<secure> (
                        new secure {*SecureEntropy, 1 << 30,
                            data::byte_slice (nonce), data::byte_slice (Personalization)}));
            }
        }
    }
}
