// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_CYCLE
#define DATA_TOOLS_CYCLE

#include <data/functional/list.hpp>
    
namespace data::tool {
    
    template <List list, typename X> requires SequenceOf<list, X>
    struct cycle {
        
        list Cycle;
        
        bool valid () const;
        
        cycle ();
        explicit cycle (list l);
        cycle (std::initializer_list<X> x);
        
        cycle &operator = (const cycle &c);
        
        size_t size () const;
        const X &head () const;
        cycle insert (const X &x) const;
        cycle reverse () const;
        
        cycle rotate_left () const;
        cycle rotate_right () const;
        cycle rotate_left (uint32 n) const;
        cycle rotate_right (uint32 n) const;
        
        cycle remove () const;
        cycle remove (uint32 n) const;
        
        bool operator == (const cycle &) const;
        bool operator != (const cycle &c) const;
    };
    
    template <List list, typename X> requires SequenceOf<list, X>
    std::ostream &operator << (std::ostream &o, const cycle<list, X> n) {
        return o << "cycle" << n.Cycle;
    }
    
    template <List list, typename X> requires SequenceOf<list, X>
    cycle<list, X> cycle<list, X>::remove () const {
        size_t s = size ();
        if (s == 0) return cycle {};
        return {Cycle.rest ()};
    }
    
    template <List list, typename X> requires SequenceOf<list, X>
    cycle<list, X> cycle<list, X>::remove (uint32 n) const {
        if (n == 0) return *this;
        size_t s = size ();
        if (s > n) return cycle {};
        return remove (n - 1).remove ();
    }
    
    template <List list, typename X> requires SequenceOf<list, X>
    bool cycle<list, X>::operator == (const cycle &c) const {
        size_t s = size ();
        if (s != c.size ()) return false;
        
        if (s == 0) return true;
        
        cycle x = c;
        for (int i = 0; i < s; i++) {
            if (Cycle == x.Cycle) return true;
            x = x.rotate_left ();
        }
        
        return false;
    }
    
    template <List list, typename X> requires SequenceOf<list, X>
    inline bool cycle<list, X>::operator != (const cycle &c) const {
        return !operator == (c);
    }
    
    template <List list, typename X> requires SequenceOf<list, X>
    inline bool cycle<list, X>::valid () const {
        return data::valid (Cycle);
    }
    
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X>::cycle () : Cycle {} {}
    
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X>::cycle (list l) : Cycle {l} {}
    
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X>::cycle (std::initializer_list<X> x) : Cycle {} {
        for (X e : x) Cycle = Cycle << e;
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> &cycle<list, X>::operator = (const cycle &c) {
        Cycle = c.Cycle;
        return *this;
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline size_t cycle<list, X>::size () const {
        return data::size (Cycle);
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline const X& cycle<list, X>::head () const {
        return Cycle.first ();
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> cycle<list, X>::insert (const X &x) const {
        return cycle {Cycle.insert (x)};
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> cycle<list, X>::reverse () const {
        return cycle {data::reverse (Cycle)};
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> cycle<list, X>::rotate_left () const {
        return cycle {data::rotate_left (Cycle)};
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> cycle<list, X>::rotate_right () const {
        return cycle {data::rotate_right (Cycle)};
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> cycle<list, X>::rotate_left (uint32 n) const {
        return cycle {data::rotate_left (Cycle, n)};
    }
        
    template <List list, typename X> requires SequenceOf<list, X>
    inline cycle<list, X> cycle<list, X>::rotate_right (uint32 n) const {
        return cycle {data::rotate_right (Cycle, n)};
    }
}

#endif

