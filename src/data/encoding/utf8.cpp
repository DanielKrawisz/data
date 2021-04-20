#include <data/encoding/unicode.hpp>
#include "utf8.h"

namespace data::encoding::unicode {
    
    bytes utf8_encode(const string& x) {
        try {
            bytes m{static_cast<unsigned char>(x.size() * 4)};
            auto it = m.begin();
            ::utf8::utf32to8(x.begin(), x.end(), it);
            bytes z{static_cast<unsigned char>(it - m.begin())};
            std::copy(m.begin(), it, z.begin());
            return z;
        } catch (::utf8::exception& e) {
            return {};
        }
    }
    
    bytes utf8_encode(const std::u32string& x) {
        try {
            bytes m{static_cast<unsigned char>(x.size())};
            auto it = m.begin();
            ::utf8::utf32to8(x.begin(), x.end(), it);
            bytes z{static_cast<unsigned char>(it - m.begin())};
            std::copy(m.begin(), it, z.begin());
            return z;
        } catch (::utf8::exception& e) {
            return {};
        }
    }
    
    ptr<std::u32string> utf8_decode(const bytes& s) {
        ptr<std::u32string> p;
        try {
            std::u32string m{};
            m.resize(s.size());
            auto it = m.begin();
            ::utf8::utf8to32(s.begin(), s.end(), it);
            p = std::make_shared<std::u32string>();
            p->resize(it - m.begin());
            std::copy(m.begin(), it, p->begin());
            return p;
        } catch (::utf8::exception& e) {
            return nullptr;
        }
    }
    
}
