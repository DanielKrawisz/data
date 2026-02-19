
#ifndef DATA_PARSE_XML
#define DATA_PARSE_XML

#include <data/parse/ASCII.hpp>
#include <data/parse/hex.hpp>

namespace data::parse::XML {

    template <typename X> using predicate = UTF8::predicate<X>;

    struct is_xml_char {
        constexpr bool operator () (uint32_t c) {
            return c == 0x9  ||
                c == 0xA  ||
                c == 0xD  ||
                (c >= 0x20    && c <= 0xD7FF) ||
                (c >= 0xE000  && c <= 0xFFFD) ||
                (c >= 0x10000 && c <= 0x10FFFF);
        }
    };

    struct is_xml_name_start_char {
        constexpr bool operator () (uint32_t c) const {
            return
                c == ':' ||
                c == '_' ||
                (c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||

                (c >= 0xC0    && c <= 0xD6) ||
                (c >= 0xD8    && c <= 0xF6) ||
                (c >= 0xF8    && c <= 0x2FF) ||
                (c >= 0x370   && c <= 0x37D) ||
                (c >= 0x37F   && c <= 0x1FFF) ||
                (c >= 0x200C  && c <= 0x200D) ||
                (c >= 0x2070  && c <= 0x218F) ||
                (c >= 0x2C00  && c <= 0x2FEF) ||
                (c >= 0x3001  && c <= 0xD7FF) ||
                (c >= 0xF900  && c <= 0xFDCF) ||
                (c >= 0xFDF0  && c <= 0xFFFD) ||
                (c >= 0x10000 && c <= 0xEFFFF);
        }
    };

    struct is_xml_name_char {
        constexpr bool operator () (uint32_t c) const {
            return
                is_xml_name_start_char {} (c) ||
                c == '-' ||
                c == '.' ||
                (c >= '0' && c <= '9') ||
                c == 0xB7 ||
                (c >= 0x0300 && c <= 0x036F) ||
                (c >= 0x203F && c <= 0x2040);
        }
    };

    struct Name :
        sequence<
            predicate<is_xml_name_start_char>,
            star<predicate<is_xml_name_char>>> {};

    struct comment_unit : predicate<is_xml_char> {};

    struct Comment :
        sequence<
            exactly<'<','!','-','-'>,
            until<star<comment_unit>,
                '-','-'>, one<'>'>> {};

    struct is_xml_space {
        constexpr bool operator () (uint32_t c) const {
            return
            c == 0x20 ||
            c == 0x9  ||
            c == 0xD  ||
            c == 0xA;
        }
    };

    struct is_xml_encname_start {
        constexpr bool operator () (uint32_t c) const {
            return
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z');
        }
    };

    struct is_xml_encname_char {
        constexpr bool operator () (uint32_t c) const {
            return is_xml_encname_start {} (c) ||
                (c >= '0' && c <= '9') ||
                c == '.' ||
                c == '_' ||
                c == '-';
        }
    };

    // not used
    struct is_xml_pubid_char {
        constexpr bool operator () (uint32_t c) const {
            return
            c == 0x20 ||
            c == 0xD  ||
            c == 0xA  ||

            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||

            c == '-'  || c == '\'' || c == '('  || c == ')' ||
            c == '+'  || c == ','  || c == '.'  || c == '/' ||
            c == ':'  || c == '='  || c == '?'  || c == ';' ||
            c == '!'  || c == '*'  || c == '#'  || c == '@' ||
            c == '$'  || c == '_'  || c == '%';
        }
    };

    struct is_xml_chardata_char {
        constexpr bool operator () (uint32_t c) const {
            return
            is_xml_char {} (c) &&
            c != '<' &&
            c != '&';
        }
    };

    struct is_xml_attvalue_single_char {
        constexpr bool operator () (uint32_t c) const {
            return
            is_xml_char {} (c) &&
            c != '<' &&
            c != '&' &&
            c != '\'';
        }
    };

    struct is_xml_attvalue_double_char {
        constexpr bool operator () (uint32_t c) const {
            return
            is_xml_char {} (c) &&
            c != '<' &&
            c != '&' &&
            c != '"';
        }
    };

    struct S : plus<predicate<is_xml_space>> {};

    struct EntityRef :
        sequence<
            one<'&'>,
            Name,
            one<';'>> {};

    struct chardata_unit :
        alternatives<
            predicate<is_xml_chardata_char>,   // excludes < and &
            sequence<
                one<']'>,
                alternatives<
                    predicate<is_xml_chardata_char>,
                    sequence<
                        one<']'>,
                        predicate<is_xml_chardata_char>>>>> {};

    struct CharData :
        //complement<
            star<chardata_unit>
        // , contains<exactly<']',']','>'>>>
        {};

    struct cdata_unit : predicate<is_xml_char> {};

    struct CDSect :
        sequence<
            exactly<'<','!','[','C','D','A','T','A','['>,
            until<star<cdata_unit>,
                ']',']','>'>> {};

    struct pi_unit : predicate<is_xml_char> {};

    struct pi_target :
        complement<
            Name,
            exactly<'x','m','l'>> {};

    struct PI :
        sequence<
            exactly<'<','?'>,
            pi_target, // TODO this must not match 'xml'
            alternatives<
                sequence<
                    S,
                    until<star<pi_unit>, '?','>'>>,
                exactly<'?','>'>>> {};

    struct Digit : ASCII::digit {};

    struct HexDigit : ASCII::hex {};

    struct CharRef_dec :
        sequence<
            exactly<'&','#'>,
            plus<Digit>,
            one<';'>> {};

    struct CharRef_hex :
        sequence<
            exactly<'&','#','x'>,
            plus<HexDigit>,
            one<';'>> {};

    struct CharRef :
        alternatives<
            CharRef_hex,
            CharRef_dec> {};

    struct Reference :
        alternatives<
            EntityRef,
            CharRef>{};

    struct AttValue_double :
        sequence<
            one<'"'>,
            star<
                alternatives<
                    predicate<is_xml_attvalue_double_char>,
                    Reference>>,
            one<'"'>> {};

    struct AttValue_single :
        sequence<
            one<'\''>,
            star<
                alternatives<
                    predicate<is_xml_attvalue_single_char>,
                    Reference>>,
            one<'\''>> {};

    struct Eq : sequence<optional<S>, one<'='>, optional<S>> {};

    struct Attribute :
        sequence<
            Name,
            Eq,
            alternatives<AttValue_single, AttValue_double>> {};

    struct STag :
        sequence<
            one<'<'>,
            Name,
            star<
                sequence<
                    S,
                    Attribute
                >>,
            optional<S>,
            one<'>'>> {};

    struct EmptyElemTag :
        sequence<
            one<'<'>,
            Name,
            star<
                sequence<
                    S,
                    Attribute>>,
            optional<S>,
            exactly<'/','>'>> {};

    struct ETag :
        sequence<
            exactly<'<','/'>,
            Name,
            optional<S>,
            one<'>'>> {};

    struct element;

    struct content :
        sequence<
            optional<CharData>,
            star<
                sequence<
                    alternatives<
                        element,
                        Reference,
                        CDSect,
                        PI,
                        Comment>,
                    optional<CharData>>>> {};

    struct element :
        alternatives<
            EmptyElemTag,
            sequence<
                STag,
                content,
                ETag>> {};

    struct Misc :
        alternatives<
            Comment,
            PI,
            S> {};

    struct VersionNum :
        sequence<
            exactly<'1','.'>,
            plus<Digit>> {};

    struct VersionInfo :
        sequence<
            S,
            exactly<'v','e','r','s','i','o','n'>,
            Eq,
            alternatives<
                sequence<
                    one<'\''>,
                    VersionNum,
                    one<'\''>>,
                sequence<
                    one<'"'>,
                    VersionNum,
                    one<'"'>>>> {};

    struct EncName :
        sequence<
            predicate<is_xml_encname_start>,
            star<predicate<is_xml_encname_char>>> {};

    struct EncodingDecl :
        sequence<
            S,
            exactly<'e','n','c','o','d','i','n','g'>,
            Eq,
            alternatives<
                sequence<
                    one<'"'>,
                    EncName,
                    one<'"'>>,
                sequence<
                    one<'\''>,
                    EncName,
                    one<'\''>>>>{};

    struct SDDecl :
        sequence<
            S,
            exactly<'s','t','a','n','d','a','l','o','n','e'>,
            Eq,
            alternatives<
                sequence<
                    one<'"'>,
                    alternatives<
                        exactly<'y','e','s'>,
                        exactly<'n','o'>>,
                    one<'"'>>,
                sequence<
                    one<'\''>,
                    alternatives<
                        exactly<'y','e','s'>,
                        exactly<'n','o'>>,
                    one<'\''>>>> {};

    struct XMLDecl :
        sequence<
            exactly<'<','?','x','m','l'>,
            VersionInfo,
            optional<EncodingDecl>,
            optional<SDDecl>,
            optional<S>,
            exactly<'?','>'>> {};

    struct prolog :
        sequence<
            optional<XMLDecl>,
            star<Misc>> {};

    struct document :
        sequence<
            prolog,
            element,
            star<Misc>> {};

}

#endif
