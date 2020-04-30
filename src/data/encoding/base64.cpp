
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

#include <data/encoding/base64.hpp>
#include <data/encoding/endian.hpp>

namespace data::encoding::base64 {
    
    size_t size(string_view source) {
        size_t source_size = source.size();
        if (source_size < 4) return 0;
        if (source[source_size - 1] == '=') {
            if (source[source_size - 2] == '=') return ((source_size - 4) / 4) * 3 + 1;
            else return ((source_size - 4) / 4) * 3 + 2;
        }
        return (source_size / 4) * 3;
    }
    
    uint32 padding(string_view source) {
        size_t source_size = source.size();
        if (source_size < 4) return 0;
        if (source[source_size - 1] == '=') {
            if (source[source_size - 2] == '=') return 2;
            else return 1;
        }
        return 0;
    }
    
    string::string(string_view source) : String{source}, Bytes{}, ToBytes{nullptr} {
        if (!base64::valid(source)) return;
        
        uint32 padding = base64::padding(source);
        
        Bytes.resize(size(source));
        
        const static uint32_t mask = 0x000000FF;

        const size_t length = source.length();
        if ((length % 4) != 0) return;
        Bytes.reserve(((length / 4) * 3) - padding);

        uint32_t value = 0;
        for (auto cursor = source.begin(); cursor < source.end();) {
            for (size_t position = 0; position < 4; position++) {
                value <<= 6;
                if (*cursor >= 0x41 && *cursor <= 0x5A)
                    value |= *cursor - 0x41;
                else if (*cursor >= 0x61 && *cursor <= 0x7A)
                    value |= *cursor - 0x47;
                else if (*cursor >= 0x30 && *cursor <= 0x39)
                    value |= *cursor + 0x04;
                else if (*cursor == 0x2B)
                    value |= 0x3E;
                else if (*cursor == 0x2F)
                    value |= 0x3F;
                else if (*cursor == Pad) {
                    // Handle 1 or 2 pad characters.
                    switch (source.end() - cursor) {
                        case 1:
                            Bytes.push_back((value >> 16) & mask);
                            Bytes.push_back((value >> 8) & mask);
                            ToBytes = &Bytes;
                            return;
                        case 2:
                            Bytes.push_back((value >> 10) & mask);
                            ToBytes = &Bytes;
                            return;
                        default:
                            return;
                    }
                } else return;

                cursor++;
            }

            Bytes.push_back((value >> 16) & mask);
            Bytes.push_back((value >> 8) & mask);
            Bytes.push_back((value >> 0) & mask);
        }

        ToBytes = &Bytes;
    }
    
    std::string write(bytes_view sourceBytes){
        std::string output;
        const auto size = sourceBytes.size();
        output.reserve(((size / 3) + (size % 3 > 0)) * 4);
        
        std::string Characters = characters();

        uint32_t value;
        auto cursor = sourceBytes.begin();
        for (size_t position = 0; position < size / 3; position++)
        {
            // Convert to big endian.
            value = (*cursor++) << 16;

            value += (*cursor++) << 8;
            value += (*cursor++);
            output.append(1, Characters[(value & 0x00FC0000) >> 18]);
            output.append(1, Characters[(value & 0x0003F000) >> 12]);
            output.append(1, Characters[(value & 0x00000FC0) >> 6]);
            output.append(1, Characters[(value & 0x0000003F) >> 0]);
        }

        switch (size % 3)
        {
            case 1:
                // Convert to big endian.
                value = (*cursor++) << 16;

                output.append(1, Characters[(value & 0x00FC0000) >> 18]);
                output.append(1, Characters[(value & 0x0003F000) >> 12]);
                output.append(2, Pad);
                break;
            case 2:
                // Convert to big endian.
                value = (*cursor++) << 16;

                value += (*cursor++) << 8;
                output.append(1, Characters[(value & 0x00FC0000) >> 18]);
                output.append(1, Characters[(value & 0x0003F000) >> 12]);
                output.append(1, Characters[(value & 0x00000FC0) >> 6]);
                output.append(1, Pad);
                break;
        }

        return output;
    }
    
    std::string write(bytes_view sourceBytes, endian::order r) {
        if (r == endian::big) return write(sourceBytes);
        bytes reversed(sourceBytes.size());
        std::copy(sourceBytes.rbegin(), sourceBytes.rend(), reversed.begin());
        return write(reversed);
    }
    
    std::string write(uint64 x) {
        return write(bytes_view{uint64_big{x}.data(), sizeof(uint64)});
    }
    
    std::string write(uint32 x) {
        return write(bytes_view{uint32_big{x}.data(), sizeof(uint32)});
    }
    
    std::string write(uint16 x) {
        return write(bytes_view{uint16_big{x}.data(), sizeof(uint16)});
    }
    
    std::string write(byte x) {
        return write(bytes_view{(byte*)(&x), sizeof(byte)});
    }
}
