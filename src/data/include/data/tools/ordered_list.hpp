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

    template <Stack stack, Sortable element = unref<unconst<decltype (std::declval<stack> ().first ())>>>
    struct ordered_stack;
    
    // merge two stacks together. 
    template <Stack stack> ordered_stack<stack> operator & (ordered_stack<stack>, ordered_stack<stack>);
    template <Stack stack> ordered_stack<stack> operator | (ordered_stack<stack>, ordered_stack<stack>);
    template <Stack stack> ordered_stack<stack> operator ^ (ordered_stack<stack>, ordered_stack<stack>);

    // print a stack
    template <Stack stack, Sortable element>
    requires requires (std::ostream &o, const element &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream &operator << (std::ostream &o, const ordered_stack<stack, element> &l);

    template <Stack stack> ordered_stack<stack> empty (ordered_stack<stack>);
    template <Stack stack> size_t size (ordered_stack<stack>);
    template <Stack stack> ordered_stack<stack> take (ordered_stack<stack>, size_t size);
    template <Stack stack> ordered_stack<stack> merge (ordered_stack<stack>, ordered_stack<stack>);

    template <Stack stack, Sortable element>
    struct ordered_stack : stack {

        // an ordered_stack is valid if every element is
        // valid and if the list of elements is ordered.
        bool valid () const;

        ordered_stack () : stack {} {}

        ordered_stack (stack x) : stack {x} {}

        ordered_stack insert (const element &x) const;

        template<typename ... P>
        ordered_stack insert (const element &a, const element &b, P ... p) const {
            return insert (a).insert (b, p...);
        }

        ordered_stack (std::initializer_list<wrapped<element>> z) {
            for (wrapped<element> w : z) *this = insert (w);
        }
        
        ordered_stack operator << (const element &x) const;
        ordered_stack operator <<= (const element &x);
        
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

        template <Sequence X> requires std::equality_comparable_with<element, decltype (std::declval<X> ().first ())>
        bool operator == (const X &x) const {
            return sequence_equal (*this, x);
        }

        ordered_stack merge (const ordered_stack &a) const {
            return ordered_stack {data::functional::merge_stack (static_cast<const stack> (*this), static_cast<const stack> (a))};
        }
        
        using iterator = sequence_iterator<ordered_stack>;
        using sentinel = data::sentinel<ordered_stack>;
        
        iterator begin () const {
            return iterator {*this};
        }
        
        sentinel end () const {
            return sentinel {*this};
        }
    };

    template <Stack stack> ordered_stack<stack> inline empty (ordered_stack<stack> x) {
        return x.empty ();
    }

    template <Stack stack> ordered_stack<stack> inline operator & (ordered_stack<stack> a, ordered_stack<stack> b) {
        return a.merge (b);
    }

    template <Stack stack> ordered_stack<stack> inline merge (ordered_stack<stack> a, ordered_stack<stack> b) {
        return a.merge (b);
    }

    template <Stack stack, Sortable element>
    requires requires (std::ostream &o, const element &e) {
        { o << e } -> Same<std::ostream &>;
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

    template <Stack stack> ordered_stack<stack> take (ordered_stack<stack> x, size_t size) {
        return x.take (size);
    }
    
    template <Stack stack, Sortable element>
    ordered_stack<stack, element> inline ordered_stack<stack, element>::operator << (const element &x) const {
        return insert (x);
    }

    template <Stack stack, Sortable element>
    ordered_stack<stack, element> inline ordered_stack<stack, element>::operator <<= (const element &x) {
        return *this = insert (x);
    }
    
    template <Stack stack, Sortable element>
    ordered_stack<stack, element> ordered_stack<stack, element>::insert (const element &x) const {
        if (this->empty () || x < this->first ())
            return ordered_stack {this->prepend (x)};

        return ordered_stack {rest ().insert (x).prepend (this->first ())};
    }

    template <Stack stack, Sortable element>
    bool ordered_stack<stack, element>::valid () const {
        if (this->size () == 0) return true;
        auto it = begin ();
        while (true) {
            if (!data::valid (*it)) return false;
            auto last = it++;
            if (it == end ()) return true;
            if (!(*last < *it)) return false;
        }

        return true;
    }
}

#endif

