#pragma once

#include <functional>
#include <type_traits>
#include <utility>

#include <NTL/ZZ.h>
#include <data/random.hpp>

namespace NTL {

    // A deterministic random source.  The same seed produces the same
    // sequence of bytes.
    struct generator : data::random::generator {
        constexpr static const size_t MinEntropyLength = NTL_PRG_KEYLEN;

        explicit generator (data::byte_slice seed);

        void read (data::byte *data, size_t size) override;
        void reseed (data::byte_slice seed) override;

        RandomStream Stream;
    private:
        static RandomStream make_stream (data::byte_slice seed);
    };

    using source = data::random::automatic_reseed<generator>;

    // detect if this is our NTL RandomSource wrapper type. If not, generate a new
    // random source seeded from the given source.
    template <typename F, typename... Args>
    decltype (auto) random_function (data::random::source &source, F &&function, Args &&...args) {
        struct call {
            decltype (auto) operator () (RandomStream &stream, F &&function, Args &&...args) {
                RandomStreamPush push;

                SetSeed (stream);

                return std::invoke (
                    std::forward<F> (function),
                    std::forward<Args> (args)...
                );
            }
        };

        if (auto *stream = dynamic_cast<NTL::source *> (&source))
            source = stream->Source;

        if (auto *stream = dynamic_cast<NTL::generator *> (&source))
            return call {} (stream->Stream,
                std::forward<F> (function),
                std::forward<Args> (args)...);

        unsigned char key[NTL_PRG_KEYLEN];

        source.read (
            reinterpret_cast<data::byte *> (key),
            sizeof key
        );

        RandomStream temporary (key);

        return call {} (
            temporary,
            std::forward<F> (function),
            std::forward<Args> (args)...
        );
    }

    inline generator::generator (data::byte_slice seed) : Stream (make_stream (seed)) { }

    void inline generator::reseed (data::byte_slice seed) { Stream = make_stream (seed); }

}
