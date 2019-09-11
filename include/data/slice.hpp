// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SLICE
#define DATA_SLICE

#include "list.hpp"
#include "stream.hpp"
#include <boost/endian/conversion.hpp>
#include <data/tools/index_iterator.hpp>

namespace data {
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X, size_t ...> struct slice;
    
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X> struct slice<X> {
        using iterator = X*;
        using const_iterator = const X*;
        using value_type = X;
    private:
        iterator Begin;
        iterator End;
    protected:
        slice(iterator b, iterator e) : Begin{b}, End{e} {}
        slice(iterator b, size_t size) : Begin{b}, End{b + size} {}
    public:
        slice() : Begin{nullptr}, End{nullptr} {}
        slice(std::vector<X>& x) : slice(x.data(), x.size()) {}
        template <size_t n>
        slice(std::array<X, n>& x) : slice(x.data(), x.size()) {} 
        
        static const slice make(vector<X>& x) {
            return slice{const_cast<std::vector<X>&>(x)};
        }
        
        template <size_t n>
        static const slice<X> make(std::array<X, n>& x) {
            return slice{const_cast<std::array<X, n>&>(x)};
        }
        
        virtual const size_t size() const {
            return End - Begin;
        }
        
        X& operator[](size_t n) const {
            if (n >= size()) throw std::out_of_range{"index out of range"};
            return *(Begin + n);
        }

        /// Selects a range from the current slice
        /// \param begin range begins from this index inclusive
        /// \param end range ends at this index excluisive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int32 begin, int32 end) const {
            size_t len = size();
            if(begin<0) begin=len+begin;
            if(end<0) end=len+end;
            if (begin >= len || end > len || begin >= end || begin < 0) return slice{};

            return slice{Begin + begin, end-begin};
        }

        /// Selects a range from the current slice up to end of slice
        /// \param begin  range begins from this index inclusive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int32 begin) const {
            return range(begin,size());
        }
        
        slice& operator=(const slice<X>& s) {
            Begin = s.Begin;
            End = s.End;
            return *this;
        }
        
        bool operator==(const slice<X>& s) const {
            return Begin == s.Begin && End == s.End;
        }
        
        iterator begin() {
            return Begin;
        }
        
        iterator end() {
            return End;
        }
        
        const_iterator begin() const {
            return Begin;
        }
        
        const_iterator end() const {
            return End;
        }
        
    };
    
    template <typename X, size_t n> struct slice<X, n> : public slice<X> {
        const size_t size() const final override {
            return n;
        }
        
        slice(std::array<X, n>& x) : slice<X>{x} {}
    };
    
    template <typename X>
    struct slice_ostream : public virtual ostream<X> {
    protected:
        slice<X> Slice;
        typename slice<X>::iterator It;
    public:
        void operator<<(X x) final {
            if (It == Slice.end()) throw end_of_stream{};
            *It = x;
            It++;
        }

        explicit slice_ostream(slice<X> s) : Slice{s}, It{Slice.begin()} {}

        explicit slice_ostream(std::vector<X> &v) : slice_ostream{slice<X>{v}} {}
    };
    
    template <typename X>
    struct slice_istream : public virtual istream<X> {
    protected:
        const slice<X> Slice;
        typename slice<X>::const_iterator It;
    public:

        void operator>>(X& x) final {
            if (It == Slice.end()) throw end_of_stream{};
            x = *It;
            It++;
        }
        
        explicit slice_istream(slice<X> s) : Slice{s}, It{Slice.begin()} {}
        explicit slice_istream(std::vector<X>& v) : slice_istream{slice<X>{v}} {}

    };
    
    class slice_writer : public slice_ostream<byte>, public writer {
        void bytesIntoIterator(const char*,int);
    public:
        const boost::endian::order Endian;
        using slice_ostream<byte>::operator<<;
        void operator<<(uint16_t) final override;
        void operator<<(uint32_t) final override;
        void operator<<(uint64_t) final override;
        void operator<<(int16_t) final override;
        void operator<<(int32_t) final override;
        void operator<<(int64_t) final override;
        void operator<<(bytes&) final override;
        
        slice_writer(slice<byte> s, boost::endian::order e) : slice_ostream<byte>{s}, Endian{e} {}
        slice_writer(std::vector<byte>& v, boost::endian::order e) : slice_ostream<byte>{v}, Endian{e} {}
    };
    
    class slice_reader : public slice_istream<byte>, public reader {
        char* iteratorToArray(int);
    public:
        const boost::endian::order Endian;
        using slice_istream<byte>::operator>>;
        void operator>>(uint16_t&) final override;
        void operator>>(uint32_t&) final override;
        void operator>>(uint64_t&) final override;
        void operator>>(int16_t&) final override;
        void operator>>(int32_t&) final override;
        void operator>>(int64_t&) final override;
        void operator>>(std::vector<byte>&) final override;
        
        slice_reader(slice<byte> s, boost::endian::order e) : slice_istream<byte>{s}, Endian{e} {}
        slice_reader(std::vector<byte>& v, boost::endian::order e) : slice_istream<byte>{v}, Endian{e} {}
    };

}

#endif
