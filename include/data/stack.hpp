// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STACK
#define DATA_STACK

    /*
     *    ---------------------------------------------------------------------------
     *    data::stack<X>
     *    ---------------------------------------------------------------------------
     *
     *    Part of:  functional data structures
     *
     *    Provides: a functional stack.
     *
     *    See Also:
     *        data/concepts.hpp
     *        data/list.hpp
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
     *    A persistent, functional stack. This means that structural operations
     *    (prepend, rest, reverse, etc.) create new stacks and do not modify old
     *    ones. Multiple stacks may share underlying nodes.
     *
     *    X may be any type satisfying data::Copyable, including pointers and
     *    references.
     *
     *    data::stack<X> type satisfies the concepts:
     *        data::Stack<X>
     *        data::Container<X>
     *        data::ConstIterable<X>
     *        data::Iterable<X>          (when X is non-const)
     *
     *    It interoperates with a set of free functions found by argument-dependent
     *    lookup:
     *        valid, empty, size, first, rest, prepend, reverse,
     *        contains, take, drop, join, sort, sorted, remove, erase
     *
     *    All of these may also be named explicitly as data::size, data::first, etc.
     *
     *    The stack supports:
     *        - forward iteration (range-for)
     *        - random access via operator []
     *        - non-const iteration and non-const indexing when X is non-const
     *
     *    Access is constant time for:
     *        first (z)
     *        rest (z)
     *        prepend (x, z)
     *
     *    Traversal is O (n).
     *
     *    ---------------------------------------------------------------------------
     *    EXAMPLES BY OPERATION PATTERN
     *    ---------------------------------------------------------------------------
     *
     *    for z : data::stack<X> and x : X
     *
     *    Construction:
     *        {}
     *        data::stack<int> {1, 2, 3}
     *        prepend (z, x)         -> data::stack<X>
     *        z >> x                 -> data::stack<X>  (eqivalent to prepend)
     *
     *    Inspection:
     *        empty (z)              -> bool
     *        size (z)               -> std::size_t
     *        first (z)              -> X const&        (return head value;
     *                                                   throws data::empty_sequence
     *                                                   if stack is empty.)
     *        rest (z)               -> data::stack<X>  (tail)
     *
     *    Iteration:
     *        for (auto &v : z) { ... }     // forward traversal
     *
     *    Indexing:
     *        z[i]                   -> X const &      (read)
     *        z[i]                   -> X &            (write, if X is non-const)
     *
     *    Composition:
     *        reverse (z)            -> data::stack<X>
     *        take (z, n)            -> data::stack<X>
     *        drop (z, t)            -> data::stack<X>
     *        join (z1, z2)          -> data::stack<X>
     *        z1 + z2                -> data::stack<X> (equivalent to join)
     *
     *    Search & Filtering:
     *        contains (z, x)        -> bool
     *        remove (z, n)          -> data::stack<X>   (remove all instances)
     *        erase (z, x)           -> data::stack<X>   (remove by index)
     *
     *    ---------------------------------------------------------------------------
     *    PERSISTENCE (STRUCTURAL SHARING)
     *    ---------------------------------------------------------------------------
     *
     *    Stacks share nodes. Example:
     *
     *        auto a = data::stack<int> {1, 2, 3};
     *        auto b = reverse (a);           // b = {3, 2, 1}
     *
     *        first (a) -> 1                  // a unchanged
     *        first (b) -> 3
     *
     *    Creating new stacks never invalidates older ones.
     *
     *    ---------------------------------------------------------------------------
     *    MUTABILITY WARNING
     *    ---------------------------------------------------------------------------
     *
     *    Non-const operator [] and non-const iteration return references into the
     *    shared structure.  Writing through them mutates the underlying node:
     *
     *        auto a = data::stack<int> {1, 2, 3};
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
     *    USING REFERENCES OR POINTERS IN THE STACK
     *    ---------------------------------------------------------------------------
     *
     *    Because nodes are shared and contain the stored value directly, if X is a
     *    reference or a pointer to mutable data, then mutating through one stack
     *    affects all other stacks sharing that underlying element.
     *
     *    Example:
     *
     *        data::stack<int &> s;
     *        int p = 10, q = 20;
     *        s = prepend (prepend (s, p), q);         // data::stack<int &>
     *
     *        p = 99;
     *        first (rest (s)) -> 99                   // mutation visible everywhere
     *
     *    ---------------------------------------------------------------------------
     *    CONVERSIONS
     *    ---------------------------------------------------------------------------
     *
     *    If X is implicitly convertible to Y, then:
     *
     *        data::stack<X>  ->(implicit)  data::stack<Y>
     *
     *    is also permitted.  Each element is implicitly converted during
     *    construction of the new stack.
     *
     *    If X is explicitly convertible to Y, then data::stack<X> may be cast
     *    to data::stack<Y> using an explicit conversion:
     *
     *        static_cast<data::stack<Y>> (z)
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
     *    then data::stack<X> also supports streaming:
     *
     *        std::cout << z;
     *
     *    producing a human-readable sequence of the stack’s elements.
     *
     *    ---------------------------------------------------------------------------
     *    SORTING
     *    ---------------------------------------------------------------------------
     *
     *    If X satisfies data::Sortable
     *        sort (z)               -> data::stack<X>
     *        sorted (z)             -> bool
     *
     *    ---------------------------------------------------------------------------
*/

// basic types
#include <data/types.hpp>

// This library uses a valid () method and a Valid member in a stereotyped way.
#include <data/valid.hpp>

// Interfaces related to data structures.
#include <data/functional/list.hpp>

#include <data/reverse.hpp>
#include <data/take.hpp>
#include <data/sort.hpp>

// Implementation of Stack.
#include <data/tools/linked_stack.hpp>

namespace data {

    template <typename X> using stack = linked_stack<X>;
}

#endif
