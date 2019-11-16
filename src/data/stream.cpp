//
// Created by nekosune on 13/07/19.
//

#include "data/stream.hpp"
#include <boost/endian/conversion.hpp>
#include <boost/endian/buffers.hpp>

namespace data {
    
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
