#include <data/io/arg_parser.hpp>
namespace data::io::args {
    inline const std::regex option_re {
        R"(^--([A-Za-z][A-Za-z0-9_]*)=(.*)$)"
    };

    inline const std::regex flag_re {
        R"(^--([A-Za-z][A-Za-z0-9_]*)$)"
    };

    inline const std::regex ab_flag_re {
        R"(^-([A-Za-z]+)$)"
    };

    parsed::parsed (int arg_count, const char *const arg_value[]) {
        list<string_view> positionals;
        for (int i = 0; i < arg_count; ++i) {
            string_view arg {arg_value[i]};
            std::cmatch m;

            if (std::regex_match (arg_value[i], m, option_re)) {
                std::string_view key {m[1].first, static_cast<size_t> (m[1].length ())};
                if (Options.contains (key)) throw exception {} << "alreahy have key " << key;
                // --name=value
                Options = Options.insert (key, std::string_view {m[2].first, static_cast<size_t> (m[2].length ())});
            } else if (std::regex_match (arg_value[i], m, flag_re)) {
                // --name
                std::string_view q {m[1].first, static_cast<size_t> (m[1].length ())};
                if (Flags.contains (q)) throw exception {} << "already have flag " << q;
                Flags = Flags.insert (q);
            } else if (std::regex_match (arg_value[i], m, ab_flag_re)) {
                for (int i = 0; i < m[1].length (); i++) {
                    std::string_view q {m[1].first + i, 1};
                    if (Flags.contains (q)) throw exception {} << "already have flag " << q;
                    Flags = Flags.insert (q);
                }
            } else {
                // positional
                positionals <<= arg;
            }
        }

        Arguments = cross<string_view> (positionals);
    }
}
