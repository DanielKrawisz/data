// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/types.hpp>

namespace data {

    template <size_t size> struct decimal {
        char Value[size] = {};
        bool Valid {false};
        
        constexpr decimal (const char (&input)[size]) noexcept {
            if (size <= 1) return;
            if (input[0] < '1' || input[0] > '9') return;

            for (size_t i {1}; i < size - 1; i++)
                if (input[i] < '0' || input[i] > '9') return;

            if (input[size - 1] != '\0') return;

            Valid = true;
            for (size_t i {0}; i < size; i++) Value[i] = input[i];
        }
        
        constexpr operator uint64 () const {
            if (size > 20) return 0;
            uint64 x {0};
            uint64 digit {1};
            for (int i {size-2}; i >= 0; --i) {
                x += digit * static_cast<uint64> (Value[i] - '0');
                digit *= 10;
            }
            return x;
        }
    };

    template <size_t size>
    std::ostream inline &operator << (std::ostream &o, const decimal<size> d) {
        return o << "decimal["<< size <<"]" <<'"' << d.Value << '"';
    }

    // make sure these two won't work.
    template <> struct decimal<0>;
    template <> struct decimal<1>;
    
    template <size_t N> decimal (const char (&)[N]) -> decimal<N>;
    template <size_t N> decimal (decimal<N>) -> decimal<N>;
}

#endif
