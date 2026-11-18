#include <openssl/Z.hpp>
#include <memory>

namespace OpenSSL {

    Z::Z () : Value {BN_new ()} {
        if (!Value)
            throw std::bad_alloc {};
    }

    Z::~Z () {
        BN_clear_free (Value);
    }

    Z::Z (const Z &z) : Value {z.Value ? BN_dup (z.Value) : nullptr} {
        if (!Value) {
            if (!z.Value)
                throw std::logic_error {"copying moved-from OpenSSL::Z"};

            throw std::bad_alloc {};
        }
    }

    Z::Z (Z &&z) noexcept : Value{z.Value} {
        z.Value = nullptr;
    }

    Z &Z::operator = (const Z &z) {
        if (this == &z)
            return *this;

        if (!z.Value)
            throw std::logic_error {"copying moved-from OpenSSL::Z"};

        ZZ copy = BN_dup(z.Value);

        if (!copy)
            throw std::bad_alloc {};

        BN_clear_free (Value);
        Value = copy;

        return *this;
    }

    Z &Z::operator = (Z &&z) noexcept {
        if (this == &z)
            return *this;

        BN_clear_free (Value);

        Value = z.Value;
        z.Value = nullptr;

        return *this;
    }

    Z::Z (ZZ z) : Value{z} {
        if (!Value)
            throw std::bad_alloc{};
    }

    ZZ import_dec (const dec_int &x) {
        BIGNUM *raw = nullptr;

        if (x.size () > static_cast<std::size_t> (std::numeric_limits<int>::max ()))
            throw std::runtime_error {"decimal integer too large"};

        std::unique_ptr<BIGNUM, decltype (&BN_free)> z {nullptr, &BN_free};

        int n = BN_dec2bn (&raw, x.c_str ());
        z.reset (raw);

        if (n != static_cast<int> (x.size ()))
            throw std::runtime_error {"invalid decimal integer"};

        return z.release ();
    }

    struct openssl_string_deleter {
        void operator () (char *p) const {
            OPENSSL_free (p);
        }
    };

    dec_int export_dec (const ZZ x) {
        std::unique_ptr<char, openssl_string_deleter> s {BN_bn2dec (x)};

        if (!s)
            throw std::runtime_error {"failed to export decimal integer"};

        return dec_int {s.get()};
    }

    Z operator + (const Z &a, const Z &b) {
        Z r {BN_new ()};

        if (!r.Value || !BN_add (r.Value, a.Value, b.Value))
            throw std::runtime_error {"BN_add failed"};

        return r;
    }

    Z operator - (const Z &a, const Z &b) {
        Z r {BN_new ()};

        if (!r.Value || !BN_sub (r.Value, a.Value, b.Value))
            throw std::runtime_error {"BN_sub failed"};

        return r;
    }

    Z operator - (const Z &a) {
        Z r {BN_dup (a.Value)};

        if (!r.Value)
            throw std::runtime_error {"BN_dup failed"};

        BN_set_negative (r.Value, !BN_is_negative (r.Value));

        return r;
    }

    Z operator * (const Z &a, const Z &b) {
        Z r {BN_new ()};
        BN_CTX *ctx = BN_CTX_new ();

        if (!r.Value || !ctx || !BN_mul (r.Value, a.Value, b.Value, ctx)) {
            BN_free (r.Value);
            BN_CTX_free (ctx);
            throw std::runtime_error {"BN_mul failed"};
        }

        BN_CTX_free (ctx);
        return r;
    }

    std::strong_ordering operator <=> (const Z &a, const Z &b) {
        int c = BN_cmp (a.Value, b.Value);

        if (c < 0) return std::strong_ordering::less;
        if (c > 0) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    Z pow_mod (const Z &base, const Z &exponent, const Z &modulus) {

        if (BN_is_zero (modulus.Value) || BN_is_negative (modulus.Value))
            throw data::exception {} << "invalid modulus";

        std::unique_ptr<BN_CTX, decltype (&BN_CTX_free)>
        ctx (BN_CTX_new (), BN_CTX_free);

        if (!ctx)
            throw data::exception {} << "cannot allocate OpenSSL BN_CTX";

        Z result;

        if (!BN_mod_exp (
            result.Value,
            base.Value,
            exponent.Value,
            modulus.Value,
            ctx.get ()
        )) throw data::exception {} << "OpenSSL modular exponentiation failed";

        return result;
    }

}

namespace data::math::def {

    OpenSSL::Z abs<OpenSSL::Z>::operator () (const OpenSSL::Z &x) {
        OpenSSL::ZZ z = BN_dup (x.Value);

        if (!z) throw std::runtime_error {"BN_dup failed"};

        BN_set_negative (z, 0);

        return OpenSSL::Z {z};
    }

    size_t size_in_base<OpenSSL::Z, 2>::operator () (const OpenSSL::Z &x) {
        if (!BN_is_negative (x.Value))
            return BN_num_bits (x.Value) + 1;

        OpenSSL::Z z = data::abs (x);

        if (!BN_sub_word (z.Value, 1)) {
            throw std::runtime_error {"BN_sub_word failed"};
        }

        unsigned int result = BN_num_bits (z.Value) + 1;

        return result;
    }

    division<OpenSSL::Z, OpenSSL::Z> divmod<OpenSSL::Z, OpenSSL::Z>::operator () (
        const OpenSSL::Z &a,
        const nonzero<OpenSSL::Z> &b
    ) {
        OpenSSL::Z q {BN_new ()};
        OpenSSL::Z r {BN_new ()};
        BN_CTX *ctx = BN_CTX_new ();

        if (!q.Value || !r.Value || !ctx) {
            BN_CTX_free (ctx);
            throw std::bad_alloc {};
        }

        if (!BN_div (q.Value, r.Value, a.Value, b.Value.Value, ctx)) {
            BN_CTX_free (ctx);
            throw std::runtime_error {"BN_div failed"};
        }

        if (BN_is_negative (r.Value)) {
            OpenSSL::Z abs_b {BN_dup (b.Value.Value)};

            if (!abs_b.Value) {
                BN_CTX_free (ctx);
                throw std::bad_alloc {};
            }

            BN_set_negative (abs_b.Value, 0);

            if (!BN_add (r.Value, r.Value, abs_b.Value)) {
                BN_CTX_free (ctx);
                throw std::runtime_error {"BN_add failed"};
            }

            if (BN_is_negative (b.Value.Value)) {
                if (!BN_add_word (q.Value, 1)) {
                    BN_CTX_free (ctx);
                    throw std::runtime_error {"BN_add_word failed"};
                }
            } else {
                if (!BN_sub_word (q.Value, 1)) {
                    BN_CTX_free (ctx);
                    throw std::runtime_error {"BN_sub_word failed"};
                }
            }
        }

        BN_CTX_free (ctx);
        return {std::move (q), std::move (r)};
    }

    OpenSSL::Z pow<OpenSSL::Z, OpenSSL::Z>::operator () (
        const OpenSSL::Z &x,
        const OpenSSL::Z &y
    ) {
        if (BN_is_negative (y.Value))
            throw std::domain_error {"negative exponent"};

        BN_CTX *ctx = BN_CTX_new ();

        if (!ctx)
            throw std::runtime_error {"BN_CTX_new failed"};

        OpenSSL::Z result;

        if (!BN_exp (result.Value, x.Value, y.Value, ctx)) {
            BN_CTX_free (ctx);
            throw std::runtime_error {"BN_exp failed"};
        }

        BN_CTX_free (ctx);
        return result;
    }

    OpenSSL::Z pow_mod<OpenSSL::Z, OpenSSL::Z, OpenSSL::Z>::operator () (
        const OpenSSL::Z &x,
        const OpenSSL::Z &y,
        const nonzero<OpenSSL::Z> &m
    ) {
        return OpenSSL::pow_mod (x, y, m.Value);
    }

}


