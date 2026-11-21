#include <NTL/random.hpp>

namespace NTL {
    void generator::read (data::byte *data, size_t size) {
        while (size != 0) {

            // RandomStream::get takes a long.
            const auto n = static_cast<size_t> ( std::min ( size, static_cast<size_t> (std::numeric_limits<long>::max ()) ) );

            Stream.get (
                reinterpret_cast<unsigned char *> (data),
                static_cast<long> (n)
            );

            data += n;
            size -= n;

        }

    }

    RandomStream generator::make_stream (data::byte_slice seed) {
        RandomStreamPush push;

        SetSeed (
            reinterpret_cast<const unsigned char *> (seed.data ()),
            static_cast<long> (seed.size ())
        );

        return GetCurrentRandomStream ();
    }
}
