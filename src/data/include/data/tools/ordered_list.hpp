// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <concepts>
#include <data/reverse.hpp>
#include <data/take.hpp>
    
namespace data::tool {

    template <Stack stack, Sortable elem>
    struct ordered_stack;
    
    // merge two stacks together. 
    template <Stack stack, Sortable elem> ordered_stack<stack, elem> operator & (ordered_stack<stack, elem>, ordered_stack<stack, elem>);
    template <Stack stack, Sortable elem> ordered_stack<stack, elem> operator | (ordered_stack<stack, elem>, ordered_stack<stack, elem>);
    template <Stack stack, Sortable elem> ordered_stack<stack, elem> operator ^ (ordered_stack<stack, elem>, ordered_stack<stack, elem>);

    // print a stack
    template <Stack stack, Sortable elem>
    requires requires (std::ostream &o, const elem &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream &operator << (std::ostream &o, const ordered_stack<stack, elem> &l);

    template <Stack stack, Sortable elem> bool empty (ordered_stack<stack, elem>);
    template <Stack stack, Sortable elem> size_t size (ordered_stack<stack, elem>);
    template <Stack stack, Sortable elem> ordered_stack<stack, elem> values (ordered_stack<stack, elem>);
    template <Stack stack, Sortable elem> ordered_stack<stack, elem> take (ordered_stack<stack, elem>, size_t size);
    template <Stack stack, Sortable elem> ordered_stack<stack, elem> merge (ordered_stack<stack, elem>, ordered_stack<stack, elem>);

    template <Stack stack, Sortable elem>
    struct ordered_stack {
        stack Stack;

        // an ordered_stack is valid if every element is
        // valid and if the list of elements is ordered.
        bool valid () const;

        ordered_stack () : Stack {} {}

        ordered_stack (stack x) : Stack {x} {}

        bool empty () const {
            return Stack.empty ();
        }

        size_t size () const {
            return Stack.size ();
        }

        // we cannot have a non-const version of this function
        // because if the user changed a value, the stack 
        // might not be ordered anymore. 
        const elem &first () const {
            return Stack.first ();
        }

        const elem &operator [] (size_t z) const {
            return Stack[z];
        }

        ordered_stack insert (inserted<elem> x) const;

        template<typename ... P>
        ordered_stack insert (const elem &a, const elem &b, P ... p) const {
            return Stack.insert (a).insert (b, p...);
        }

        ordered_stack (std::initializer_list<wrapped<elem>> z): Stack {} {
            for (wrapped<elem> w : z) *this = insert (w);
        }
        
        ordered_stack operator >> (inserted<elem> x) const;
        ordered_stack operator >>= (inserted<elem> x);
        
        ordered_stack rest () const {
            return {data::rest (Stack)};
        }

        ordered_stack take (size_t size) const {
            return ordered_stack {data::take (Stack, size)};
        }

        ordered_stack remove (size_t index) const {
            if (index >= Stack.size ()) return *this;
            return ordered_stack {data::take (Stack, size) + data::drop (Stack, size).rest ()};
        }

        template <Sequence X> requires std::equality_comparable_with<elem, decltype (std::declval<X> ().first ())>
        bool operator == (const X &x) const {
            return sequence_equal (*this, x);
        }

        ordered_stack merge (const ordered_stack &a) const {
            return ordered_stack {data::functional::merge_stack (Stack, a.Stack)};
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

    template <Stack stack, Sortable elem> bool inline empty (ordered_stack<stack, elem> x) {
        return x.Stack.empty ();
    }

    template <Stack stack, Sortable elem> size_t inline size (ordered_stack<stack, elem> x) {
        return x.Stack.size ();
    }

    template <Stack stack, Sortable elem> ordered_stack<stack, elem> inline values (ordered_stack<stack, elem> x) {
        return x;
    }

    template <Stack stack, Sortable elem> ordered_stack<stack, elem> inline sort (ordered_stack<stack, elem> x) {
        return x;
    }

    template <Stack stack, Sortable elem> 
    ordered_stack<stack, elem> inline 
    operator & (ordered_stack<stack, elem> a, ordered_stack<stack, elem> b) {
        return a.merge (b);
    }

    template <Stack stack, Sortable elem> 
    ordered_stack<stack, elem> inline 
    merge (ordered_stack<stack, elem> a, ordered_stack<stack, elem> b) {
        return a.merge (b);
    }

    template <Stack stack, Sortable elem>
    requires requires (std::ostream &o, const elem &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream &operator << (std::ostream &o, const ordered_stack<stack, elem> &l) {
        functional::write (o << "ordered_sequence",  l);
        return o;
    }

    template <Stack stack, Sortable elem> ordered_stack<stack, elem> take (ordered_stack<stack, elem> x, size_t size) {
        return x.take (size);
    }
    
    template <Stack stack, Sortable elem>
    ordered_stack<stack, elem> inline ordered_stack<stack, elem>::operator >> (inserted<elem> x) const {
        return insert (x);
    }

    template <Stack stack, Sortable elem>
    ordered_stack<stack, elem> inline ordered_stack<stack, elem>::operator >>= (inserted<elem> x) {
        return *this = insert (x);
    }
    
    template <Stack stack, Sortable elem>
    ordered_stack<stack, elem> ordered_stack<stack, elem>::insert (inserted<elem> x) const {
        if (Stack.empty () || x < Stack.first ())
            return ordered_stack {Stack.prepend (x)};

        return ordered_stack {rest ().insert (x).Stack.prepend (Stack.first ())};
    }

    template <Stack stack, Sortable elem>
    bool ordered_stack<stack, elem>::valid () const {
        if (Stack.size () == 0) return true;
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

