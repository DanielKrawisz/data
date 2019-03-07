#ifndef DATA_ENCODING_ENDIAN_HPP
#define DATA_ENCODING_ENDIAN_HPP

#include <stdint.h>

namespace data {

    namespace endian {

        const uint16_t uint16_all = uint16_t(0) - uint16_t(1);

        const uint32_t uint32_all = uint32_t(0) - uint32_t(1);

        const uint64_t uint64_all = uint64_t(0) - uint64_t(1);

        const uint16_t uint16_first_half = uint16_all << uint16_t(8);
        const uint16_t uint16_second_half = uint16_all >> 8;

        const uint32_t uint32_first_half = uint32_all << 16;
        const uint32_t uint32_second_half = uint32_all >> 16;

        const uint64_t uint64_first_half = uint64_all << 32;
        const uint64_t uint64_second_half = uint64_all >> 32;

        const uint32_t uint32_first_byte = uint32_first_half << 8;
        const uint32_t uint32_second_byte = uint32_first_byte >> 8;
        const uint32_t uint32_third_byte = uint32_second_byte >> 8;
        const uint32_t uint32_fourth_byte = uint32_third_byte >> 8;

        const uint64_t uint64_first_fourth = uint64_first_half << 16;
        const uint64_t uint64_second_fourth = uint64_first_fourth >> 16;
        const uint64_t uint64_third_fourth = uint64_second_fourth >> 16;
        const uint64_t uint64_fourth_fourth = uint64_third_fourth >> 16;

        const uint64_t uint64_first_byte = uint64_first_fourth << 8;
        const uint64_t uint64_second_byte = uint64_first_byte >> 8;
        const uint64_t uint64_third_byte = uint64_second_byte >> 8;
        const uint64_t uint64_fourth_byte = uint64_third_byte >> 8;
        const uint64_t uint64_fifth_byte = uint64_fourth_byte >> 8;
        const uint64_t uint64_sixth_byte = uint64_fifth_byte >> 8;
        const uint64_t uint64_seventh_byte = uint64_sixth_byte >> 8;
        const uint64_t uint64_eighth_byte = uint64_seventh_byte >> 8;

        inline uint16_t reverse_bytes(uint16_t x) {
            return ((x & uint16_first_half) >> 8) + ((x & uint16_second_half) << 8);
        }

        inline uint32_t reverse_bytes(uint32_t x) {
            return ((x & uint32_first_byte) >> 24) + ((x & uint32_second_byte) >> 8) + ((x & uint32_third_byte) << 8) + ((x & uint32_fourth_byte) << 24);
        }

        inline uint64_t reverse_bytes(uint64_t x) {
            return ((x & uint64_first_byte) >> 56) + ((x & uint64_second_byte) >> 40) + ((x & uint64_third_byte) >> 24) + ((x & uint64_fourth_byte) >> 8)
                + ((x & uint64_fifth_byte) << 8) + ((x & uint64_sixth_byte) << 24) + ((x & uint64_seventh_byte) << 40) + ((x & uint64_eighth_byte) << 56);
        }

        namespace experiment {

            const unsigned int endianness_check_var = 1;

        }

        const bool Little = (*((char*)&experiment::endianness_check_var)) == experiment::endianness_check_var;

        const bool Big = !Little;

        enum ness {
            big, 
            little
        };

        ness Endianness = Little ? little : big;

        template <typename X>
        inline X to_or_from(ness e, X x) {
            e == Endianness ? x : reverse_bytes(x); 
        }

        const uint16_t uint16_first_significant_byte = Big ? uint16_first_half : uint16_second_half;
        const uint16_t uint16_second_significant_byte = Big ? uint16_second_half : uint16_first_half;

        const uint32_t uint32_first_significant_byte = Big ? uint32_first_byte : uint32_fourth_byte;
        const uint32_t uint32_second_significant_byte = Big ? uint32_second_byte : uint32_third_byte;
        const uint32_t uint32_third_significant_byte = Big ? uint32_third_byte : uint32_second_byte;
        const uint32_t uint32_fourth_significant_byte = Big ? uint32_fourth_byte : uint32_first_byte;

    }

}

#endif 

