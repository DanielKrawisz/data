// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_PARSE_SEQUENCE
#define DATA_PARSE_SEQUENCE

#include <data/parse/control.hpp>

namespace data::parse {

    // accept a sequence of patterns.
    template <typename...> struct sequence;

    // accept the empty string
    using empty = sequence<>;

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

    template <typename M> struct sequence<M> {
        M machine;

        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr void step (string_view prefix, char c);
    };

    template <typename M, typename... Ms>
    class sequence<M, Ms...> {
        // if we tried to instantiate all of these at once,
        // we would have an infinite value.
        mutable tuple<M*, Ms*...> machines {};

        // the index of machines that is currently active.
        size_t active = 0;

        // the last size of accepted characters that was valid so far.
        // if empty, then there is no size that is valid.
        maybe<size_t> last {};

        // the number of characters that have been accepted by the previous machines.
        size_t accepted = 0;

        // the number of characters that have been read.
        size_t read = 0;

        void delete_all () {
            // the previous machine is always deleted when we increment active.
            // thus, we only have to delete starting at active. However, machines
            // past the active one may have been constructed as well, so we
            // have to delete to the end.
            for (size_t j = active; j <= sizeof... (Ms); j++) {
                apply_at (machines, [&] (auto &m) {
                    delete m;
                }, j);
            }
        }

    public:
        constexpr bool possible () const;

        constexpr bool valid () const;

        constexpr void step (string_view prefix, char c);

        sequence () {
            auto &m = std::get<0> (machines);
            m = new M {};
            if (m->valid ()) last = size_t {0};
        }

        ~sequence () {
            delete_all ();
        }

        sequence (const sequence &) = delete;
        sequence (sequence &&) = delete;
    };

    template <typename M>
    constexpr bool sequence<M>::possible () const {
        return machine.possible ();
    }

    template <typename M>
    constexpr bool sequence<M>::valid () const {
        return machine.valid ();
    }

    template <typename M>
    constexpr void sequence<M>::step (string_view prefix, char c) {
        return machine.step (prefix, c);
    }

    template <typename M, typename... Ms>
    constexpr bool sequence<M, Ms...>::possible () const {
        // The sequence is possible if, of the machines at
        // or past the active one, at least one is possible
        // and the rest are either valid or possible.
        bool at_least_one_possible = false;

        for (size_t i = active; i <= sizeof... (Ms); i++)
            if (!apply_at (machines, [&] (auto &m) -> bool {
                if (m == nullptr)
                    m = new unref<decltype (*m)> {};

                if (m->possible ()) {
                    at_least_one_possible = true;
                    return true;
                }

                return m->valid ();
            }, i)) return false;

        return at_least_one_possible;
    }

    template <typename M, typename... Ms>
    constexpr bool sequence<M, Ms...>::valid () const {
        // if the active index is past the end of the sequence, then
        // the pattern is valid if we have accepted the same number
        // of characters that we have read.
        if (active > sizeof... (Ms))
            return read == 0;

        // the pattern is valid if the active machine is valid and every subsequent machine is also valid.
        for (size_t i = active; i <= sizeof... (Ms); i++)
            if (!apply_at (machines, [&] (auto &m) -> bool {
                if (m == nullptr) m = new unref<decltype (*m)> {};
                    bool is_valid = m->valid ();
                    return is_valid;
            }, i)) return false;

        return true;
    }

    template <typename M, typename... Ms>
    constexpr void sequence<M, Ms...>::step (string_view prefix, char c) {
        size_t replay = 0;

        if (active > sizeof ... (Ms)) {
            ++read; // we have to do this in order to make the pattern invalid.
            return;
        }

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
                        // here. We must take it into account.

                        // TODO: I forgot why we need to do this so now I need a
                        // test case.
                        m->step (prefix.substr (accepted, i), prefix[accepted + i]);

                        if (m->valid ()) last = read;

                        // if it not possible to advance the pattern, then
                        // we have to replay variables.
                        if (!m->possible ()) return false;
                    }

                    // advance the number of characters read.
                    // this will invalidate the pattern if it was valid.
                    ++read;

                    // if it not possible to accept another character,
                    // then we stop and continue.
                    if (!m->possible ()) return false;

                    m->step (prefix.substr (accepted, prefix.size () - accepted), c);

                    bool is_valid = m->valid ();
                    // We note where the pattern is last valid so that
                    // we can later replay chars if we need to.
                    if (is_valid) last = read;

                    // if it is impossible to add more characters and
                    // the sub pattern is not valid, then we have to
                    // replay the current character and maybe some earlier
                    // ones to the next machine. Otherwise we can continue.
                    return m->possible () ? true : is_valid;
                }, active)) return;

            // if there was no last valid state, then we
            // invalidate this pattern and return.
            if (!bool (last)) {
                delete_all ();
                active = sizeof... (Ms) + 1;
                return;
            }

            accepted += *last;

            // delete active machine.
            apply_at (machines, [&](auto &m) {
                delete m;
            }, active);

            // increment active index.
            ++active;

            // In this case we have to be done because
            // we are at the end of the sequence.
            if (active > sizeof... (Ms)) return;

            // revert the number of read characters to the number of accepted characters.
            // we have to replay from here through the prefix.
            read = 0;

            // in this case, the latest character has been accepted by the previous
            // pattern, so we stop here. (can this really happen now?)
            if (accepted > prefix.size ()) return;

            replay = prefix.size () - accepted;

            // we now set the last point appropriately.
            apply_at (machines,
                [&](auto &m) {
                    if (m == nullptr) m = new unref<decltype (*m)> {};
                    last = m->valid () ? maybe<int> {0} : maybe<int> {};
                }, active);
        }
    }

}

#endif
