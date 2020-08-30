// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_PERMUTATION
#define DATA_MATH_PERMUTATION

#include <data/tools.hpp>

#include <data/math/arithmetic.hpp>
#include <data/math/associative.hpp>

namespace data::math {

    template <typename elem>
    struct permutation {
        struct cycle : data::cycle<elem> {
            using data::cycle<elem>::cycle;
            
            cycle(const data::cycle<elem>& c) : data::cycle<elem>{c} {}
            
            bool valid() const;
        
            elem operator*(const elem e) const;
        
            permutation operator*(const cycle& c) const {
                throw method::unimplemented{"cycle * cycle"};
            }
            
            cycle inverse() const {
                return {data::cycle<elem>::reverse()};
            }
            
            bool operator==(const cycle& c) const {
                return data::cycle<elem>::operator==(c);
            }
            
            bool operator!=(const cycle& c) const {
                return data::cycle<elem>::operator!=(c);
            }
            
            set<elem> elements() const {
                auto x = set<elem>{data::cycle<elem>::Cycle};
                if (x.size() == 0) return set<elem>{};
                return x;
            }
            
            // the identity cycle is the same as all cycles
            // consisting of repetitions of a single element.
            cycle normalize() const;
        
            permutation operator*(const permutation& p) const {
                throw method::unimplemented{"cycle * perm"};
            }
        };
        
        list<cycle> Cycles;
        
        permutation() : Cycles{} {}
        permutation(std::initializer_list<cycle> x);
        
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
        
        elem operator*(const elem& e) const;
        
        permutation operator*(const permutation& p) const;
        
        permutation operator*(const cross<elem>& v) const;
        
        bool operator==(const permutation& p) const;
        
        bool operator!=(const permutation& p) const;
        
    private:
        permutation(list<cycle> c) : Cycles{c} {}
        
        permutation operator*(const cycle& p) const;
    };
    
}

template <typename elem>
std::ostream& operator<<(std::ostream& o, const data::math::permutation<elem>& m) {
    return o << "permutation" << m.Cycles;
}

// Declare associativity and commutivity of operators + and * on N. 
namespace data::math {
    template <typename elem> struct associative<times<permutation<elem>>, permutation<elem>> {};
    
    template <typename elem> struct identity<times<permutation<elem>>, permutation<elem>> {
        static const permutation<elem> value() {
            return permutation<elem>::identity();
        }
    }; 
    
    template <typename elem> 
    bool permutation<elem>::cycle::valid() const {
        if (!data::cycle<elem>::valid()) return false;
        set<elem> el = elements();
        return el.size() == data::cycle<elem>::size() || el.size() == 1;
    }
    
    template <typename elem> 
    elem permutation<elem>::cycle::operator*(const elem e) const {
        list<elem> c = data::cycle<elem>::Cycle;
        while (!c.empty()) {
            elem a = c.first();
            list<elem> c = c.rest();
            if (e == a) return c.empty() ? data::cycle<elem>::Cycle.first() : c.first();
        }
        return e;
    }
    
    template <typename elem> 
    typename permutation<elem>::cycle
    permutation<elem>::cycle::normalize() const {
        if (data::cycle<elem>::size() == 0 || data::cycle<elem>::size() == 1) return cycle{};
        
        elem first = data::cycle<elem>::Cycle.first();
        list<elem> rest = data::cycle<elem>::Cycle.rest();
        while (!rest.empty()) if (first != data::cycle<elem>::Cycle.first()) return *this;
        else rest = rest.rest();
        
        return cycle{};
    }
    
    template <typename elem> 
    permutation<elem>::permutation(std::initializer_list<cycle> x) : Cycles{} {
        permutation p{};
        for (cycle c : x) p = p * c;
        Cycles = p.Cycles;
    } 
    
    template <typename elem>     
    bool permutation<elem>::valid() const {
        set<elem> elements{};
        list<cycle> cycles = Cycles;
        while (!cycles.empty()) {
            cycle c = cycles.first().normalize();
            cycles = cycles.rest();
            if (!c.valid()) return false;
            
            list<elem> e = c.Cycle;
            while (!e.empty()) {
                elem x = e.first();
                if (elements.contains(x)) return false;
                elements = elements.insert(x);
            }
        }
        return true;
    }
    
    template <typename elem>    
    inline permutation<elem> 
    permutation<elem>::normalize() const {
        return for_each([](const cycle c) -> cycle {
            return c.normalize();
        }, Cycles);
    }
    
    template <typename elem> 
    inline permutation<elem> 
    permutation<elem>::inverse() const {
        return for_each([](const cycle c) -> cycle {
            return c.inverse();
        }, Cycles);
    }
        
    template <typename elem> 
    elem permutation<elem>::operator*(const elem& e) const {
        list<cycle> apply = Cycles;
        while (!apply.empty()) {
            cycle c = apply.first();
            elem a = c * e;
            if (a != e) return a;
        }
        return e;
    }
    
    template <typename elem> 
    permutation<elem> 
    permutation<elem>::operator*(const permutation& p) const {
        permutation left{this->normalize()};
        if (left.Cycles.empty()) return p;
        
        permutation right = p.normalize();
        if (right.Cycles.empty()) return left;
        
        if (commute(left, right)) return permutation{left.Cycles << right.Cycles};
        
        while (!right.Cycles.empty()) {
            left = left * right.Cycles.first();
            right.Cycles = right.Cycles.rest();
        }
        
        return left;
    }
        
    template <typename elem> 
    inline permutation<elem> 
    permutation<elem>::operator*(const cross<elem>& v) const {
        cross<elem> output(v.size());
        for(int i = 0; i < v.size(); ++i) output[i] = operator*(v[i]);
        return output;
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
    
    template <typename elem> 
    permutation<elem> 
    permutation<elem>::operator*(const cycle& c) const {
        list<cycle> cycles = Cycles;
        if (!cycles.empty()) return permutation{c};
        
        permutation p = cycles.first() * c;
        cycles = cycles.rest();
        while (!cycles.empty()) {
            cycle x = cycles.first();
            p = x * p;
            cycles = cycles.rest();
        }
        return p;
    }
}

#endif

