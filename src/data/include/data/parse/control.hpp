
#ifndef DATA_PARSE_CONTROL
#define DATA_PARSE_CONTROL

#include <data/parse/accept.hpp>
#include <data/tuple.hpp>
#include <data/maybe.hpp>

namespace data::parse {

    // accept no string (even the empty string)
    struct invalid {
        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr int step (string_view, char c);
    };

    // accept every string.
    struct any {
        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr int step (string_view, char c);
    };

    // accept a sequence of patterns.
    template <Machine...> struct sequence;

    // accept the empty string
    using empty = sequence<>;

    template <Machine Sub, size_t min = 0, size_t max = std::numeric_limits<size_t>::max ()>
    requires (min <= max) struct repeated;

    template <Machine sub> struct optional : repeated<sub, 0, 1> {};
    template <Machine sub> struct plus : repeated<sub, 1> {};
    template <Machine sub> struct star : repeated<sub> {};
    template <size_t size, typename m> struct rep : repeated<m, size, size> {};

    template <Machine... Ms> struct alternatives {
        std::tuple<Ms...> machines;

        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr void step (string_view prefix, char c);

    };

    // accept a string with a given max size (in chars)
    template <Machine sub, size_t max> struct max_size {
        sub Sub;
        size_t Size;

        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr void step (string_view prefix, char c);

    };

    template <Machine M> struct sequence<M> {
        M machine;

        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr void step (string_view prefix, char c);
    };

    template <> struct sequence<> {
        bool Valid = true;

        constexpr bool possible () const {
            return false;
        }

        constexpr bool valid () const {
            return Valid;
        }

        constexpr void step (string_view prefix, char c) {
            Valid = false;
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

        constexpr bool possible () const {
            return active <= sizeof... (Ms);
        }

        constexpr bool valid () const {
            // if the active index is past the end of the sequence, then
            // the pattern is valid if we have accepted the same number
            // of characters that we have read.
            if (!possible ()) return accepted == read;

            // the pattern is valid if the active machine is valid and every subsequent machine is also valid.
            for (size_t i = active; i <= sizeof... (Ms); i++)
                if (!apply_at (machines, [&] (const auto &m) -> bool {
                    return m.valid ();
                }, i)) return false;

            return true;
        }

        constexpr void step (string_view prefix, char c);
    };

    template <Machine Sub, size_t min, size_t max>
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

        constexpr bool possible () const {
            return repetitions < max && machine.possible ();
        }

        constexpr bool valid () const {
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

        constexpr void step (string_view prefix, char c);

    };

    constexpr bool inline invalid::possible () const {
        return false;
    }

    constexpr bool inline invalid::valid () const {
        return false;
    }

    constexpr int inline invalid::step (string_view, char c) {
        return 0;
    }

    constexpr bool inline any::possible () const {
        return true;
    }

    constexpr bool inline any::valid () const {
        return true;
    }

    constexpr int inline any::step (string_view, char c) {
        return 1;
    }

    template <Machine M>
    constexpr bool sequence<M>::possible () const {
        return machine.possible ();
    }

    template <Machine M>
    constexpr bool sequence<M>::valid () const {
        return machine.valid ();
    }

    template <Machine M>
    constexpr void sequence<M>::step (string_view prefix, char c) {
        return machine.step (prefix, c);
    }

    // the machine is possible if any sub machine is possible.
    template <Machine... Ms>
    constexpr bool inline alternatives<Ms...>::possible () const {
        log::indent qqq {};
        DATA_LOG (normal) << "alternatives: test possible";
        for (int i = 0; i < sizeof... (Ms); i++)
            if (apply_at (machines, [] (const auto &m) { return m.possible (); }, i)) return true;
        return false;
    }

    // and valid if any sub machine is valid.
    template <Machine... Ms>
    constexpr bool inline alternatives<Ms...>::valid () const {
        log::indent qqq {};
        DATA_LOG (normal) << "alternatives: test valid";
        for (int i = 0; i < sizeof... (Ms); i++)
            if (apply_at (machines, [] (const auto &m) { return m.valid (); }, i)) return true;
        return false;
    }

    template <Machine... Ms>
    constexpr void alternatives<Ms...>::step (string_view prefix, char c) {
        log::indent qqq {};
        DATA_LOG (normal) << "alternatives: read char " << c;
        for_each (machines, [&] (auto &m) { m.step (prefix, c); });
    }

    template <Machine sub, size_t max>
    constexpr bool inline max_size<sub, max>::possible () const {
        return Sub.possible () && Size <= max;
    }

    template <Machine sub, size_t max>
    constexpr bool inline max_size<sub, max>::valid () const {
        return Sub.valid () && Size <= max;
    }

    template <Machine sub, size_t max>
    constexpr void inline max_size<sub, max>::step (string_view prefix, char c) {
        Size++;
        Sub.step (prefix, c);
    }

    // TODO we incorrectly always return 1 here. In this commit,
    // we are ignoring the return value of step. We will get all
    // current tests working and then use the return value correctly.
    template <Machine M, Machine... Ms>
    constexpr void sequence<M, Ms...>::step (string_view prefix, char c) {
        size_t replay = 0;
        log::indent qqq {};
        DATA_LOG (normal) << "sequence: read char " << c << "; active index is " << active << "; number of machines is " << (sizeof ...(Ms)) + 1;

        if (active > sizeof ... (Ms)) return;

        while (true) {

            // try to apply the char to the current sequence.
            if (apply_at (machines,
                [&](auto &m) -> bool {

                    // we never do this in the initial round of the loop.
                    for (int i = 0; i < replay; i++) {
                        ++read;
                        if (!possible ()) return true;

                        // we drop the characters accepted by previous
                        // patterns in this sequence.
                        // TODO: we incorrectly ignore the value returned by step
                        // here. We will take it into account in the next commit.
                        m.step (prefix.substr (accepted, i), prefix[accepted + i]);

                        bool is_valid = m.valid ();
                        if (is_valid) last = read;

                        // if it not possible to advance the pattern, then
                        // we have to replay variables.
                        if (!m.possible ()) return false;
                    }

                    // advance the number of characters read.
                    // this will invalidate the pattern if it was valid.
                    ++read;

                    // if it not possible to accept another character,
                    // then we stop and return an invalid pattern.
                    if (!possible ()) return true;

                    // TODO: we incorrectly ignore the value returned by step
                    // here. We will take it into account in the next commit.
                    m.step (prefix.substr (accepted, prefix.size () - accepted), c);
                    bool is_valid = m.valid ();
                    // We note where the pattern is last valid so that
                    // we can later replay chars if we need to.
                    if (is_valid) last = read;
                    // if it is impossible to add more characters and
                    // the sub pattern is not valid, then we have to
                    // replay the current character and maybe some earlier
                    // ones to the next machine. Otherwise we can continue.
                    return m.possible () ? true : is_valid;
                }, active)) return;

            // if there was no last valid state, then we
            // invalidate this pattern and return.
            if (!bool (last)) {
                active = sizeof... (Ms) + 1;
                return;
            }

            accepted = *last;

            // increment active index.
            ++active;

            // In this case we have to be done because
            // we are at the end of the sequence.
            if (active > sizeof... (Ms)) return;

            // revert the number of read characters to the number of accepted characters.
            // we have to replay from here through the prefix.
            read = accepted;

            // in this case, the latest character has been accepted by the previous
            // pattern, so we stop here. (can this really happen now?)
            if (accepted > prefix.size ()) return;

            replay = prefix.size () - accepted;

            // we now set the last point appropriately.
            apply_at (machines,
                [&](auto &m) {
                    last = m.valid () ? maybe<int> {0} : maybe<int> {};
                }, active);
        }
    }

    // TODO we incorrectly always return 1 here. In this commit,
    // we are ignoring the return value of step. We will get all
    // current tests working and then use the return value correctly.
    template <Machine Sub, size_t min, size_t max> requires (min <= max)
    constexpr void repeated<Sub, min, max>::step (string_view prefix, char c) {
        while (true) {

            ++read;
            if (!possible ()) return;

            machine.step (prefix.substr (previously_accepted, prefix.size () - previously_accepted), c);

            if (machine.valid ()) last = read;

            // if it is possible to add more characters, then we are done
            // because we have no more. If the sub machine was never
            // valid, then we are in an invalid state and can't increment.
            if (machine.possible () || !bool (last)) return;

            ++repetitions;

            previously_accepted += *last;

            machine = Sub {};

            last = machine.valid () ? maybe<size_t> {0} : maybe<size_t> {};
            read = 0;

            // in this case, the char was accepted and
            if (previously_accepted > prefix.size ()) return;
            // replay all characters from the last valid state until now.
            for (int i = previously_accepted; i < prefix.size (); i++) {
                ++read;
                if (!possible ()) return;

                machine.step (prefix.substr (previously_accepted, i - previously_accepted), prefix[i]);
                if (machine.valid ()) last = read;
            }
        }
    }

}

#endif
