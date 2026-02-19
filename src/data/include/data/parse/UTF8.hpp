
#ifndef DATA_PARSE_UTF8_RANGES
#define DATA_PARSE_UTF8_RANGES

#include <data/parse/exactly.hpp>
#include <iostream>

namespace data::parse::UTF8 {

    // ===============================
    // ASCII Punctuation (for clarity)
    // ===============================

    struct space              : one<0x0020> {};
    struct exclamation_mark   : one<0x0021> {};
    struct quotation_mark     : one<0x0022> {};
    struct number_sign        : one<0x0023> {};
    struct dollar_sign        : one<0x0024> {};
    struct percent_sign       : one<0x0025> {};
    struct ampersand          : one<0x0026> {};
    struct apostrophe         : one<0x0027> {};
    struct left_paren         : one<0x0028> {};
    struct right_paren        : one<0x0029> {};
    struct asterisk           : one<0x002A> {};
    struct plus_sign          : one<0x002B> {};
    struct comma              : one<0x002C> {};
    struct hyphen_minus       : one<0x002D> {};
    struct period             : one<0x002E> {};
    struct slash              : one<0x002F> {};
    struct colon              : one<0x003A> {};
    struct semicolon          : one<0x003B> {};
    struct less_than          : one<0x003C> {};
    struct equals_sign        : one<0x003D> {};
    struct greater_than       : one<0x003E> {};
    struct question_mark      : one<0x003F> {};
    struct at_sign            : one<0x0040> {};
    struct left_bracket       : one<0x005B> {};
    struct backslash_char     : one<0x005C> {};
    struct right_bracket      : one<0x005D> {};
    struct caret              : one<0x005E> {};
    struct underscore         : one<0x005F> {};
    struct grave_accent       : one<0x0060> {};
    struct left_brace         : one<0x007B> {};
    struct vertical_bar       : one<0x007C> {};
    struct right_brace        : one<0x007D> {};
    struct tilde              : one<0x007E> {};


    // :::::::::::::::::::::::::::::::
    // Curly Quotes & Dashes
    // :::::::::::::::::::::::::::::::

    struct left_single_quote  : one<0x2018> {}; // ‘
    struct right_single_quote : one<0x2019> {}; // ’
    struct left_double_quote  : one<0x201C> {}; // “
    struct right_double_quote : one<0x201D> {}; // ”
    struct single_low_quote   : one<0x201A> {}; // ‚
    struct double_low_quote   : one<0x201E> {}; // „

    struct en_dash            : one<0x2013> {}; // –
    struct em_dash            : one<0x2014> {}; // —
    struct horizontal_bar     : one<0x2015> {}; // ―
    struct ellipsis           : one<0x2026> {}; // …


    // :::::::::::::::::::::::::::::::
    // Mathematical Symbols
    // :::::::::::::::::::::::::::::::

    struct multiplication_sign : one<0x00D7> {}; // ×
    struct division_sign       : one<0x00F7> {}; // ÷
    struct plus_minus          : one<0x00B1> {}; // ±
    struct not_sign            : one<0x00AC> {}; // ¬
    struct degree_sign         : one<0x00B0> {}; // °
    struct micro_sign          : one<0x00B5> {}; // µ

    struct infinity            : one<0x221E> {}; // ∞
    struct square_root         : one<0x221A> {}; // √
    struct proportional_to     : one<0x221D> {}; // ∝
    struct integral            : one<0x222B> {}; // ∫
    struct double_integral     : one<0x222C> {}; // ∬
    struct summation           : one<0x2211> {}; // ∑
    struct product             : one<0x220F> {}; // ∏

    struct element_of          : one<0x2208> {}; // ∈
    struct not_element_of      : one<0x2209> {}; // ∉
    struct subset              : one<0x2282> {}; // ⊂
    struct superset            : one<0x2283> {}; // ⊃
    struct subset_eq           : one<0x2286> {}; // ⊆
    struct superset_eq         : one<0x2287> {}; // ⊇

    struct logical_and         : one<0x2227> {}; // ∧
    struct logical_or          : one<0x2228> {}; // ∨
    struct forall              : one<0x2200> {}; // ∀
    struct exists              : one<0x2203> {}; // ∃
    struct therefore           : one<0x2234> {}; // ∴
    struct because             : one<0x2235> {}; // ∵

    struct approx_equal        : one<0x2248> {}; // ≈
    struct not_equal           : one<0x2260> {}; // ≠
    struct less_equal          : one<0x2264> {}; // ≤
    struct greater_equal       : one<0x2265> {}; // ≥
    struct identical_to        : one<0x2261> {}; // ≡


    // :::::::::::::::::::::::::::::::
    // Arrows
    // :::::::::::::::::::::::::::::::

    struct left_arrow          : one<0x2190> {}; // ←
    struct up_arrow            : one<0x2191> {}; // ↑
    struct right_arrow         : one<0x2192> {}; // →
    struct down_arrow          : one<0x2193> {}; // ↓
    struct left_right_arrow    : one<0x2194> {}; // ↔
    struct implies_arrow       : one<0x21D2> {}; // ⇒
    struct iff_arrow           : one<0x21D4> {}; // ⇔


    // :::::::::::::::::::::::::::::::
    // Common Symbols
    // :::::::::::::::::::::::::::::::

    struct copyright_sign      : one<0x00A9> {}; // ©
    struct registered_sign     : one<0x00AE> {}; // ®
    struct trademark_sign      : one<0x2122> {}; // ™
    struct section_sign        : one<0x00A7> {}; // §
    struct pilcrow             : one<0x00B6> {}; // ¶
    struct bullet              : one<0x2022> {}; // •

    struct euro_sign           : one<0x20AC> {}; // €
    struct pound_sign          : one<0x00A3> {}; // £
    struct yen_sign            : one<0x00A5> {}; // ¥


    // :::::::::::::::::::::::::::::::
    // Invisible / Structural
    // :::::::::::::::::::::::::::::::

    struct no_break_space      : one<0x00A0> {};
    struct zero_width_space    : one<0x200B> {};
    struct zero_width_joiner   : one<0x200D> {};
    struct zero_width_nonjoin  : one<0x200C> {};
    struct left_to_right_mark  : one<0x200E> {};
    struct right_to_left_mark  : one<0x200F> {};


    // :::::::::::::::::::::::::::::::
    // Common Latin-1 Letters
    // :::::::::::::::::::::::::::::::

    struct a_acute             : one<0x00E1> {}; // á
    struct e_acute             : one<0x00E9> {}; // é
    struct i_acute             : one<0x00ED> {}; // í
    struct o_acute             : one<0x00F3> {}; // ó
    struct u_acute             : one<0x00FA> {}; // ú
    struct n_tilde             : one<0x00F1> {}; // ñ
    struct c_cedilla           : one<0x00E7> {}; // ç

    // ===============================
    // Basic Multilingual Plane (BMP)
    // ===============================

    // Basic ASCII
    struct ascii_upper : range<0x0041, 0x005A> {}; // A-Z
    struct ascii_lower : range<0x0061, 0x007A> {}; // a-z

    struct basic_latin : alternatives<
        ascii_upper,         // A–Z
        ascii_lower> {};     // a-z

    // Latin-1 Supplement
    struct latin1_supplement : range<0x0080, 0x00FF> {};

    // Latin Extended
    struct latin_extended_a : range<0x0100, 0x017F> {};
    struct latin_extended_b : range<0x0180, 0x024F> {};
    struct latin_extended_additional : range<0x1E00, 0x1EFF> {};

    // IPA Extensions
    struct ipa_extensions : range<0x0250, 0x02AF> {};

    // Spacing Modifier Letters
    struct spacing_modifier_letters : range<0x02B0, 0x02FF> {};

    // Combining Diacritical Marks
    struct combining_diacritical_marks : range<0x0300, 0x036F> {};


    // ===============================
    // Greek & Cyrillic
    // ===============================

    struct greek_and_coptic : range<0x0370, 0x03FF> {};
    struct cyrillic_basic : range<0x0400, 0x04FF> {};
    struct cyrillic_supplement : range<0x0500, 0x052F> {};
    struct cyrillic_extended_a : range<0x2DE0, 0x2DFF> {};
    struct cyrillic_extended_b : range<0xA640, 0xA69F> {};


    // ===============================
    // Hebrew & Arabic
    // ===============================

    struct hebrew : range<0x0590, 0x05FF> {};
    struct arabic_basic : range<0x0600, 0x06FF> {};
    struct arabic_supplement : range<0x0750, 0x077F> {};
    struct arabic_extended_a : range<0x08A0, 0x08FF> {};


    // ===============================
    // Indic Scripts
    // ===============================

    struct devanagari : range<0x0900, 0x097F> {};
    struct bengali : range<0x0980, 0x09FF> {};
    struct gurmukhi : range<0x0A00, 0x0A7F> {};
    struct gujarati : range<0x0A80, 0x0AFF> {};
    struct oriya : range<0x0B00, 0x0B7F> {};
    struct tamil : range<0x0B80, 0x0BFF> {};
    struct telugu : range<0x0C00, 0x0C7F> {};
    struct kannada : range<0x0C80, 0x0CFF> {};
    struct malayalam : range<0x0D00, 0x0D7F> {};


    // ===============================
    // East Asian Scripts
    // ===============================

    struct hiragana : range<0x3040, 0x309F> {};
    struct katakana : range<0x30A0, 0x30FF> {};
    struct katakana_phonetic_extensions : range<0x31F0, 0x31FF> {};

    struct bopomofo : range<0x3100, 0x312F> {};

    struct cjk_unified_ideographs : range<0x4E00, 0x9FFF> {};
    struct cjk_extension_a : range<0x3400, 0x4DBF> {};

    struct hangul_jamo : range<0x1100, 0x11FF> {};
    struct hangul_syllables : range<0xAC00, 0xD7AF> {};


    // ===============================
    // Symbols & Punctuation
    // ===============================

    struct general_punctuation : range<0x2000, 0x206F> {};
    struct superscripts_subscripts : range<0x2070, 0x209F> {};
    struct currency_symbols : range<0x20A0, 0x20CF> {};
    struct combining_marks_for_symbols : range<0x20D0, 0x20FF> {};

    struct letterlike_symbols : range<0x2100, 0x214F> {};
    struct number_forms : range<0x2150, 0x218F> {};
    struct arrows : range<0x2190, 0x21FF> {};
    struct mathematical_operators : range<0x2200, 0x22FF> {};
    struct miscellaneous_technical : range<0x2300, 0x23FF> {};
    struct control_pictures : range<0x2400, 0x243F> {};
    struct enclosed_alphanumerics : range<0x2460, 0x24FF> {};
    struct box_drawing : range<0x2500, 0x257F> {};
    struct block_elements : range<0x2580, 0x259F> {};
    struct geometric_shapes : range<0x25A0, 0x25FF> {};
    struct miscellaneous_symbols : range<0x2600, 0x26FF> {};
    struct dingbats : range<0x2700, 0x27BF> {};


    // ===============================
    // Private Use & Specials
    // ===============================

    struct private_use_area : range<0xE000, 0xF8FF> {};
    struct specials : range<0xFFF0, 0xFFFF> {};

    using emoji_faces = range<0x1F600, 0x1F64F>;

    struct latin : alternatives<
        basic_latin,
        latin1_supplement,
        latin_extended_a,
        latin_extended_b,
        latin_extended_additional> {};


    // ===============================
    // Latin Case (Approximate)
    // ===============================

    struct latin_upper : alternatives<
        ascii_upper,
        range<0x00C0, 0x00D6>,
        range<0x00D8, 0x00DE>,
        range<0x0100, 0x017E> // partial; mixed but practical
    > {};

    struct latin_lower : alternatives<
        ascii_lower,
        range<0x00DF, 0x00F6>,
        range<0x00F8, 0x00FF>,
        range<0x0101, 0x017F> // partial; mixed but practical
    > {};

    struct greek : greek_and_coptic {};

    // ===============================
    // Greek Case (Approximate)
    // ===============================

    struct greek_upper : range<0x0391, 0x03A9> {};
    struct greek_lower : range<0x03B1, 0x03C9> {};

    struct cyrillic : alternatives<
        cyrillic_basic,
        cyrillic_supplement,
        cyrillic_extended_a,
        cyrillic_extended_b> {};

    // ===============================
    // Cyrillic Case (Approximate)
    // ===============================

    struct cyrillic_upper : range<0x0410, 0x042F> {};
    struct cyrillic_lower : range<0x0430, 0x044F> {};

    struct upper : alternatives<latin_upper, greek_upper, cyrillic_upper> {};
    struct lower : alternatives<latin_lower, greek_lower, cyrillic_lower> {};

    struct arabic_all : alternatives<
        arabic,
        arabic_supplement,
        arabic_extended_a> {};

    struct indic : alternatives<
        devanagari,
        bengali,
        gurmukhi,
        gujarati,
        oriya,
        tamil,
        telugu,
        kannada,
        malayalam> {};

    struct japanese : alternatives<
        hiragana,
        katakana,
        katakana_phonetic_extensions> {};

    struct korean : alternatives<
        hangul_jamo,
        hangul_syllables> {};

    struct cjk : alternatives<
        cjk_unified_ideographs,
        cjk_extension_a> {};

    using french_letter =
        alternatives<
            alpha,
            one<0x00E0>, one<0x00E2>, one<0x00E6>,
            one<0x00E7>,
            one<0x00E9>, one<0x00E8>, one<0x00EA>, one<0x00EB>,
            one<0x00EE>, one<0x00EF>,
            one<0x00F4>, one<0x0153>,
            one<0x00F9>, one<0x00FB>, one<0x00FC>,
            one<0x00FF>,
            // uppercase versions
            one<0x00C0>, one<0x00C2>, one<0x00C6>,
            one<0x00C7>,
            one<0x00C9>, one<0x00C8>, one<0x00CA>, one<0x00CB>,
            one<0x00CE>, one<0x00CF>,
            one<0x00D4>, one<0x0152>,
            one<0x00D9>, one<0x00DB>, one<0x00DC>>;

    using german_letter =
        alternative<
            alpha,
            one<0x00E4>, one<0x00F6>, one<0x00FC>,
            one<0x00C4>, one<0x00D6>, one<0x00DC>,
            one<0x00DF>,
            one<0x1E9E>
        >;

    using spanish_letter =
        alternative<
            alpha,
            one<0x00F1>, one<0x00D1>,
            one<0x00E1>, one<0x00E9>, one<0x00ED>,
            one<0x00F3>, one<0x00FA>,
            one<0x00FC>,
            one<0x00C1>, one<0x00C9>, one<0x00CD>,
            one<0x00D3>, one<0x00DA>
        >;

    using spanish_punctuation =
        alternative<
            exactly<U+00BF>,
            exactly<U+00A1>
        >;
}

#endif

