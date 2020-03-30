// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_PERMUTATION
#define DATA_MATH_PERMUTATION

#include <data/types.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/tools/map_set.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/associative.hpp>

namespace data::math {

    template <typename elem>
    struct permutation {
        struct cycle {
            tool::functional_queue<elem> Cycle;
            cycle(std::initializer_list<elem>);
        private:
            cycle(tool::functional_queue<elem> c) : Cycle{c} {}
            friend struct permutation<elem>;
        };
        
        tool::functional_queue<cycle> Cycles;
        
        permutation();
        permutation(std::initializer_list<cycle>);
        
        bool valid() const {
            tool::map_set<elem> elements = {};
            for (cycle c : Cycles) {
                if (c.empty()) return false;
                for (elem x : c) {
                    if (elements.contains(x)) return false;
                    elements = elements.insert(x);
                }
            }
            return true;
        }
        
        static permutation identity() {
            return permutation();
        }
        
        permutation inverse() const {
            return for_each([](const cycle c) -> cycle {
                return {functional::stack::reverse(c.Cycle)};
            }, Cycles);
        }
        
        permutation operator*(const elem e) const {
            tool::functional_queue<cycle> apply = Cycles;
            while (!apply.empty()) {
                tool::functional_queue<elem> cycle = apply.first();
                while (!cycle.empty()) {
                    elem a = cycle.first();
                    cycle = cycle.rest();
                    if (e == a) return cycle.empty() ? apply.first().first() : cycle.first();
                }
            }
            return e;
        }
        
        permutation operator*(const cycle c) const {
            tool::functional_queue<elem> output{};
            tool::functional_queue<elem> input = c.Cycle;
            while (!input.empty()) {
                output = output << operator*(input.first());
                input = input.rest();
            }
            return {output};
        }
        
        permutation operator*(const permutation p) const {
            tool::functional_queue<cycle> output{};
            tool::functional_queue<cycle> input = p.Cycles;
            while (!input.empty()) {
                output = output << operator*(input.first());
                input = input.rest();
            }
            return {output};
        }
        
        permutation operator*(const cross<elem> v) const {
            cross<elem> output(v.size());
            for(int i = 0; i < v.size(); ++i) output[i] = operator*(v[i]);
            return output;
        }
        
    private:
        permutation(tool::functional_queue<cycle> c) : Cycles{c} {}
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
}

#endif

