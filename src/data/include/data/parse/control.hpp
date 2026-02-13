
#ifndef DATA_PARSE_CONTROL
#define DATA_PARSE_CONTROL

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
                if (!state.valid ()) {
                    in.setstate (std::ios::failbit);
                }
                return buf;
            }

            char c = traits::to_char_type (p);
            State next = state.step (buf, c);

            if (!next.possible ()) {
                if (!next.valid ()) {
                    in.setstate (std::ios::failbit);
                } else {
                    in.get ();
                    buf.push_back (c);
                }

                return buf;
            }

            in.get ();
            buf.push_back (c);
            state = std::move (next);
        }
    }

    template <Machine State>
    bool inline accept (std::stringstream &ss) {
        State state {};
        read_token (ss, state);
        return bool (ss);
    }

    template <Machine State> bool inline accept (const std::string &x) {
        std::stringstream ss {x};
        return accept<State> (ss) && ss.peek () == std::char_traits<char>::eof ();
    }

    template <Machine State> bool inline accept (const std::u8string &x) {
        return accept<State> (std::string (reinterpret_cast<const char*> (x.data ()), x.size ()));
    }

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

    template <Machine...> struct sequence;

    using empty = sequence<>;

    template <Machine M>
    struct sequence<M> {
        M machine;

        bool possible () const {
            return machine.possible ();
        }

        bool valid () const {
            return machine.valid ();
        }

        sequence step (string_view prefix, char c) const {
            return sequence {machine.step (prefix, c)};
        }
    };

    template <> struct sequence<> {
        bool Valid = true;

        bool possible () const {
            return false;
        }

        bool valid () const {
            return Valid;
        }

        sequence step (string_view prefix, char c) const {
            return sequence {false};
        }
    };

    template <Machine M, Machine... Ms>
    struct sequence<M, Ms...> {
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
                bool (last) && *last == 0 && read == 0 && repetitions <= max;
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

        size_t replay = 0;

        while (true) {

            // try to apply the char to the current sequence.
            if (apply_at (next.machines,
                [&](auto &m) -> bool {

                    // we never do this in the initial round of the loop.
                    for (int i = 0; i < replay; i++) {
                        ++next.read;
                        if (!next.possible ()) return true;

                        // we drop the characters accepted by previous
                        // patterns in this sequence.
                        m = m.step (prefix.substr (next.accepted, i), prefix[next.accepted + i]);

                        bool is_valid = m.valid ();
                        if (is_valid) next.last = next.read;

                        // if it not possible to advance the pattern, then
                        // we have to replay variables.
                        if (!m.possible ()) return false;
                    }

                    // advance the number of characters read.
                    // this will invalidate the pattern if it was valid.
                    ++next.read;

                    // if it not possible to accept another character,
                    // then we stop and return an invalid pattern.
                    if (!next.possible ()) return true;

                    m = m.step (prefix.substr (next.accepted, prefix.size () - next.accepted), c);
                    bool is_valid = m.valid ();
                    // We note where the pattern is last valid so that
                    // we can later replay chars if we need to.
                    if (is_valid) next.last = next.read;
                    // if it is impossible to add more characters and
                    // the sub pattern is not valid, then we have to
                    // replay the current character and maybe some earlier
                    // ones to the next machine. Otherwise we can continue.
                    return m.possible () ? true : is_valid;
                }, next.active)) return next;

            // if there was no last valid state, then we
            // invalidate this pattern and return.
            if (!bool (next.last)) {
                next.active = sizeof... (Ms) + 1;
                return next;
            }

            next.accepted = *next.last;

            // increment active index.
            ++next.active;

            // In this case we have to be done because
            // we are at the end of the sequence.
            if (next.active > sizeof... (Ms)) return next;

            // revert the number of read characters to the number of accepted characters.
            // we have to replay from here through the prefix.
            next.read = next.accepted;

            // in this case, the latest character has been accepted by the previous
            // pattern, so we stop here. (can this really happen now?)
            if (next.accepted > prefix.size ()) return next;

            replay = prefix.size () - next.accepted;

            // we now set the last point appropriately.
            apply_at (next.machines,
                [&](auto &m) {
                    next.last = m.valid () ? maybe<int> {0} : maybe<int> {};
                }, next.active);
        }
    }

    template <Machine Sub, size_t min, size_t max>
    requires (min <= max)
    repeated<Sub, min, max> repeated<Sub, min, max>::step (string_view prefix, char c) const {

        repeated next = *this;

        while (true) {

            ++next.read;
            if (!next.possible ()) return next;

            next.machine = next.machine.step (prefix.substr (next.previously_accepted, prefix.size () - next.previously_accepted), c);

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

            // in this case, the char was accepted and
            if (next.previously_accepted > prefix.size ()) return next;
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
