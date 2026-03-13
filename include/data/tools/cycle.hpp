// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_CYCLE
#define DATA_TOOLS_CYCLE

#include <data/functional/list.hpp>
#include <data/reverse.hpp>
    
namespace data::tool {

    template <typename X, List<X> list> struct cycle;

    template <typename X, List<X> list> bool operator == (const cycle<X, list> &, const cycle<X, list> &);

    template <typename X, List<X> list> bool empty (cycle<X, list>);
    template <typename X, List<X> list> size_t size (cycle<X, list>);
    
    template <typename X, List<X> list> struct cycle {
        
        list Cycle;
        
        bool valid () const;
        
        cycle ();
        explicit cycle (list l);
        cycle (std::initializer_list<wrapped<X>> x);
        
        size_t size () const;
        const X &head () const;
        cycle insert (inserted<X> x) const;
        cycle reverse () const;
        
        cycle rotate_left () const;
        cycle rotate_right () const;
        cycle rotate_left (uint32 n) const;
        cycle rotate_right (uint32 n) const;
        
        cycle remove () const;
        cycle remove (uint32 n) const;
    };
    
    template <typename X, List<X> list>
    std::ostream &operator << (std::ostream &o, const cycle<X, list> n) {
        return o << "cycle" << n.Cycle;
    }

    template <typename X, List<X> list> bool empty (cycle<X, list> x) {
        return empty (x.Cycle);
    }

    template <typename X, List<X> list> size_t size (cycle<X, list> x) {
        return size (x.Cycle);
    }
    
    template <typename X, List<X> list>
    cycle<X, list> cycle<X, list>::remove () const {
        size_t s = size ();
        if (s == 0) return cycle {};
        return {Cycle.rest ()};
    }
    
    template <typename X, List<X> list>
    cycle<X, list> cycle<X, list>::remove (uint32 n) const {
        if (n == 0) return *this;
        size_t s = size ();
        if (s > n) return cycle {};
        return remove (n - 1).remove ();
    }
    
    template <typename X, List<X> list>
    bool operator == (const cycle<X, list> &a, const cycle<X, list> &b) {
        size_t size = a.size ();
        if (size != b.size ()) return false;
        
        if (size == 0) return true;
        
        cycle<X, list> x = b;
        for (int i = 0; i < size; i++) {
            if (a.Cycle == x.Cycle) return true;
            x = x.rotate_left ();
        }
        
        return false;
    }
    
    template <typename X, List<X> list>
    inline bool cycle<X, list>::valid () const {
        return data::valid (Cycle);
    }
    
    template <typename X, List<X> list>
    inline cycle<X, list>::cycle () : Cycle {} {}
    
    template <typename X, List<X> list>
    inline cycle<X, list>::cycle (list l) : Cycle {l} {}
    
    template <typename X, List<X> list>
    inline cycle<X, list>::cycle (std::initializer_list<wrapped<X>> x) : Cycle {} {
        for (X e : x) Cycle = Cycle << e;
    }
        
    template <typename X, List<X> list>
    inline size_t cycle<X, list>::size () const {
        return data::size (Cycle);
    }

    template <typename X, List<X> list>
    const X inline &cycle<X, list>::head () const {
        return Cycle.first ();
    }

    template <typename X, List<X> list>
    inline cycle<X, list> cycle<X, list>::insert (inserted<X> x) const {
        return cycle {Cycle.insert (x)};
    }

    template <typename X, List<X> list>
    inline cycle<X, list> cycle<X, list>::reverse () const {
        return cycle {data::reverse (Cycle)};
    }

    template <typename X, List<X> list>
    inline cycle<X, list> cycle<X, list>::rotate_left () const {
        return cycle {data::rotate_left (Cycle)};
    }

    template <typename X, List<X> list>
    inline cycle<X, list> cycle<X, list>::rotate_right () const {
        return cycle {data::rotate_right (Cycle)};
    }

    template <typename X, List<X> list>
    inline cycle<X, list> cycle<X, list>::rotate_left (uint32 n) const {
        return cycle {data::rotate_left (Cycle, n)};
    }

    template <typename X, List<X> list>
    inline cycle<X, list> cycle<X, list>::rotate_right (uint32 n) const {
        return cycle {data::rotate_right (Cycle, n)};
    }
}

#endif

