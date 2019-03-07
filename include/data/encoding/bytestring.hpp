#ifndef DATA_ENCODING_BYTESTRING_HPP
#define DATA_ENCODING_BYTESTRING_HPP

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian.hpp>

namespace data {
    
    using bytestring = ::data::slice<byte>;
    
    class reader {
        bytestring::iterator i;
        bytestring::iterator e;
        
    public: 
        bool Valid;
        reader(bytestring b) : i(b.begin()), e(b.end()), Valid(true) {}
        
    private:
        reader(bytestring::iterator ee) : i(ee), e(ee), Valid(false) {}

    public:
        bool end() const {
            return i == e;
        }

        reader invalidate() const {
            return reader(e);
        }
        
        reader read(byte&) const;
        
        reader check(byte b) const {
            byte x;
            reader r = read(x);
            return b == x ? r : r.invalidate();
        }
        
        reader read(std::vector<byte>& v) const;
        
        reader read(uint16_t&) const;
        
        reader read(uint32_t&) const;
        
        reader read(endian::ness, uint16_t&) const;
        
        reader read(endian::ness, uint32_t&) const;
        
        reader read(endian::ness, uint64_t&) const;
    };
    
    class writer {
        bytestring::iterator i;
        bytestring::iterator e;
        
    public: 
        bool Valid;
        
        writer& operator=(writer w) {
            i = w.i;
            e = w.e;
            return *this;
        }
        
    private:
        writer(bytestring::iterator initial, bytestring::iterator end) : i(initial), e(end), Valid(true) {}
        writer(bytestring::iterator ee) : i(ee), e(ee), Valid(false) {}

    public:
        static writer make(bytestring& b) {
            return writer(b.begin(), b.end());
        }
        
        writer write(const byte) const;
        
        writer write(const bytestring) const;
        
        writer write(uint16_t) const;
        
        writer write(uint32_t) const;
        
        writer write(endian::ness, uint16_t) const;
        
        writer write(endian::ness, uint32_t) const;
        
        writer write(endian::ness, uint64_t) const;
    };

}

#endif 
