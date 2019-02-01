#ifndef DATA_SLICE_HPP
#define DATA_SLICE_HPP

#include "list.hpp"

namespace data
{

    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X>
    class slice {
        
        mutable X invalid;
        X* data;
        uint64_t len; 
        
        slice(X* x, uint64_t size) : data{x}, len{size} {}

    public:
        slice(std::vector<X>& v) : invalid(0), data(v.data()), len(v.size()) {};
        
        static const slice make(sequence<X>& v) {
            return slice(const_cast<std::vector<X>&>(v));
        }
        
        slice() : invalid(0), data(nullptr), len(0) {};
        
        const N size() const {
            return len;
        }

        X& operator[](N n) const {
            if (n >= len) return invalid;
            return data[n];
        }

        slice<X> range(N begin, N end) const {
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
            if (data == s.data && len == s.len) return true;
            return false;
        }

        // a construct which turns a slice into a list. 
        class list {
            constexpr static const ::data::list::definition::list<list, X> is_list{}; 
            
            slice<X>& Slice;
            N Index;
            
            list(slice<X>& s, N n) : Slice(s), Index(n) {}
            list(const slice<X>& s, N n) : Slice(const_cast<slice<X>&>(s)), Index(n) {}
            
        public:
            static list make(slice<X>& s, N n) {
                return list{s, n};
            }
            
            static const list make(const slice<X>& s, N n) {
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
        
        using iterator = data::list::iterator<list, X>;

        iterator begin() {
            return iterator{list::make(*this, N(0))};
        }
            
        iterator end() {
            return iterator{list::make(*this, size())};
        }

        const iterator begin() const {
            return iterator{list::make(*this, N(0))};
        }
            
        const iterator end() const {
            return iterator{list::make(*this, size())};
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

}

#endif


