// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SPONGE
#define DATA_CRYPTO_SPONGE

#include <data/string.hpp>
#include <data/bytes.hpp>
#include <data/stream.hpp>

namespace data::crypto::sponge {

    template <typename absorber, typename squeezer> concept Sponge =
        data::Writer<absorber, byte> && data::Reader<squeezer, byte> &&
        std::default_initializable<absorber> &&
        std::movable<absorber> && requires (absorber &&a) {
            { squeezer {std::move (a)} };
        };

    template <typename squeezer, Sponge<squeezer> absorber, typename ...X>
    squeezer inline XOF (X ...x) {
        absorber w {};
        (w << ... << x);
        return squeezer {std::move (w)};
    }

    template <typename squeezer, Sponge<squeezer> absorber>
    squeezer inline shake (byte_slice b) {
        return XOF<squeezer, absorber> (b);
    }

    template <typename squeezer, Sponge<squeezer> absorber>
    struct custom_shake {
        const string &Name;
        const bytes &Customization;
        template <typename ...inputs>
        squeezer operator () (inputs... in) const {
            return XOF<squeezer, absorber> (Name, Customization, in...);
        }
    };

    template <typename squeezer, Sponge<squeezer> absorber, size_t size>
    struct hash_function : custom_shake<squeezer, absorber> {
        using custom_shake<squeezer, absorber>::custom_shake;
        hash::digest<size> operator () (byte_slice msg) const {
            hash::digest<size> dig;
            static_cast<const custom_shake<squeezer, absorber> &> (*this) (msg).read (dig.data (), dig.size ());
            return dig;
        }
    };

    template <typename squeezer, Sponge<squeezer> absorber, size_t size>
    struct MAC_function : custom_shake<squeezer, absorber> {
        using custom_shake<squeezer, absorber>::custom_shake;
        hash::digest<size> operator () (byte_slice key, byte_slice msg) const {
            hash::digest<size> dig;
            static_cast<const custom_shake<squeezer, absorber> &> (*this) (key, msg).read (dig.data (), dig.size ());
            return dig;
        }
    };

    template <typename squeezer, Sponge<squeezer> absorber, size_t size>
    struct hash_engine {
        constexpr const static size_t DigestSize = size;
        hash_engine (): Absorber {} {}

        hash_engine &Update (const byte *b, size_t z) {
            Absorber.write (b, z);
            return *this;
        }

        void Final (byte b[DigestSize]) {
            squeezer {std::move (Absorber)}.read (b, DigestSize);
        }

        hash_engine &Restart () {
            *this = hash_engine {};
            return *this;
        }

    private:
        absorber Absorber;
    };

    template <typename squeezer, Sponge<squeezer> absorber, size_t size>
    struct hash_writer : absorber {
        constexpr const static size_t DigestSize = size;
        using digest = hash::digest<DigestSize>;

        digest &Digest;

        hash_writer (digest &d): Digest {d} {}

        ~hash_writer () {
            squeezer {std::move (static_cast<absorber &> (*this))}.read (Digest.data (), DigestSize);
        }

        hash_writer (const hash_writer &) = delete;
        hash_writer &operator = (const hash_writer&) = delete;
        hash_writer (hash_writer&&) = delete;
        hash_writer &operator = (hash_writer &&) = delete;
    };

    template <typename squeezer, Sponge<squeezer> absorber, size_t size>
    struct MAC_engine : hash_engine<squeezer, absorber, size> {
        bytes Key;

        MAC_engine (byte_slice key): hash_engine<squeezer, absorber, size> {}, Key (key.size ()) {
            std::copy (key.begin (), key.end (), Key.begin ());
            Update (Key.data (), Key.size ());
        }

        MAC_engine &Update (const byte *b, size_t z) {
            static_cast<hash_engine<squeezer, absorber, size> &> (*this).Update (b, z);
            return *this;
        }

        MAC_engine &Restart () {
            static_cast<hash_engine<squeezer, absorber, size> &> (*this).Update (Key.data (), Key.size ());
            return *this;
        }
    };

    template <typename squeezer, Sponge<squeezer> absorber, size_t size>
    struct MAC_writer : hash_writer<squeezer, absorber, size> {
        using digest = hash_writer<squeezer, absorber, size>::digest;
        MAC_writer (digest &d, byte_slice key): hash_writer<squeezer, absorber, size> {d} {
            this->write (key.data (), key.size ());
        }
    };

}

#endif
