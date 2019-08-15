//
// Created by nekosune on 13/07/19.
//

#include "data/slice.hpp"
#include <boost/endian/conversion.hpp>
#include <boost/endian/buffers.hpp>

namespace data {

    void slice_writer::operator<<(uint16_t x) {
        if(this->Endian==boost::endian::order::big) {
            boost::endian::big_uint16_buf_t buff(x);
            bytesIntoIterator(buff.data(), sizeof(uint16_t));
        }
        else {
            boost::endian::little_uint16_buf_t buff(x);
            bytesIntoIterator(buff.data(),sizeof(uint16_t));
        }
    }

    void slice_writer::operator<<(bytes & x) {
        for (unsigned char i : x) {
            if (It == Slice.end()) throw end_of_stream{};
            *It = i;
            It++;
        }
    }

    void slice_writer::operator<<(uint32_t x) {
        if(this->Endian==boost::endian::order::big) {
            boost::endian::big_uint32_buf_t buff(x);
            bytesIntoIterator(buff.data(), sizeof(uint32_t));
        }
        else {
            boost::endian::little_uint32_buf_t buff(x);
            bytesIntoIterator(buff.data(),sizeof(uint32_t));
        }
    }

    void slice_writer::operator<<(uint64_t x) {
        if(this->Endian==boost::endian::order::big) {
            boost::endian::big_uint64_buf_t buff(x);
            bytesIntoIterator(buff.data(), sizeof(uint64_t));
        }
        else {
            boost::endian::little_uint64_buf_t buff(x);
            bytesIntoIterator(buff.data(),sizeof(uint64_t));
        }
    }



    void slice_writer::operator<<(int16_t x) {
        if(this->Endian==boost::endian::order::big) {
            boost::endian::big_int16_buf_t buff(x);
            bytesIntoIterator(buff.data(), sizeof(int16_t));
        }
        else {
            boost::endian::little_int16_buf_t buff(x);
            bytesIntoIterator(buff.data(),sizeof(int16_t));
        }
    }

    void slice_writer::operator<<(int32_t x) {
        if(this->Endian==boost::endian::order::big) {
            boost::endian::big_int32_buf_t buff(x);
            bytesIntoIterator(buff.data(), sizeof(int32_t));
        }
        else {
            boost::endian::little_int32_buf_t buff(x);
            bytesIntoIterator(buff.data(),sizeof(int32_t));
        }
    }

    void slice_writer::operator<<(int64_t x) {
        if(this->Endian==boost::endian::order::big) {
            boost::endian::big_int64_buf_t buff(x);
            bytesIntoIterator(buff.data(), sizeof(int64_t));
        }
        else {
            boost::endian::little_int64_buf_t buff(x);
            bytesIntoIterator(buff.data(),sizeof(int64_t));
        }
    }

    void slice_writer::bytesIntoIterator(const char* data,int count) {
        for(int i=0;i<count;i++)
        {
            if (It == Slice.end())
            {
                throw end_of_stream{
                };
                return;
            };
            *It = (unsigned char) data[i];
            It++;
        }
    }



    void slice_reader::operator>>(uint16_t &x) {
        char* array=iteratorToArray(sizeof(uint16_t));
        if(this->Endian==boost::endian::order::big) {
            x=(array[0] << 8) + array[1];
            delete[] array;
        }
        else
        {
            x=(array[1] << 8) + array[0];
            delete[] array;
        }
    }

    void slice_reader::operator>>(uint32_t &x) {
        char* array=iteratorToArray(sizeof(uint32_t));
        if(this->Endian==boost::endian::order::big) {
            x=(array[0] << 24) + (array[1] << 16) +(array[2] << 8) +array[3];
            delete[] array;
        }
        else
        {
            x=(array[3] << 24) + (array[2] << 16) +(array[1] << 8) +array[0];
            delete[] array;
        }
    }

    void slice_reader::operator>>(uint64_t &x) {
        char* array=iteratorToArray(sizeof(uint64_t));
        if(this->Endian==boost::endian::order::big) {
            x= (((uint64_t)array[0]) << 56) + (((uint64_t)array[1]) << 48) +(((uint64_t)array[2]) << 40)+(((uint64_t)array[3]) << 32)+(((uint64_t)array[4]) << 24) + (((uint64_t)array[5]) << 16) +(((uint64_t)array[6]) << 8) +array[7];
            delete[] array;
        }
        else
        {
            x= (((uint64_t)array[7]) << 56) + (((uint64_t)array[6]) << 48) +(((uint64_t)array[5]) << 40)+(((uint64_t)array[4]) << 32)+(((uint64_t)array[3]) << 24) + (((uint64_t)array[2]) << 16) +(((uint64_t)array[1]) << 8) +array[0];
            delete[] array;
        }
    }

    void slice_reader::operator>>(std::vector<byte> &x) {
        for(int i=0;i<x.size();i++)
        {
            if (It == Slice.end()) throw end_of_stream{};
            x[i]=*It;
            It++;
        }

    }

    void slice_reader::operator>>(int16_t & x) {
        char* array=iteratorToArray(sizeof(int16_t));
        if(this->Endian==boost::endian::order::big) {
            x=(array[0] << 8) + array[1];
            delete[] array;
        }
        else
        {
            x=(array[1] << 8) + array[0];
            delete[] array;
        }
    }

    void slice_reader::operator>>(int32_t & x) {
        char* array=iteratorToArray(sizeof(int32_t));
        if(this->Endian==boost::endian::order::big) {
            x=(array[0] << 24) + (array[1] << 16) +(array[2] << 8) +array[3];
            delete[] array;
        }
        else
        {
            x=(array[3] << 24) + (array[2] << 16) +(array[1] << 8) +array[0];
            delete[] array;
        }
    }

    void slice_reader::operator>>(int64_t & x) {
        char* array=iteratorToArray(sizeof(int64_t));
        if(this->Endian==boost::endian::order::big) {
            x= (((uint64_t)array[0]) << 56) + (((uint64_t)array[1]) << 48) +(((uint64_t)array[2]) << 40)+(((uint64_t)array[3]) << 32)+(((uint64_t)array[4]) << 24) + (((uint64_t)array[5]) << 16) +(((uint64_t)array[6]) << 8) +array[7];
            delete[] array;
        }
        else
        {
            x= (((uint64_t)array[7]) << 56) + (((uint64_t)array[6]) << 48) +(((uint64_t)array[5]) << 40)+(((uint64_t)array[4]) << 32)+(((uint64_t)array[3]) << 24) + (((uint64_t)array[2]) << 16) +(((uint64_t)array[1]) << 8) +array[0];
            delete[] array;
        }
    }

    char *slice_reader::iteratorToArray(int amount) {
        char* array=new char[amount];
        for(int i=0;i<amount;i++)
        {
            if (It == Slice.end()) {
                throw end_of_stream{};
                return 0;
            }
            array[i]=*It;
            It++;
        }
        return array;
    }


}
