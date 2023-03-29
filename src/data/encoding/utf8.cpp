#include <data/encoding/unicode.hpp>
#include <iostream>
#include "utf8.h"

namespace data::encoding::UTF8 {

    size_t utf8_len (const std::u32string &u32_str) {
        size_t utf8_len = 0;

        for (char32_t cp : u32_str) {
            if (cp < 0x80) {
                utf8_len += 1;
            } else if (cp < 0x800) {
                utf8_len += 2;
            } else if (cp < 0x10000) {
                utf8_len += 3;
            } else {
                utf8_len += 4;
            }
        }

        return utf8_len;
    }
    
    string encode (const data::unicode &x) {
        string m;
        m.resize (utf8_len (x));
        auto it = m.begin ();
        ::utf8::utf32to8 (x.begin (), x.end (), it);
        return m;
    }
    
    ptr<data::unicode> decode (const string &s) {
        ptr<data::unicode> p;
        try {
            std::u32string m {};
            m.resize (s.size ());
            auto it = m.begin ();
            ::utf8::utf8to32 (s.begin (), s.end (), it);
            p = std::make_shared<data::unicode> ();
            p->resize (it - m.begin ());
            std::copy (m.begin (), it, p->begin ());
            return p;
        } catch (::utf8::exception& e) {
            return nullptr;
        }
    }
    
}
