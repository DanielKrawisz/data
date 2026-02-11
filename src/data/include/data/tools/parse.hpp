
#ifndef DATA_TOOLS_SCHEMA
#define DATA_TOOLS_SCHEMA

#include <data/concepts.hpp>
#include <data/tuple.hpp>
#include <data/maybe.hpp>

namespace data::parse {
    template <typename M>
    concept Machine = std::default_initializable<M> && requires (const M &m) {
        // whether the pattern is valid as it is.
        { m.valid () } -> Same<bool>;
        // whether characters could be added to make a valid pattern.
        { m.possible () } -> Same<bool>;
    } && requires (const M &m, string_view v, char c) {
        { m.step (v, c) } -> Same<M>;
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

        while (true) {
            int p = in.peek ();
            if (p == traits::eof ()) {
                if (!state.valid ())
                    in.setstate (std::ios::failbit);
                return buf;
            }

            char c = traits::to_char_type (p);
            State next = state.step (buf, c);

            if (!next.possible ()) {
                if (!next.valid ())
                    in.setstate (std::ios::failbit);
                return buf;
            }

            in.get ();
            buf.push_back (c);
            state = std::move (next);
        }
    }

    template <Machine State>
    bool accept (const std::string &x) {
        std::stringstream ss {x};
        State state {};
        read_token (ss, state);
        // if the string was accepted, the stream should
        // not be in an error state and should be at eof.
        return bool (ss) && ss.eof ();
    }

}

namespace data::parse::machine {

    struct invalid {

        bool possible () const {
            return false;
        }

        bool valid () const {
            return false;
        }

        invalid step (string_view, char c) const {
            return *this;
        }
    };

    struct any {

        bool possible () const {
            return true;
        }

        bool valid () const {
            return true;
        }

        any step (string_view, char c) const {
            return *this;
        }
    };

    template <char... Cs>
    struct exactly {
        static constexpr char pattern[] = { Cs... };
        static constexpr std::size_t N = sizeof...(Cs);

        std::size_t pos = 0;
        bool dead = false;

        bool possible () const {
            return !dead;
        }

        bool valid () const {
            return !dead && pos == N;
        }

        exactly step (string_view, char c) const {
            exactly next = *this;

            if (pos >= N || c != pattern[pos]) {
                next.dead = true;
                return next;
            }

            next.pos++;
            return next;
        }
    };

    template <char c> using one = exactly<c>;

    template <Machine sub, size_t max>
    struct max_size {
        sub Sub;
        size_t Size;

        bool possible () const {
            return Sub.possible () && Size <= max;
        }

        bool valid () const {
            return Sub.valid () && Size <= max;
        }

        max_size step (string_view prefix, char c) const {
            return max_size {Sub.step (prefix, c), Size + 1};
        }

    };

    template <Machine M, Machine... Ms>
    struct sequence {
        tuple<M, Ms...> machines;
        size_t active = 0;

        // the last size of accepted characters that was valid so far.
        // if empty, then there is no size that is valid.
        maybe<size_t> last = std::get<0> (machines).valid () ? maybe<int> {0} : maybe<int> {};

        // the number of characters that have been accepted by the previous machines.
        size_t accepted = 0;

        // the number of characters that have been read.
        size_t read = 0;

        bool possible () const {
            return active <= sizeof... (Ms);
        }

        bool valid () const {
            // if the active index is past the end of the sequence, then
            // the pattern is valid if we have accepted the same number
            // of characters that we have read.
            if (!possible ()) return accepted == read;

            // the pattern is valid if the active machine is valid and every subsequent machine is also valid.
            for (size_t i = active; i <= sizeof... (Ms); i++)
                if (!apply_at (machines, [] (const auto &m) -> bool {
                    return m.valid ();
                }, i)) return false;

            return true;
        }

        sequence step (string_view prefix, char c) const;
    };

    template <Machine... Ms> struct alternatives {
        std::tuple<Ms...> machines;

        std::size_t active = 0;

        bool possible () const {
            if (active >= sizeof... (Ms)) return false;
            return apply_at (machines, [] (const auto &x) -> bool {
                return x.possible ();
            }, active);
        }

        bool valid () const {
            if (active >= sizeof... (Ms)) return false;
            return apply_at (machines, [] (const auto &x) -> bool {
                return x.valid ();
            }, active);
        }

        alternatives step (string_view prefix, char c) const;

    };

    template <Machine Sub, size_t min = 0, size_t max = std::numeric_limits<size_t>::max ()>
    requires (min <= max)
    struct repeated {
        Sub machine;

        // number of repetitions matched so far.
        size_t repetitions = 0;

        // characters accepted so far in the current iteration.
        maybe<size_t> last = machine.valid () ? maybe<size_t> {0} : maybe<size_t> {};

        // the characters we have read so far in the current iteration.
        size_t read = 0;

        // the size of the prefix that was accepted by previous iterations.
        size_t previously_accepted = 0;

        bool possible () const {
            return repetitions < max && machine.possible ();
        }

        bool valid () const {
            return
                // if the sub machine is valid then we can increment the
                // number of repetitions if necessary, so the whole pattern
                // is valid if the number of repetitions is one less than the minimum.
                bool (last) && *last == read && repetitions < max && repetitions + 1 >= min ||
                // if the sub machine is not valid but we haven't read any characters
                // in this iteration yet, then the pattern is valid if we have counted
                // at least the minimum number of repetitions.
                !bool (last) && read == 0 && repetitions <= max && repetitions >= min ||
                // if we do not have the minimum required number of repetitions, the
                // pattern is valid if we have read no characters on the current iteration
                // and the sub machine is valid (meaning it accepts the empty string.)
                bool (last) && *last == 0 && read == 0 && repetitions < max;
        }

        repeated step (string_view prefix, char c) const;

    };

    template <Machine sub> using optional = repeated<sub, 0, 1>;
    template <Machine sub> using plus = repeated<sub, 1>;
    template <Machine sub> using star = repeated<sub>;

    template <Machine M, Machine... Ms>
    sequence<M, Ms...>
    sequence<M, Ms...>::step (string_view prefix, char c) const {
        sequence next = *this;

        if (!possible ()) return next;

        if (apply_at (next.machines,
            [&](auto &m) -> bool {
                ++next.read;
                auto pre = prefix.substr (next.accepted, prefix.size () - next.accepted);
                m = m.step (pre, c);
                if (m.valid ()) next.last = next.read;
                return m.possible ();
            }, next.active)) return next;

        while (true) {
            // if there was no last valid state, then we
            // make the next state invalid and return.
            if (!bool (next.last)) {
                next.active = sizeof... (Ms) + 1;
                return next;
            }

            ++next.active;
            next.accepted = *next.last;
            next.read = next.accepted;

            if (!next.possible ()) return next;

            if (apply_at (next.machines,
                [&](auto &m) -> bool {
                    if (!m.valid ()) next.last = maybe<int> {};

                    for (int i = next.accepted; i < prefix.size (); i++) {
                        ++next.read;
                        auto pre = prefix.substr (next.accepted, i - next.accepted);
                        m = m.step (pre, prefix[i]);
                        if (m.valid ()) next.last = next.read;
                        if (!m.possible ()) return false;
                    }

                    ++next.read;

                    auto pre = prefix.substr (next.accepted, prefix.size () - next.accepted);
                    m = m.step (pre, c);
                    if (m.valid ()) next.last = next.read;
                    return m.possible ();
                }, next.active)) return next;
        }
    }

    template <Machine Sub, size_t min, size_t max>
    requires (min <= max)
    repeated<Sub, min, max> repeated<Sub, min, max>::step (string_view prefix, char c) const {

        if (!possible ()) return *this;

        repeated next = *this;

        while (true) {
            ++next.read;
            if (!next.possible ()) return next;

            auto pre = prefix.substr (next.previously_accepted, prefix.size () - next.previously_accepted);
            next.machine = next.machine.step (pre, c);

            if (next.machine.valid ()) next.last = next.read;

            // if it is possible to add more characters, then we are done
            // because we have no more. If the sub machine was never
            // valid, then we are in an invalid state and can't increment.
            if (next.machine.possible () || !bool (next.last)) return next;

            ++next.repetitions;

            next.previously_accepted += *next.last;

            next.machine = Sub {};

            next.last = next.machine.valid () ? maybe<size_t> {0} : maybe<size_t> {};
            next.read = 0;

            // replay all characters from the last valid state until now.
            for (int i = next.previously_accepted; i < prefix.size (); i++) {
                ++next.read;
                if (!next.possible ()) return next;
                next.machine = next.machine.step (prefix.substr (next.previously_accepted, i - next.previously_accepted), prefix[i]);
                if (next.machine.valid ()) next.last = next.read;
            }
        }
    }

    template <Machine... Ms>
    alternatives<Ms...>
    alternatives<Ms...>::step (string_view prefix, char c) const {
        alternatives next = *this;

        if (next.active >= sizeof... (Ms)) return next;

        if (apply_at (next.machines,
            [&](auto &m) -> bool {
                m = m.step (prefix, c);
                return m.possible () ? true : m.valid ();
            }, next.active)) return next;

        while (++next.active < sizeof... (Ms)) {
            if (apply_at (next.machines,
                [&](auto &m) -> bool {
                    for (int i = 1; i <= prefix.size (); i++) {
                        m = m.step (prefix.substr (0, i - 1), prefix[i - 1]);
                        if (!m.possible ()) return false;
                    }

                    m = m.step (prefix, c);

                    return m.possible () ? true : m.valid ();
                }, next.active)) return next;
        }

        return next;
    }

}

#endif
