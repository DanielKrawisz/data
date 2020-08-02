// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_CYCLE
#define DATA_TOOLS_CYCLE

#include <data/tools.hpp>
    
namespace data::tool {
    
    template <typename X>
    struct cycle {
        list<X> Cycle;
        
        bool valid() const {
            return data::valid(Cycle);
        }
            
        cycle() : Cycle{} {}
        
        explicit cycle(list<X> l) : Cycle{l} {}
        
        cycle(std::initializer_list<X> x) : Cycle{} {
            for (X e : x) Cycle = Cycle << e;
        }
        
        cycle& operator=(const cycle& c) {
            Cycle = c.Cycle;
            return *this;
        }
        
        size_t size() const {
            return Cycle.size();
        }
        
        const X& head() const {
            return Cycle.first();
        }
        
        cycle insert(const X& x) const {
            return cycle{Cycle.insert(x)};
        }
        
        cycle reverse() const {
            return cycle{data::reverse(Cycle)};
        }
        
        cycle rotate_left() const {
            return cycle{functional::list::rotate_left(Cycle)};
        }
        
        cycle rotate_right() const {
            return cycle{functional::list::rotate_right(Cycle)};
        }
        
        cycle rotate_left(uint32 n) const {
            return cycle{functional::list::rotate_left(Cycle, n)};
        }
        
        cycle rotate_right(uint32 n) const {
            return cycle{functional::list::rotate_right(Cycle, n)};
        }
        
        cycle remove() const {
            size_t s = size();
            if (s == 0) return cycle{}; 
            return {Cycle.rest()};
        }
        
        cycle remove(uint32 n) const {
            if (n == 0) return *this;
            size_t s = size();
            if (s > n) return cycle{}; 
            return remove(n - 1).remove();
        }
        
        bool operator==(const cycle&) const;
        
        bool operator!=(const cycle& c) const {
            return !operator==(c);
        }
    };
    
}

template <typename X> 
std::ostream& operator<<(std::ostream& o, const data::tool::cycle<X> n) {
    return o << "cycle" << n.Cycle;
}
    
namespace data::tool {
    
    template <typename X>
    bool cycle<X>::operator==(const cycle& c) const {
        size_t s = size();
        if (s != c.size()) return false;
        
        if (s == 0) return true;
        
        cycle x = c;
        for (int i = 0; i < s; i++) {
            if (Cycle == x.Cycle) return true;
            x = x.rotate_left();
        }
        
        return false;
    }
}

#endif

