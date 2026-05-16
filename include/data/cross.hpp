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
    
    // Cross (for cross product) is an iterable fixed-size container of any size.
    // Cross resembles a vector with the ability to resize removed.
    // Other improvements include: being able to index from the end with
    // negative numbers as well as from the beginning.
    // TODO: intead of using a vector, we need something
    // that will lazily initilize the pointer so that
    // the empty cross can be used in a constexpr expression.
    template <std::default_initializable X> struct cross : std::vector<wrapped<X>> {
        constexpr cross ();
        explicit cross (size_t size);
        explicit cross (size_t size, X fill);
        
        cross (std::initializer_list<X> x);
        
        template<Sequence list>
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
    cross<X> drop (const cross<X> &x, size_t size) {
        cross<X> n;
        if (size >= x.size ()) return n;
        n.resize (x.size () - size);
        auto i = x.begin () + size;
        for (X &z : n) {
            z = *i;
            i++;
        }
        return n;
    }

    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s);

    template <std::default_initializable X>
    X &cross<X>::operator [] (int i) {
        size_t size = this->size ();
        if (size == 0) throw out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::vector<X>::operator [] (i);
    }

    template <std::default_initializable X>
    bool cross<X>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

    template <std::default_initializable X>
    const X &cross<X>::operator [] (int i) const {
        size_t size = this->size ();
        if (size == 0) throw out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::vector<X>::operator [] (i);
    }
    
    template <std::default_initializable X>
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
    
    template <std::default_initializable X>
    constexpr inline cross<X>::cross () : std::vector<X> {} {}
    
    template <std::default_initializable X>
    inline cross<X>::cross (size_t size) : std::vector<X> (size) {}
    
    template <std::default_initializable X>
    inline cross<X>::cross (size_t size, X fill) : std::vector<X> (size) {
        for (auto it = std::vector<X>::begin (); it < std::vector<X>::end (); it++) *it = fill;
    }
    
    template <std::default_initializable X>
    inline cross<X>::cross (std::initializer_list<X> x) : std::vector<X> {x} {}
    
    template <std::default_initializable X>
    template<Sequence list>
    cross<X>::cross (list l) : cross {} {
        std::vector<X>::resize (data::size (l));
        auto b = std::vector<X>::begin ();
        while (!l.empty ()) {
            *b = l.first ();
            l = l.rest ();
            b++;
        }
    }
    
    template <std::default_initializable X>
    inline cross<X>::operator slice<X> () {
        return slice<X> (static_cast<std::vector<X> &> (*this));
    }
    
    template <std::default_initializable X>
    inline slice<X> cross<X>::range (int e) {
        return operator slice<X> ().range (e);
    }
    
    template <std::default_initializable X>
    inline slice<X> cross<X>::range (int b, int e) {
        return operator slice<X> ().range (e);
    }
    
}

#endif


