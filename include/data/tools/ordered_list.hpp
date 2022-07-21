// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <concepts>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_stack.hpp>
    
namespace data::tool {
    
    template <functional::stack stack, ordered element = element_of<stack>>
    struct ordered_stack : stack {
        ordered_stack() : stack{} {}
        
        ordered_stack insert(const element& x) const;
        
        template<typename ... P>
        ordered_stack insert(const element &a, const element &b, P ... p) const {
            return insert(a).insert(b, p...);
        }
        
        template<typename ... P>
        ordered_stack(P ... p) : stack{} {
            *this = insert(p...);
        }
        
        ordered_stack operator<<(const element& x) const;
        
        ordered_stack rest() const {
            return {data::rest(static_cast<const stack>(*this))};
        }
        
        using iterator = sequence_iterator<ordered_stack>;
        using sentinel = data::sentinel<ordered_stack>;
        
        iterator begin() const {
            return iterator{*this};
        }
        
        sentinel end() const {
            return sentinel{*this};
        }
        
    private:
        ordered_stack(const stack &x) : stack{x} {}
    };
    
    template <functional::stack stack, ordered element>
    std::ostream& operator<<(std::ostream& o, const ordered_stack<stack, element>& l) {
        o << "ordered_list{";
        if (!l.empty()) {
            ordered_stack<stack, element> x = l;
            while(true) {
                o << x.first();
                x = x.rest();
                if (x.empty()) break;
                o << ", ";
            }
        }
        return o << "}";
    }
    
    template <functional::stack stack, ordered element>
    ordered_stack<stack, element> inline ordered_stack<stack, element>::operator<<(const element& x) const {
        return insert(x);
    }
    
    template <functional::stack stack, ordered element>
    ordered_stack<stack, element> ordered_stack<stack, element>::insert(const element& x) const {
        if (this->empty() || x < this->first()) return {this->prepend(x)};
        return {rest().insert(x).prepend(this->first())};
    }
}

#endif

