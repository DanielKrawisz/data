// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_RANDOM
#define DATA_RANDOM

#include <data/iterable.hpp>
#include <data/bytes.hpp>
#include <data/size.hpp>
#include <iostream>
#include <random>
#include <chrono>

namespace data {

    template <typename engine> concept URBG = std::uniform_random_bit_generator<engine>;

    struct entropy : reader<byte> {

        using result_type = byte;

        constexpr static byte min () {
            return 0;
        }

        constexpr static byte max () {
            return 255;
        }

        byte operator () () {
            byte b;
            *this >> b;
            return b;
        }

        virtual ~entropy () {}

        // throw this when something goes wrong.
        struct fail : std::exception {
            const char *what () const noexcept override {
                return "failed to read entropy stream";
            }
        };

        void skip (size_t size) final override {}
    };

    template <std::integral X>
    entropy inline &operator >> (entropy &e, X &x) {
        e.read ((byte *) (&x), sizeof (x));
        return e;
    }

    struct entropy_sum final : entropy {
        ptr<entropy> Left;
        ptr<entropy> Right;

        entropy_sum (ptr<entropy> left, ptr<entropy> right): Left {left}, Right {right} {}

        void read (byte *b, size_t x) final override {
            Left->read (b, x);
            bytes c (x);
            Right->read (c.data (), x);
            arithmetic::bit_xor<byte> (c.data () + x, c.data (), c.data (), b);
        }
    };

    // ask the user to type random characters into a keyboard.
    struct user_entropy final : entropy {
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

    // the famous one-time pad.
    struct fixed_entropy final : entropy {
        bytes Entropy;
        int Position;

        fixed_entropy (byte_slice b) : Entropy {b}, Position {0} {}

        void read (byte *b, size_t x) final override {
            if (x > Entropy.size () - Position) throw entropy::fail {};

            for (int i = 0; i < x; i++) {
                b[i] = Entropy[i + Position];
                Position++;
            }
        }
    };

    // TODO replace this with something good.
    using random_engine = std::default_random_engine;

    random_engine& get_random_engine ();

    template <URBG engine>
    struct std_random final : entropy {

        engine Engine;

        std_random () : std_random {std::chrono::system_clock::now ().time_since_epoch ().count ()} {}
        std_random (data::uint64 seed);

        void read (byte *b, size_t z) override {
            for (int i = 0; i < z; i++) b[i] = Engine ();
        }
    };

    // combine two random sources via xor.
    struct linear_combination_random final : entropy {
        size_t Ratio;
        ptr<entropy> Cheap;
        // get one secure byte for every Ratio cheap bytes and xor it with them.
        ptr<entropy> Secure;

        linear_combination_random (size_t r, ptr<entropy> cheap, ptr<entropy> secure) : Ratio {r}, Cheap {cheap}, Secure {secure} {}

        void read (byte *b, size_t z) override {
            if (current_cheap + z > Ratio) {
                size_t until_next = z - ((current_cheap + z) - Ratio);
                read (b, until_next);
                *Secure >> current_secure;
                current_cheap = 0;
                read (b + until_next, z - until_next);
            }

            Cheap->read (b, z);
            for (size_t t = 0; t < z; t++) b[t] ^= current_secure;
            current_cheap += z;
        }

    protected:
        byte current_secure;
        size_t current_cheap;
    };

    template <> inline std_random<std::default_random_engine>::std_random (data::uint64 seed) : Engine {} {
        Engine.seed (seed);
    }

    uint32 select_index_by_weight (cross<double>, entropy &r);

}

#endif
