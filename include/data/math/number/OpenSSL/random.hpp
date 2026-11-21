
#pragma once

#include <OpenSSL/Z.hpp>

#include <data/math/number/prime.hpp>

#include <openssl/bn.h>
#include <climits>
#include <memory>

namespace data::math::number {

    template <>
    prime<OpenSSL::Z> is_prime<OpenSSL::Z> (
        random::source &source,
        const OpenSSL::Z &n,
        unsigned rounds
    ) {
        if (n.Value == nullptr)
            throw exception {} << "null OpenSSL integer";

        if (BN_is_negative (n.Value) || BN_is_zero (n.Value) ||
            BN_is_one (n.Value))
            return false;

        if (rounds > INT_MAX)
            throw exception {} << "too many Miller-Rabin rounds";

        if (BN_is_word (n.Value, 2) || BN_is_word (n.Value, 3))
            return true;

        if (!BN_is_odd (n.Value))
            return false;

        add_randomness (source);

        std::unique_ptr<BN_CTX, decltype (&BN_CTX_free)>
            ctx {BN_CTX_new (), BN_CTX_free};

        if (!ctx)
            throw exception {} << "cannot allocate OpenSSL BN_CTX";

        int result = BN_is_prime_fasttest_ex (
            n.Value,
            static_cast<int> (rounds),
            ctx.get (),
            1,
            nullptr
        );

        if (result < 0)
            throw exception {} << "OpenSSL primality test failed";

        return result == 1;
    }

    template <>
    prime<OpenSSL::Z> generate_random_prime<OpenSSL::Z> (
        random::source &source,
        uint32 digits,
        generate_prime_parameters parameters
    ) {
        if (digits < 2 || digits > INT_MAX)
            throw exception {} << "invalid prime bit length: " << digits;

        if (parameters.rounds > INT_MAX)
            throw exception {} << "too many Miller-Rabin rounds";

        if (parameters.safe && digits < 6 && digits != 3)
            throw exception {} << "invalid safe prime bit length: " << digits;

        add_randomness (source);

        std::unique_ptr<BN_CTX, decltype (&BN_CTX_free)>
            ctx {BN_CTX_new (), BN_CTX_free};

        if (!ctx)
            throw exception {} << "cannot allocate OpenSSL BN_CTX";

        std::unique_ptr<BIGNUM, decltype (&BN_clear_free)>
            p {BN_new (), BN_clear_free};

        if (!p)
            throw exception {} << "cannot allocate OpenSSL integer";

        if (BN_generate_prime_ex2 (
                p.get (),
                static_cast<int> (digits),
                parameters.safe,
                nullptr,
                nullptr,
                nullptr,
                ctx.get ()
            ) != 1)
            throw exception {} << "OpenSSL prime generation failed";

        return OpenSSL::Z {p.release ()};
    }
}

