// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_RANDOM
#define DATA_RANDOM

#include <cryptopp/rng.h>
#include <data/iterable.hpp>
#include <data/bytes.hpp>
#include <data/size.hpp>
#include <iostream>
#include <random>

namespace data::random {

    template <typename engine> concept URBG = std::uniform_random_bit_generator<engine>;
    template <typename engine> concept RNG = std::uniform_random_bit_generator<engine> && std::derived_from<engine, reader<byte>>;

    // satisfies concept RNG.
    struct source : reader<byte> {

        using result_type = byte;

        constexpr static byte min () {
            return 0;
        }

        constexpr static byte max () {
            return 255;
        }

        // get one byte.
        byte operator () () {
            byte b;
            *this >> b;
            return b;
        }

        virtual ~source () {}

        // throw this when something goes wrong.
        struct fail : std::exception {
            const char *what () const noexcept override {
                return "failed to read entropy stream";
            }
        };

        void skip (size_t size) final override {}
    };

    template <std::integral X>
    source inline &operator >> (source &e, X &x) {
        e.read ((byte *) (&x), sizeof (x));
        return e;
    }

    source inline &operator >> (source &e, bool &b) {
        byte x;
        e >> x;
        b = x & 1 > 0;
        return e;
    }

    // the famous one-time pad.
    struct one_time_pad final : source {
        bytes Entropy;
        int Position;

        one_time_pad (byte_slice b) : Entropy {b}, Position {0} {}

        void read (byte *b, size_t x) final override {
            if (x > Entropy.size () - Position) throw source::fail {};

            for (int i = 0; i < x; i++) {
                b[i] = Entropy[i + Position];
                Position++;
            }
        }
    };

    // this type exists to enable a mode with absolutely
    // deterministic RNG starting a given seed. This is
    // not what we would want with ordinary use of the
    // program. It would be used to produce a
    // replicatable condition for testing.
    struct fixed_entropy final : source {
        bytes String;
        size_t Index;
        fixed_entropy (const bytes &b): String {b}, Index {0} {}
        void read (byte *result, size_t remaining) final override {
            while (remaining > 0) {
                size_t bytes_to_copy = remaining > (String.size () - Index) ? (String.size () - Index) : remaining;
                std::copy (String.begin () + Index, String.begin () + Index + bytes_to_copy, result);
                result += bytes_to_copy;
                remaining -= bytes_to_copy;
                if (Index == String.size ()) Index = 0;
            }
        }
    };

    // ask the user to type random characters into a keyboard.
    struct user_entropy final : source {
        std::string UserMessageAsk;
        std::string UserMessageAskMore;
        std::string UserMessageConfirm;
        std::ostream &Cout;
        std::istream &Cin;

        bytes Input;
        size_t Position;

        user_entropy (const std::string &ask, const std::string &ask_more, const std::string &confirm, std::ostream &out, std::istream &in) :
        UserMessageAsk {ask}, UserMessageAskMore {ask_more}, UserMessageConfirm {confirm}, Cout {out}, Cin {in}, Input {}, Position {0} {}

        void read (byte *, size_t x) final override;
    };

    template <typename engine> concept DRBG =
        RNG<engine> && std::derived_from<engine, source> &&
        requires (engine &e, byte_slice b) {
            { e.reseed (b) };
        };

    struct generator : source {
        virtual void reseed (byte_slice) = 0;
        virtual ~generator () {}
    };

    // convert any std engine type to an implementation of our concept.
    template <URBG engine> struct std_random final : generator {

        engine Engine;

        std_random () : generator {} {}

        std_random (uint64 u): std_random {byte_slice {(byte *) &u, 8}} {}

        std_random (byte_slice b) : std_random {} {
            reseed (b);
        }

        void read (byte *b, size_t z) override {
            for (int i = 0; i < z; i++) b[i] = Engine ();
        }

        // TODO use the whole number if more than 8 bytes are provided.
        void reseed (byte_slice b) override {
            uint64 seed {0};
            std::copy (b.begin (), b.begin () + (b.size () < 8 ? b.size () : 8), (byte *) &seed);
            Engine.seed (seed);
        }

        constexpr const static size_t MinEntropyLength = 8;
    };

    struct reseed_required : exception::base<reseed_required> {};

    // satisfy DRBG concept
    template <DRBG drbg, size_t min_seed_length = drbg::MinEntropyLength>
    struct automatic_reseed final : generator {
        source &Source;
        drbg Random;
        size_t BytesBeforeReseed;
        size_t BytesRemaining;

        // NOTE: EntropySize is not the same thing as strength.
        // The source we request ought to have a size that is
        // at least as much as the strength, but depending on
        // the quality of the source source, longer strings
        // may be required.
        size_t EntropySize;

        static bytes get_seed (source &e, size_t source_size) {
            bytes seed (source_size);
            e >> seed;
            return seed;
        }

        template <typename ...rest>
        automatic_reseed (source &source, size_t bytes_before_reseed, rest &&...r):
            Source {source}, Random {byte_slice (get_seed (source, min_seed_length)), std::forward<rest> (r)...},
            BytesBeforeReseed {bytes_before_reseed},
            BytesRemaining {0}, EntropySize {min_seed_length} {}

        void read (byte *b, size_t size) final override {
            if (BytesRemaining == 0) {
                Random.reseed (get_seed (Source, EntropySize));
                BytesRemaining = BytesBeforeReseed;
            }

            while (size > 0) {

                try {
                    while (BytesRemaining < size) {
                        Random.read (b, BytesRemaining);
                        b += BytesRemaining;
                        size -= BytesRemaining;
                        Random.reseed (get_seed (Source, EntropySize));
                        BytesRemaining = BytesBeforeReseed;
                    }

                    Random.read (b, size);
                    BytesRemaining -= size;
                    size = 0;
                } catch (reseed_required) {
                    Random.reseed (get_seed (Source, EntropySize));
                }
            }
        }

        void reseed (byte_slice x) final override {
            Random.reseed (x);
        }
    };

    using default_casual_random = automatic_reseed<std_random<std::default_random_engine>>;

    // NOTE: we do not actually implement this function.
    source &get ();

    uint32 select_index_by_weight (cross<double>, source &r);

}

#endif
