#include <data/concepts.hpp>
#include <data/random.hpp>
#include <openssl/rand.h>

namespace OpenSSL {

    struct secure_source : random::source {

        static secure_source &instance () {
            static secure_source source;
            return source;
        }

        void read (byte *b, size_t z) override {
            while (z > 0) {
                auto n = std::min (z, size_t (INT_MAX));

                if (RAND_priv_bytes (
                    b, static_cast<int> (n)
                ) != 1)
                    throw std::runtime_error {
                        "OpenSSL RAND_priv_bytes failed"
                    };

                    b += n;
                    z -= n;
            }
        }

    private:
        secure_source () = default;
    };

    // Mix additional random data from data::random::source into OpenSSL.
    inline void seed (random::source &source, size_t size = 32) {
        std::vector<byte> bytes (size);
        source.read (bytes.data (), bytes.size ());

        RAND_add (bytes.data (), static_cast<int> (bytes.size ()), 0);
    }

}
