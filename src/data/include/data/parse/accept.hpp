
#ifndef DATA_PARSE_ACCEPT
#define DATA_PARSE_ACCEPT

#include <data/concepts.hpp>
#include <data/types.hpp>
#include <istream>

namespace data::parse {

    template <typename M>
    concept Machine = requires {
        { [] () constexpr { return M {}; } () };
    } && requires (const M &m) {
        // whether the pattern is valid as it is.
        { [] (const M &m) constexpr { return m.valid (); } (m) } -> Same<bool>;
        // whether characters could be added to make a valid pattern.
        { [] (const M &m) constexpr { return m.possible (); } (m) } -> Same<bool>;
    } && requires (M &m, string_view v, char c) {
        // return the amount of backtracking required.
        { [] (M &m, string_view v, char c) constexpr { return m.step (v, c); } (m, v, c) } -> Same<int>;
    };

    // NOTE: because PEGTL is a stateless parser, you can't use it
    // to recognize a token, such as a URL. We realized this once
    // we got to this point. In theory we could build up the entire
    // URL library using state machines like we have below, but we
    // haven't got around to that yet.
    template <Machine State>
    std::string read_token (std::istream &in, State &state) {
        using traits = std::char_traits<char>;
        std::string buf;

        // TODO backtracking.
        while (true) {
            int p = in.peek ();
            if (p == traits::eof ()) break;

            char c = traits::to_char_type (p);
            state.step (buf, c);

            in.get ();
            buf.push_back (c);

            if (!state.possible ()) break;
        }

        if (!state.valid ())
            in.setstate (std::ios::failbit);

        return buf;
    }

    template <Machine State>
    constexpr string_view read_token (const char *in, State &state) {
        size_t i = 0;
        char c;
        while ((c = in[i]) != '\0') {
            state.step (string_view {in, i}, c);
            i++;
            if (!state.possible ()) break;
        }

        return state.valid () ? string_view {in, i} : string_view {nullptr, 0};
    }

    template <Machine State>
    bool inline accept (std::stringstream &ss) {
        State state {};
        read_token (ss, state);
        return bool (ss);
    }

    template <Machine State> constexpr bool inline accept (const std::string &x) {
        State state {};
        string_view result = read_token (x.data (), state);
        return result.size () == x.size () && result.data () != nullptr;
    }

    template <Machine State> constexpr bool inline accept (const std::u8string &x) {
        State state {};
        string_view result = read_token (reinterpret_cast<const char*> (x.data ()), state);
        return result.size () == x.size () && result.data () != nullptr;
    }
}

#endif
