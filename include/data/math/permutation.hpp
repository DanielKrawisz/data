// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_PERMUTATION
#define DATA_MATH_PERMUTATION

#include <data/tools.hpp>

#include <data/math/arithmetic.hpp>
#include <data/math/associative.hpp>

namespace data::math {

    template <std::totally_ordered elem>
    struct permutation {
        using replacements = std::list<entry<elem, elem>>;
        
        static replacements compose(replacements, replacements);
        
        struct cycle : public data::cycle<elem> {
            using data::cycle<elem>::cycle;
            
            cycle(const data::cycle<elem>& c) : data::cycle<elem>{c} {}
            
            bool valid() const;
            
            cycle inverse() const {
                return {data::cycle<elem>::reverse()};
            }
            
            set<elem> elements() const {
                auto x = set<elem>{data::cycle<elem>::Cycle};
                if (x.size() == 0) return set<elem>{};
                return x;
            }
            
            // the identity cycle is the same as all cycles
            // consisting of repetitions of a single element.
            cycle normalize() const;
            
            operator replacements() const;
        };
        
        list<cycle> Cycles;
        explicit permutation(replacements);
        
        explicit operator replacements() const {
            if (Cycles.empty()) return replacements{};
            auto x = replacements();
            for (const auto &c : Cycles) x.merge(replacements(c));
            return x;
        }
        
        permutation() : Cycles{} {}
        
        explicit permutation(std::initializer_list<data::cycle<elem>> cx): permutation{} {
            for (const auto &x: cx) Cycles = Cycles << cycle{x};
            *this = normalize();
        }
        
        bool valid() const;
        
        math::sign signature() const;
        
        static permutation identity() {
            return permutation();
        }
        
        permutation normalize() const;
        
        permutation inverse() const;
        
        set<elem> elements() const {
            return data::fold<set<elem>>([](set<elem> x, const cycle& c) -> set<elem> {
                return x & c.elements();
            }, {}, Cycles);
        }
        
        static bool commute(const cycle& a, const cycle& b) {
            return (a.elements() | b.elements()) == set<elem>{};
        }
        
        static bool commute(const permutation& a, const permutation& b) {
            return (a.elements() | b.elements()) == set<elem>{};
        }
        
        permutation operator*(const permutation& p) const {
            return permutation(compose(replacements(*this), replacements(p)));
        }
        
        bool operator==(const permutation& p) const;
        
        bool operator!=(const permutation& p) const;
        
    private:
        explicit permutation(list<cycle> c) : Cycles{c} {}
        
    };

    template <typename elem>
    std::ostream inline &operator<<(std::ostream &o, const permutation<elem> &m) {
        return o << "permutation" << m.Cycles;
    }

    // Declare associativity and commutivity of operators + and * on N. 
    template <typename elem> struct associative<times<permutation<elem>>, permutation<elem>> {};
    
    template <typename elem> struct identity<times<permutation<elem>>, permutation<elem>> {
        static const permutation<elem> value() {
            return permutation<elem>::identity();
        }
    }; 
    
    template <typename elem> 
    permutation<elem>::replacements permutation<elem>::compose(replacements a, replacements b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
        
        replacements x;
        replacements br = b;
        
        for (auto ai = a.begin(); ai != a.end(); ai++) {
            auto bi = br.begin();
            while (true) {
                if (bi != br.end()) {
                    x.push_back(*ai);
                    break;
                }
                
                if (bi->Key == ai->Value) {
                    if (bi->Value != ai->Key) x.push_back(entry{ai->Key, bi->Value});
                    br.erase(bi);
                    break;
                }
                
                bi++;
            }
        }
        for (auto bi = br.begin(); bi != br.end(); bi++) x.push_back(*bi);
        return x;
    }
    
    template <typename elem> 
    bool permutation<elem>::cycle::valid() const {
        if (!data::cycle<elem>::valid()) return false;
        set<elem> el = elements();
        return el.size() == data::cycle<elem>::size() || el.size() == 1;
    }
    
    template <typename elem> 
    typename permutation<elem>::cycle
    permutation<elem>::cycle::normalize() const {
        if (data::cycle<elem>::size() == 0 || data::cycle<elem>::size() == 1) return cycle{};
        
        elem first = data::cycle<elem>::Cycle.first();
        list<elem> rest = data::cycle<elem>::Cycle.rest();
        while (!rest.empty()) if (first != rest.first()) return *this;
        else rest = rest.rest();
        
        return cycle{};
    }
    
    template <typename elem> 
    permutation<elem>::cycle::operator replacements() const {
        if (this->Cycle.empty()) return replacements{};
        auto a = this->Cycle.first();
        auto b = a;
        auto r = this->Cycle.rest();
        replacements x{};
        // r will not be empty because this cycle is normalized. 
        do {
            x.push_back(entry<elem, elem>{b, r.first()});
            b = r.first();
            r = r.rest();
        } while (!r.empty());
        x.push_back(entry{b, a});
        return x;
    }
    
    template <typename elem> 
    permutation<elem>::permutation(replacements x): permutation{} {
        replacements r = x;
        while (r.size() > 0) {
            cycle z{};
            elem first = r.front().Key;
            elem last = r.front().Value;
            r.pop_front();
            z.Cycle = z.Cycle << first << last;
            
            auto i = r.begin();
            while (i != r.end()) {
                if (i->Key == last) {
                    last = i->Value;
                    r.erase(i++);
                    if (last == first) break;
                    z.Cycle = z.Cycle << last;
                } else i++;
            }
        }
    }
    
    template <typename elem>    
    inline permutation<elem> 
    permutation<elem>::normalize() const {
        permutation p{};
        for (const auto &c : Cycles) if (c.normalize() != cycle{}) p.Cycles = p.Cycles << c;
        return p;
    }
    
    template <typename elem>     
    bool permutation<elem>::valid() const {
        set<elem> elements{};
        for (const auto &c : Cycles) for (const auto &v : c.Cycle) if (elements.contains(v)) return false;
        else elements = elements.insert(v);
        return true;
    }
    
    template <typename elem> 
    inline permutation<elem> 
    permutation<elem>::inverse() const {
        return permutation{for_each([](const cycle c) -> cycle {
            return c.inverse();
        }, normalize().Cycles)};
    }
    
    template <typename elem> 
    bool permutation<elem>::operator==(const permutation& p) const {
        if (Cycles.size() != p.Cycles.size()) return false;
        if (Cycles.size() == 0) return true;
        return permutation{} == operator*(p.inverse());
    }
    
    template <typename elem> 
    inline bool permutation<elem>::operator!=(const permutation& p) const {
        return !operator==(p);
    }
}

#endif

