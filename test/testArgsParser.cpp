// Copyright (c) 2023 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/io/arg_parser.hpp"

namespace data::io {

    enum parameter {
        verb,
        subject,
        object,
        article,
        preposition,
        complement
    };

    TEST (ArgsParserTest, TestFlagsOptions) {
        // a single flag.
        command<parameter> cmd {option {verb, "verb"}, flag {article, "article"}};

    }
}
