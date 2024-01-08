// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/io/arg_parser.hpp"
#include "gtest/gtest.h"

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
        command<parameter> cmd {
            {{verb, "verb"}, {verb, "subject"}},
            {{verb, "object"}},
            {{article, "article", 'A'}, {preposition, "preposition", 'P'}, {complement, "complement", 'C'}}};

    }
}
