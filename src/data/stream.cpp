//
// Created by nekosune on 13/07/19.
//

#include "data/stream.hpp"
#include <boost/endian/conversion.hpp>
#include <boost/endian/buffers.hpp>

namespace data {

    writer writer::operator<<(bytes_view x) const {
        ostream<byte> w = Writer;
        for(bytes_view::iterator i = x.begin(); i != x.end(); i++) w = w << *i;
        return writer{w};
    }

    writer writer::operator<<(const ordered<uint16, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_uint16_buf_t{x.Value}.data(), sizeof(uint16)});
    }
        
    writer writer::operator<<(const ordered<uint16, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_uint16_buf_t{x.Value}.data(), sizeof(uint16)});
    }

    writer writer::operator<<(const ordered<uint32, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_uint32_buf_t{x.Value}.data(), sizeof(uint32)});
    }
        
    writer writer::operator<<(const ordered<uint32, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_uint32_buf_t{x.Value}.data(), sizeof(uint32)});
    }

    writer writer::operator<<(const ordered<uint64, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_uint64_buf_t{x.Value}.data(), sizeof(uint64)});
    }
        
    writer writer::operator<<(const ordered<uint64, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_uint64_buf_t{x.Value}.data(), sizeof(uint64)});
    }

    writer writer::operator<<(const ordered<int16, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_int16_buf_t{x.Value}.data(), sizeof(int16)});
    }
        
    writer writer::operator<<(const ordered<int16, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_int16_buf_t{x.Value}.data(), sizeof(int16)});
    }

    writer writer::operator<<(const ordered<int32, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_int32_buf_t{x.Value}.data(), sizeof(int32)});
    }
        
    writer writer::operator<<(const ordered<int32, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_int32_buf_t{x.Value}.data(), sizeof(int32)});
    }

    writer writer::operator<<(const ordered<int64, big> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::big_int64_buf_t{x.Value}.data(), sizeof(int64)});
    }
        
    writer writer::operator<<(const ordered<int64, little> x) const {
        return operator<<(bytes_view{(const byte*)boost::endian::little_int64_buf_t{x.Value}.data(), sizeof(int64)});
    }

    void reader::operator>>(std::vector<byte> &x) {
        for(int i=0;i<x.size();i++)
        {
            if (It == Slice.end()) throw end_of_stream{};
            x[i]=*It;
            It++;
        }

    }

    void forward(const bytes_view Slice, slice<byte>::const_iterator& It, int amount, char* to) {
        for(int i=0;i<amount;i++)
        {
            if (It == Slice.end()) {
                throw end_of_stream{};
            }
            *to = *It;
            It++;
            to++;
        }
    }

    void reader::operator>>(ordered<uint16, little>& x) {
        forward(Slice, It, sizeof(uint16), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<uint16, big>& x) {
        forward(Slice, It, sizeof(uint16), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<uint32, little>& x) {
        forward(Slice, It, sizeof(uint32), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<uint32, big>& x) {
        forward(Slice, It, sizeof(uint32), (char*)(&x.Value));
    }
    
    void reader::operator>>(ordered<uint64, big>& x) {
        forward(Slice, It, sizeof(uint64), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<uint64, little>& x) {
        forward(Slice, It, sizeof(uint64), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<int16, big>& x) {
        forward(Slice, It, sizeof(int16), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<int16, little>& x) {
        forward(Slice, It, sizeof(int16), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<int32, big>& x) {
        forward(Slice, It, sizeof(int32), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<int32, little>& x) {
        forward(Slice, It, sizeof(int32), (char*)(&x.Value));
    }
    
    void reader::operator>>(ordered<int64, big>& x) {
        forward(Slice, It, sizeof(int64), (char*)(&x.Value));
    }

    void reader::operator>>(ordered<int64, little>& x) {
        forward(Slice, It, sizeof(int64), (char*)(&x.Value));
    }

}
