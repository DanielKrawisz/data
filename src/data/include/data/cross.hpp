// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CROSS
#define DATA_CROSS

#include <data/sequence.hpp>
#include <data/slice.hpp>
#include <data/valid.hpp>
#include <data/stream.hpp>

namespace data {
    
    // The cartesian product. 
    // it is the same as a vector with some slight improvements. 
    template <typename X> struct cross : std::vector<X> {
        constexpr cross ();
        explicit cross (size_t size);
        explicit cross (size_t size, X fill);
        
        cross (std::initializer_list<X> x);
        
        template<sequence list>
        explicit cross (list l);
        
        bool valid () const;
        
        X &operator [] (int i);
        
        const X &operator [] (int i) const;
        
        explicit operator slice<X> ();
        explicit operator slice<const X> () const;
        
        slice<X> range (int);
        slice<X> range (int, int);

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;
        
        cross (std::vector<X> &&v) : std::vector<X> {v} {}
        
    protected:
        void fill (const X &x) {
            for (X& z : *this) z = x;
        }
        
    };
    
    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s);

    template <typename X>
    X &cross<X>::operator [] (int i) {
        size_t size = this->size ();
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::vector<X>::operator [] (i);
    }

    template <typename X>
    bool cross<X>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

    template <typename X>
    const X &cross<X>::operator [] (int i) const {
        size_t size = this->size ();
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::vector<X>::operator [] (i);
    }
    
    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s) {
        auto b = s.begin ();
        o << "[";
        while (true) {
            if (b == s.end ()) return o << "]";
            else if (b != s.begin ()) o << ", ";
            o << *b;
            b++;
        }
    }
    
    template <typename X>
    constexpr inline cross<X>::cross () : std::vector<X> {} {}
    
    template <typename X>
    inline cross<X>::cross (size_t size) : std::vector<X> (size) {}
    
    template <typename X>
    inline cross<X>::cross (size_t size, X fill) : std::vector<X> (size) {
        for (auto it = std::vector<X>::begin (); it < std::vector<X>::end (); it++) *it = fill;
    }
    
    template <typename X>
    inline cross<X>::cross (std::initializer_list<X> x) : std::vector<X> {x} {}
    
    template <typename X>
    template<sequence list>
    cross<X>::cross (list l) : cross {} {
        std::vector<X>::resize (data::size (l));
        auto b = std::vector<X>::begin ();
        while (!l.empty ()) {
            *b = l.first ();
            l = l.rest ();
            b++;
        }
    }
    
    template <typename X>
    inline cross<X>::operator slice<X> () {
        return slice<X> (static_cast<std::vector<X> &> (*this));
    }
    
    template <typename X>
    inline slice<X> cross<X>::range (int e) {
        return operator slice<X> ().range (e);
    }
    
    template <typename X>
    inline slice<X> cross<X>::range (int b, int e) {
        return operator slice<X> ().range (e);
    }
    
}

#endif


