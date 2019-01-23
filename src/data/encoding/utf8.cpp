#include "utf8.h"
#include <data/encoding/utf8.hpp>

namespace data {
    
    namespace encoding {
        
        namespace utf8 {

            string write(unicode x) {
                try {
                    std::vector<byte> m{static_cast<unsigned char>(x.size() * 4)};
                    auto it = m.begin();
                    ::utf8::utf32to8(x.begin(), x.end(), &it);
                    std::vector<byte> x{static_cast<unsigned char>(&*it - &*m.begin())};
                    std::copy(m.begin(), it, x.begin());
                    return x;
                } catch (::utf8::exception& e) {
                    return {};
                }
            }

            unicode read(string s) {
                try {
                    std::vector<uint32_t> m(s.size());
                    auto it = m.begin();
                    ::utf8::utf8to32(s.begin(), s.end(), &it);
                    std::vector<uint32_t> x{static_cast<unsigned char>((&*it - &*m.begin()) / 4)};
                    std::copy(m.begin(), it, x.begin());
                    return x;
                } catch (::utf8::exception& e) {
                    return {};
                }
            }

        }

    }

}
