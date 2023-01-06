// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_PARSER
#define DATA_PARSER

#include <data/stream.hpp>

namespace data {

    using parse_error = exception;

    template <typename word, typename out>
    using parser = function<ptr<writer<word>> (function<void (const out &)>, function<void (parse_error)>)>;
}

#endif
