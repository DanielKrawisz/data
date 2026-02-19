
#ifndef DATA_PARSE_JSON
#define DATA_PARSE_JSON

#include <data/parse/ASCII.hpp>
#include <data/parse/hex.hpp>

namespace data::parse::email {

    struct atext : alternatives<
        ASCII::alpha,
        ASCII::digit,
        one<'!'>, one<'#'>, one<'$'>, one<'%'>, one<'&'>,
        one<'\''>, one<'*'>, one<'+'>, one<'-'>, one<'/'>,
        one<'='>, one<'?'>, one<'^'>, one<'_'>, one<'`'>,
        one<'{'>, one<'|'>, one<'}'>, one<'~'>
    > {};

    // atom = 1*atext
    struct atom : repeated<atext, 1, 255> {};

    // dot-atom-text = atom *("." atom)
    struct dot_atom_text_tail :
        repeated<
            sequence<one<'.'>, atom>,
            0, 254
        > {};

    struct dot_atom_text :
        sequence<atom, dot_atom_text_tail> {};

    // qtext = printable ASCII except \ and "
    struct qtext : alternatives<
        range<0x21, 0x21>,
        range<0x23, 0x5B>,
        range<0x5D, 0x7E>> {};

    // quoted-pair = "\" ASCII
    struct quoted_pair :
        sequence<
            one<'\\'>,
            range<0x00, 0x7F>> {};

    struct qcontent : alternatives<
        qtext,
        quoted_pair> {};

    struct quoted_string :
        sequence<
            one<'"'>,
            repeated<qcontent, 0, 255>,
            one<'"'>
        > {};

    struct local_part :
    alternatives<
        dot_atom_text,
        quoted_string
    > {};

}
#endif
