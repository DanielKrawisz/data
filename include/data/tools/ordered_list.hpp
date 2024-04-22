// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <concepts>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/reverse.hpp>
    
namespace data::tool {
    
    template <functional::stack stack, ordered element = element_of<stack>>
    struct ordered_stack : stack {
        ordered_stack () : stack {} {}
        
        ordered_stack insert (const element &x) const;

        ordered_stack (const element &x): stack {} {
            *this = insert (x);
        }

        template<typename ... P>
        ordered_stack insert (const element &a, const element &b, P ... p) const {
            return insert (a).insert (b, p...);
        }
        
        template<typename ... P>
        ordered_stack (const element &x, P ... p) : stack {} {
            *this = insert (x).insert (p...);
        }
        
        ordered_stack operator << (const element &x) const;
        
        ordered_stack rest () const {
            return {data::rest (static_cast<const stack> (*this))};
        }

        ordered_stack take (uint32 size) const {
            return ordered_stack {data::take (static_cast<stack> (*this), size)};
        }

        ordered_stack remove (uint32 index) const {
            if (index >= this->size ()) return *this;
            return ordered_stack {data::take (static_cast<stack> (*this), size) + data::drop (static_cast<stack> (*this), size).rest ()};
        }
        
        using iterator = sequence_iterator<ordered_stack>;
        using sentinel = data::sentinel<ordered_stack>;
        
        iterator begin () const {
            return iterator {*this};
        }
        
        sentinel end () const {
            return sentinel {*this};
        }

        template <data::sequence X> requires std::equality_comparable_with<element, data::element_of<X>>
        bool operator == (const X &x) const {
            return sequence_equal (*this, x);
        }

        ordered_stack merge (const ordered_stack &a) const {
            return ordered_stack {data::functional::merge_stack (static_cast<const stack> (*this), static_cast<const stack> (a))};
        }
        
    private:
        ordered_stack (const stack &x) : stack {x} {}
    };

    template <functional::stack stack, ordered element>
    requires requires (std::ostream &o, const element &e) {
        { o << e } -> std::same_as<std::ostream &>;
    } std::ostream &operator << (std::ostream &o, const ordered_stack<stack, element> &l) {
        o << "ordered_list {";
        if (!l.empty ()) {
            ordered_stack<stack, element> x = l;
            while (true) {
                o << x.first ();
                x = x.rest ();
                if (x.empty ()) break;
                o << ", ";
            }
        }
        return o << "}";
    }
    
    template <functional::stack stack, ordered element>
    ordered_stack<stack, element> inline ordered_stack<stack, element>::operator << (const element &x) const {
        return insert (x);
    }
    
    template <functional::stack stack, ordered element>
    ordered_stack<stack, element> ordered_stack<stack, element>::insert (const element &x) const {
        if (this->empty () || x < this->first ()) {
            return ordered_stack {this->prepend (x)};
        }

        return ordered_stack {rest ().insert (x).prepend (this->first ())};
    }
}

#endif

