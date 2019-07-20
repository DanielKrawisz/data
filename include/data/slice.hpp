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
    template <typename X>
    class slice {
        
        mutable X invalid;
        X* data;
        uint64_t len;
        
        slice(X* x, uint64_t size) : data{x}, len{size} {}

    public:
        slice(uint64_t l, X* d) : invalid(0), data(d), len(l) {} 
        
        slice(std::vector<X>& v) : slice(v.size(), v.data()) {} 
        
        static const slice make(vector<X>& v) {
            return slice(const_cast<std::vector<X>&>(v));
        }
        
        slice() : invalid(0), data(nullptr), len(0) {};
        
        const uint size() const {
            return len;
        }

        X& operator[](uint n) const {
            if (n >= len) return invalid;
            return data[n];
        }

        slice<X> range(uint begin, uint end) const {
            if (begin >= len || end >= len || begin >= end) return slice();

            return slice(data[begin], end - begin);
        };

        slice& operator=(const slice<X> s) {
            invalid = s.invalid;
            data = s.data;
            len = s.len;
            return *this;
        }

        bool operator==(const slice<X>& s) const {
            if (this == &s) return true;
            return data == s.data && len == s.len;
        }

        // a construct which turns a slice into a list. 
        class list {
            constexpr static const ::data::list::definition::list<list, X> is_list{}; 
            
            slice<X>& Slice;
            uint Index;
            
            list(slice<X>& s, uint n) : Slice(s), Index(n) {}
            list(const slice<X>& s, uint n) : Slice(const_cast<slice<X>&>(s)), Index(n) {}
            
        public:
            static list make(slice<X>& s, uint n) {
                return list{s, n};
            }
            
            static const list make(const slice<X>& s, uint n) {
                return list{s, n};
            }
            
            list& operator=(list i) {
                Slice = i.Slice;
                Index = i.Index;
                return *this;
            }
            
            bool empty() const {
                return Index >= Slice.size();
            }
            
            X& first() const {
                if (empty()) return Slice.invalid;
                
                return Slice[Index];
            }
            
            list rest() const {
                if (empty()) return *this;
                
                return {Slice, Index + 1};
            }
        
            bool operator==(const list& s) const {
                return Slice == s.Slice && (Index == s.Index || (empty() && s.empty()));
            }
        
            bool operator!=(const list& s) const {
                return !((*this) == s);
            }
        };
        
        using iterator = index_iterator<slice&, X&>;

        iterator begin() {
            return iterator{*this, uint(0)};
        }
            
        iterator end() {
            return iterator{*this, size()};
        }

        const iterator begin() const {
            return iterator{*const_cast<slice*>(this), uint(0)};
        }
            
        const iterator end() const {
            return iterator{*const_cast<slice*>(this), size()};
        }
            
    };
                
    template <typename X>
    inline bool empty(const typename slice<X>::list l) {
        return l.empty();
    }
                
    template <typename X>
    inline X& first(const typename slice<X>::list l) {
        return l.first();
    }
                
    template <typename X>
    inline const typename slice<X>::list rest(const typename slice<X>::list l) {
        return l.rest();
    }
    
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
        typename slice<X>::iterator It;
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
        const boost::endian::order Endian;
        void bytesIntoIterator(const char*,int);
    public:

        void operator<<(uint16_t) final ;
        void operator<<(uint32_t) final ;
        void operator<<(uint64_t) final ;
        void operator<<(int16_t) final ;
        void operator<<(int32_t) final ;
        void operator<<(int64_t) final ;
        void operator<<(bytes&) final ;
        
        slice_writer(slice<byte> s, boost::endian::order e) : slice_ostream<byte>{s}, Endian{e} {}
        slice_writer(std::vector<byte>& v, boost::endian::order e) : slice_ostream<byte>{v}, Endian{e} {}
    };
    
    class slice_reader : public slice_istream<byte>, public reader {
        const boost::endian::order Endian;
        std::vector<byte>  iteratorToArray(int);
    public:
        void operator>>(uint16_t&) final ;
        void operator>>(uint32_t&) final ;
        void operator>>(uint64_t&) final ;
        void operator>>(int16_t&) final ;
        void operator>>(int32_t&) final ;
        void operator>>(int64_t&) final ;
        void operator>>(std::vector<byte>&) final ;
        
        slice_reader(slice<byte> s, boost::endian::order e) : slice_istream<byte>{s}, Endian{e} {}
        slice_reader(std::vector<byte>& v, boost::endian::order e) : slice_istream<byte>{v}, Endian{e} {}
    };

}

#endif
