// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/tools/cycle.hpp>
#include <data/list.hpp>

namespace data {

    template <typename X> using cycle = tool::cycle<X, list<X>>;
}
