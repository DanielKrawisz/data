// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

/*
 *    ---------------------------------------------------------------------------
 *    data::list<X>
 *    ---------------------------------------------------------------------------
 *
 *    Part of:  functional data structures
 *
 *    Provides: a functional queue (that also supports prepend).
 *
 *    See Also:
 *        data/concepts.hpp
 *        data/stack.hpp
 *        data/map.hpp
 *        data/for_each.hpp
 *        data/replace.hpp
 *        data/lift.hpp
 *        data/select.hpp
 *        data/transpose.hpp
 *        data/flatten.hpp
 *        data/shuffle.hpp
 *
 *    ---------------------------------------------------------------------------
 *    OVERVIEW
 *    ---------------------------------------------------------------------------
 *
 *    A persistent, functional queue. This means that structural operations
 *    (append, rest, reverse, etc.) create new stacks and do not modify old
 *    ones. Multiple stacks may share underlying nodes.
 *
 *    X may be any type satisfying data::Copyable, including pointers and
 *    references.
 *
 *    data::list<X> type satisfies the concepts:
 *        data::List<X>
 *        data::Container<X>
 *        data::ConstIterable<X>
 *        data::Iterable<X>          (when X is non-const)
 *
 *    It interoperates with a set of free functions found by argument-dependent
 *    lookup:
 *        valid, empty, size, first, rest, append, prepend, reverse, contains,
 *        take, drop, join, sort, sorted, remove, erase, rotate_right, rotate_left
 *
 *    All of these may also be named explicitly as data::size, data::first, etc.
 *
 *    The list supports:
 *        - forward iteration (range-for)
 *        - random access via operator []
 *        - non-const iteration and non-const indexing when X is non-const
 *
 *    Access is constant time for:
 *        first (z)
 *        rest (z)
 *        append (x, z)
 *
 *    Traversal is O (n).
 *
 *    ---------------------------------------------------------------------------
 *    EXAMPLES BY OPERATION PATTERN
 *    ---------------------------------------------------------------------------
 *
 *    for z : data::list<X> and x : X
 *
 *    Construction:
 *        {}
 *        data::list<int> {1, 2, 3}
 *        append (z, x)          -> data::list<X>
 *        z << x                 -> data::list<X>   (eqivalent to append)
 *        prepend (z, x)         -> data::list<X>
 *        z >> x                 -> data::list<X>   (eqivalent to prepend)
 *
 *    Inspection:
 *        empty (z)              -> bool
 *        size (z)               -> std::size_t
 *        first (z)              -> X const&        (return head value;
 *                                                   throws data::empty_sequence
 *                                                   if list is empty.)
 *        rest (z)               -> data::list<X>   (tail)
 *
 *    Iteration:
 *        for (auto &v : z) { ... }     // forward traversal
 *
 *    Indexing:
 *        z[i]                   -> X const &      (read)
 *        z[i]                   -> X &            (write, if X is non-const)
 *
 *    Composition:
 *        reverse (z)            -> data::list<X>
 *        take (z, n)            -> data::list<X>
 *        drop (z, t)            -> data::list<X>
 *        join (z1, z2)          -> data::list<X>
 *        z1 + z2                -> data::list<X> (equivalent to join)
 *        rotate_right (z, n)    -> data::list<X>
 *        rotate_left (z, n)     -> data::list<X>
 *
 *    Search & Filtering:
 *        contains (z, x)        -> bool
 *        remove (z, n)          -> data::list<X>   (remove all instances)
 *        erase (z, x)           -> data::list<X>   (remove by index)
 *
 *    ---------------------------------------------------------------------------
 *    PERSISTENCE (STRUCTURAL SHARING)
 *    ---------------------------------------------------------------------------
 *
 *    Stacks share nodes. Example:
 *
 *        auto a = data::list<int> {1, 2, 3};
 *        auto b = reverse (a);           // b = {3, 2, 1}
 *
 *        first (a) -> 1                  // a unchanged
 *        first (b) -> 3
 *
 *    Creating new lists never invalidates older ones.
 *
 *    ---------------------------------------------------------------------------
 *    MUTABILITY WARNING
 *    ---------------------------------------------------------------------------
 *
 *    Non-const operator [] and non-const iteration return references into the
 *    shared structure.  Writing through them mutates the underlying node:
 *
 *        auto a = data::list<int> {1, 2, 3};
 *        auto b = a;                    // shares all nodes with a
 *
 *        a[1] = 99;                     // modifies the shared node
 *
 *        b[1] returns 99                // b sees the change
 *
 *    This can be convenient, but it is easy to introduce unintended coupling
 *    between values.  Use writable access only when you fully understand the
 *    implications of functional sharing.
 *
 *    ---------------------------------------------------------------------------
 *    USING REFERENCES OR POINTERS IN THE LIST
 *    ---------------------------------------------------------------------------
 *
 *    Because nodes are shared and contain the stored value directly, if X is a
 *    reference or a pointer to mutable data, then mutating through one list
 *    affects all other lists sharing that underlying element.
 *
 *    Example:
 *
 *        data::list<int &> s;
 *        int p = 10, q = 20;
 *        s = append (append (s, p), q);     // data::list<int &>
 *
 *        q = 99;
 *        first (rest (s)) -> 99             // mutation visible everywhere
 *
 *    ---------------------------------------------------------------------------
 *    CONVERSIONS
 *    ---------------------------------------------------------------------------
 *
 *    If X is implicitly convertible to Y, then:
 *
 *        data::list<X>   ->(implicit)  data::list<Y>
 *
 *    is also permitted. Each element is implicitly converted during
 *    construction of the new list.
 *
 *    If X is explicitly convertible to Y, then data::list<X> may be cast
 *    to data::list<Y> using an explicit conversion:
 *
 *        static_cast<data::list<Y>> (z)
 *
 *    with each element converted explicitly.
 *
 *    ---------------------------------------------------------------------------
 *    STREAMING
 *    ---------------------------------------------------------------------------
 *
 *    If X provides:
 *
 *        std::ostream &operator << (std::ostream &, X const&);
 *
 *    then data::list<X> also supports streaming:
 *
 *        std::cout << z;
 *
 *    producing a human-readable sequence of the list’s elements.
 *
 *    ---------------------------------------------------------------------------
 *    SORTING
 *    ---------------------------------------------------------------------------
 *
 *    If X satisfies data::Sortable
 *        sort (z)               -> data::list<X>
 *        sorted (z)             -> bool
 *
 *    ---------------------------------------------------------------------------
*/

#include <data/stack.hpp>

// implementation of List
#include <data/tools/functional_queue.hpp>

namespace data {

    // functional queue built using the list. 
    template <typename X> using list = functional_queue<data::stack<X>, X>;

}

#endif
