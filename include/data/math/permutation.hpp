// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_PERMUTATION
#define DATA_MATH_PERMUTATION

#include <list>
#include <data/for_each.hpp>

#include <data/math/arithmetic.hpp>
#include <data/math/associative.hpp>
#include <data/math/group.hpp>

namespace data::math {

    template <std::totally_ordered elem> 
    struct permutation {
        using replacements = std::list<entry<elem, elem>>;
        
        static replacements compose (replacements, replacements);

        using cycle = data::cycle<elem>;

        static bool valid (const cycle &);
        static cycle inverse (const cycle &);
        static set<elem> elements (const cycle &);
        static math::sign sign (const cycle &);
        static replacements as_replacements (const cycle &);

        // the identity cycle is the same as all cycles
        // consisting of repetitions of a single element.
        static cycle normalize (const cycle &);
        
        list<cycle> Cycles;
        explicit permutation (replacements);
        
        explicit operator replacements () const {
            if (Cycles.empty ()) return replacements {};
            auto x = replacements ();
            for (const auto &c : Cycles) x.merge (as_replacements (c));
            return x;
        }
        
        permutation () : Cycles {} {}
        
        permutation (std::initializer_list<data::cycle<elem>> cx) : permutation {} {
            for (const auto &x: cx) Cycles = Cycles << cycle (x);
            *this = normalize ();
        }
        
        bool valid () const;
        
        math::sign sign () const;
        
        static permutation identity () {
            return permutation ();
        }
        
        permutation normalize () const;
        
        permutation inverse () const;
        
        set<elem> elements () const {
            return data::fold<set<elem>> ([] (set<elem> x, const cycle &c) -> set<elem> {
                return x & c.elements ();
            }, {}, Cycles);
        }
        
        static bool commute (const cycle &a, const cycle &b) {
            return (a.elements () | b.elements ()) == set<elem> {};
        }
        
        static bool commute (const permutation &a, const permutation &b) {
            return (a.elements () | b.elements ()) == set<elem> {};
        }
        
        permutation operator * (const permutation& p) const {
            return permutation (compose (replacements (*this), replacements (p)));
        }
        
        bool operator == (const permutation &p) const;
        
        bool operator != (const permutation &p) const;
        
    private:
        explicit permutation (list<cycle> c) : Cycles {c} {}
        
    };

    template <typename elem>
    std::ostream inline &operator << (std::ostream &o, const permutation<elem> &m) {
        return o << "permutation" << m.Cycles;
    }

    // Declare associativity and commutivity of operators + and * on N. 
    template <typename elem> struct associative<times<permutation<elem>>, permutation<elem>> {};
    
    template <typename elem> struct identity<times<permutation<elem>>, permutation<elem>> {
        static const permutation<elem> value () {
            return permutation<elem>::identity ();
        }
    }; 
    
    template <typename elem> 
    permutation<elem>::replacements permutation<elem>::compose (replacements a, replacements b) {
        
        if (a.empty ()) return b;
        if (b.empty ()) return a;
        
        replacements x;
        replacements br = b;
        
        for (auto ai = a.begin (); ai != a.end (); ai++) {
            auto bi = br.begin ();
            while (true) {
                if (bi == br.end ()) {
                    x.push_back (*ai);
                    break;
                }
                
                if (bi->Key == ai->Value) {
                    if (bi->Value != ai->Key) x.push_back (entry {ai->Key, bi->Value});
                    br.erase (bi);
                    break;
                }
                
                bi++;
            }
        }
        for (auto bi = br.begin (); bi != br.end (); bi++) x.push_back (*bi);
        return x;
    }

    template <typename elem>
    permutation<elem>::cycle inline permutation<elem>::inverse (const permutation<elem>::cycle &c) {
        return c.reverse ();
    }

    template <typename elem>
    set<elem> inline permutation<elem>::elements (const permutation<elem>::cycle &c) {
        return set<elem> {c.Cycle};
    }
    
    template <typename elem> 
    permutation<elem>::cycle permutation<elem>::normalize (const permutation<elem>::cycle &c) {
        if (c.size () == 0 || c.size () == 1) return cycle {};
        
        elem first = c.Cycle.first ();
        list<elem> rest = c.Cycle.rest ();
        while (!rest.empty ()) if (first != rest.first ()) return c;
        else rest = rest.rest ();
        
        return cycle {};
    }
    
    template <typename elem> 
    permutation<elem>::replacements permutation<elem>::as_replacements (const cycle &c) {
        if (c.Cycle.empty ()) return replacements {};
        auto a = c.Cycle.first ();
        auto b = a;
        auto r = c.Cycle.rest ();
        replacements x {};
        // r will not be empty because this cycle is normalized. 
        do {
            x.push_back (entry<elem, elem> {b, r.first ()});
            b = r.first ();
            r = r.rest ();
        } while (!r.empty ());
        x.push_back (entry {b, a});
        return x;
    }
    
    template <typename elem> 
    permutation<elem>::permutation (replacements x): permutation {} {
        replacements r = x;
        
        while (r.size () > 0) {
            cycle z {};
            elem first = r.front ().Key;
            elem last = r.front ().Value;
            r.pop_front ();
            z.Cycle = z.Cycle << first << last;
            
            while (true) {
                auto i = r.begin ();
                while (i != r.end ()) {
                    if (i->Key == last) {
                        last = i->Value;
                        r.erase (i++);
                        if (last == first) goto out;
                        z.Cycle = z.Cycle << last;
                    } else i++;
                }
            }
            
            out: 
            
            Cycles = Cycles << z;
        }
    }
    
    template <typename elem>    
    permutation<elem> inline
    permutation<elem>::normalize () const {
        permutation p {};
        for (const auto &c : Cycles) if (normalize (c) != cycle {}) p.Cycles = p.Cycles << c;
        return p;
    }
    
    template <typename elem>     
    bool permutation<elem>::valid () const {
        set<elem> elements {};
        for (const auto &c : Cycles) for (const auto &v : c.Cycle) if (elements.contains (v)) return false;
        else elements = elements.insert (v);
        return true;
    }
    
    template <typename elem> 
    permutation<elem> inline
    permutation<elem>::inverse () const {
        return permutation {for_each ([] (const cycle c) -> cycle {
            return permutation<elem>::inverse (c);
        }, normalize ().Cycles)};
    }
    
    template <typename elem> 
    bool permutation<elem>::operator == (const permutation &p) const {
        if (Cycles.size () != p.Cycles.size ()) return false;
        if (Cycles.size () == 0) return true;
        return permutation {} == operator * (p.inverse ());
    }
    
    template <typename elem> 
    inline bool permutation<elem>::operator != (const permutation& p) const {
        return !operator == (p);
    }
    
    template <typename elem> 
    math::sign permutation<elem>::sign () const {
        if (!valid ()) return math::zero;
        return data::fold<math::sign> ([] (math::sign x, const cycle& c) -> math::sign {
            return x * permutation<elem>::sign (c);
        }, math::positive, Cycles);
    }
    
    template <typename elem> 
    math::sign permutation<elem>::sign (const cycle &c) {
        if (!c.valid ()) return math::zero;
        size_t nx = normalize (c).size ();
        return nx == 0 ? math::positive : nx % 2 == 0 ? math::negative : math::positive; 
    }
    
    template <typename elem> 
    bool permutation<elem>::valid (const cycle &c) {
        if (!c.valid ()) return false;
        set<elem> el = elements ();
        return el.size () == c.size () || el.size () == 1;
    }
}

#endif

