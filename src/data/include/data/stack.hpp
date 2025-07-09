// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STACK
#define DATA_STACK

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
