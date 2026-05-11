// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_PARSE_CONTROL
#define DATA_PARSE_CONTROL

#include <data/parse/accept.hpp>
#include <data/tuple.hpp>
#include <data/maybe.hpp>

namespace data::parse {

    // accept no string (even the empty string)
    struct invalid;

    // accept every string.
    struct any;

    template <typename Sub, size_t ...> struct repeated;

    template <typename Sub, size_t min, size_t max> requires (min <= max) struct repeated<Sub, min, max>;

    template <typename Sub, size_t exactly> struct repeated<Sub, exactly> : repeated<Sub, exactly, exactly> {};

    constexpr const size_t unlimited = std::numeric_limits<size_t>::max ();

    template <typename Sub> struct repeated<Sub> : repeated<Sub, 0, unlimited> {};

    template <typename sub, size_t max> struct most : repeated<sub, 0, max> {};
    template <typename sub, size_t min> struct least : repeated<sub, min, unlimited> {};

    template <typename sub> struct optional : repeated<sub, 0, 1> {};
    template <typename sub> struct plus : least<sub, 1> {};
    template <typename sub> struct star : least<sub, 0> {};

    // run each alternative in parallel. If more than one is
    // valid, the first valid one is accepted.
    template <typename... Ms> class alternatives;

    // accept a string with a given max size (in chars)
    template <typename sub, size_t max> struct max_size;

    // must match X and must not match Y.
    template <typename X, typename Y> struct complement;

    // Match X ending in Y. (Thus Y must be part of X)
    template <typename X, char y, char... Y> struct until;

    // must match X and must contain Y as a substring.
    template <typename X, char... Y> struct contains;

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

    template <typename... Ms> class alternatives {
        mutable std::tuple<Ms*...> machines {};

    public:
        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr void step (string_view prefix, char c);

        ~alternatives () {
            for_each ([] (auto m) {
                if (m != nullptr) delete m;
            }, machines);
        }

        alternatives (): machines {} {
            for_each ([] (auto m) {
                m = nullptr;
            }, machines);
        }

        alternatives (const alternatives &) = delete;
        alternatives (alternatives &&) = delete;
    };

    // accept a string with a given max size (in chars)
    template <typename sub, size_t max> struct max_size {
        sub Sub;
        size_t Size;

        constexpr bool possible () const;
        constexpr bool valid () const;
        constexpr void step (string_view prefix, char c);

    };

    template <typename Sub, size_t min, size_t max> requires (min <= max)
    struct repeated<Sub, min, max> {
        Sub *machine = new Sub {};

        // number of repetitions matched so far.
        size_t repetitions = 0;

        // characters accepted so far in the current iteration.
        maybe<size_t> last = machine->valid () ? maybe<size_t> {0} : maybe<size_t> {};

        // the characters we have read so far in the current iteration.
        size_t read = 0;

        // the size of the prefix that was accepted by previous iterations.
        size_t previously_accepted = 0;

        constexpr bool possible () const {
            return repetitions < max && machine->possible ();
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

        repeated () {}

        constexpr ~repeated () {
            delete machine;
        }

        repeated (const repeated &) = delete;
        repeated (repeated &&) = delete;

    };

    template <typename X, typename Y>
    struct complement {
        X x;
        Y y;

        constexpr void step (std::string_view sv, char c) {
            x.step (sv, c);
            y.step (sv, c);
        }

        constexpr bool possible () const {
            return x.possible ();
        }

        constexpr bool valid () const {
            return x.valid () && !y.valid ();
        }
    };

    // match X ending with Y.
    template <typename X, char y, char... Y> struct until {
        X Sub;

        static constexpr char term[] = { y, Y... };
        static constexpr size_t N = sizeof...(Y) + 1;

        size_t Index = 0;        // prefix matched
        bool Halted = false;

        constexpr void step (std::string_view sv, char c) {
            if (Halted) {
                Index = 0; // invalidate pattern
                return;
            }

            Sub.step (sv, c);

            if (c == term[Index]) {
                Index++;
                if (Index == N) Halted = true;
            } //else Index = 0;
            else {

                for (size_t i = sv.size () - Index; i < sv.size (); i++) {
                    Index = 0;

                    for (size_t j = i + 1; j < sv.size (); j++) {
                        if (sv[j] == term[Index]) Index++;
                        else goto cont;
                    }

                    if (c == term[Index]) {
                        Index++;

                        return;
                    }

                    cont:
                }

                Index = 0;

                if (c == term[Index]) {
                    Index++;
                    return;
                }
            }

        }

        constexpr bool possible () const {
            return !Halted && Sub.possible ();
        }

        constexpr bool valid () const {
            // Valid if we halted exactly at terminator
            return Halted && Index == N && Sub.valid ();
        }
    };

    template <typename X, char... Y>
    struct contains {
        X unit;

        static constexpr char term[] = { Y... };
        static constexpr size_t N = sizeof...(Y);

        size_t k = 0;        // prefix matched

        constexpr void step (std::string_view sv, char c) {
            unit.step (sv, c);

            if (k == N) return;

            if (c == term[k]) {
                k++;
                return;
            }

            for (size_t i = sv.size () - k; i < sv.size (); i++) {
                k = 0;

                for (size_t j = i + 1; j < sv.size (); j++) {
                    if (sv[j] == term[k]) k++;
                    else goto cont;
                }

                if (c == term[k]) {
                    k++;

                    return;
                }

                cont:
            }

        }

        constexpr bool possible () const {
            return unit.possible ();
        }

        constexpr bool valid () const {
            // Valid if we halted exactly at terminator
            return k == N && unit.valid ();
        }
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

    // the machine is possible if any sub machine is possible.
    template <typename... Ms>
    constexpr bool inline alternatives<Ms...>::possible () const {
        for (int i = 0; i < sizeof... (Ms); i++)
            if (apply_at (machines, [] (auto &m) {
                if (m == nullptr) m = new unref<decltype (*m)> {};
                return m->possible ();
            }, i)) return true;
        return false;
    }

    // and valid if any sub machine is valid.
    template <typename... Ms>
    constexpr bool inline alternatives<Ms...>::valid () const {
        for (int i = 0; i < sizeof... (Ms); i++)
            if (apply_at (machines, [] (auto &m) {
                if (m == nullptr) m = new unref<decltype (*m)> {};
                return m->valid ();
            }, i)) return true;
        return false;
    }

    template <typename... Ms>
    constexpr void alternatives<Ms...>::step (string_view prefix, char c) {
        for_each ([&] (auto &m) {
            if (m == nullptr) m = new unref<decltype (*m)> {};
            m->step (prefix, c);
        }, machines);
    }

    template <typename sub, size_t max>
    constexpr bool inline max_size<sub, max>::possible () const {
        return Sub.possible () && Size <= max;
    }

    template <typename sub, size_t max>
    constexpr bool inline max_size<sub, max>::valid () const {
        return Sub.valid () && Size <= max;
    }

    template <typename sub, size_t max>
    constexpr void inline max_size<sub, max>::step (string_view prefix, char c) {
        Size++;
        Sub.step (prefix, c);
    }

    // TODO we incorrectly always return 1 here. In this commit,
    // we are ignoring the return value of step. We will get all
    // current tests working and then use the return value correctly.
    template <typename Sub, size_t min, size_t max> requires (min <= max)
    constexpr void repeated<Sub, min, max>::step (string_view prefix, char c) {
        while (true) {

            ++read;
            if (!possible ()) return;

            machine->step (prefix.substr (previously_accepted, prefix.size () - previously_accepted), c);

            if (machine->valid ()) last = read;

            // if it is possible to add more characters, then we are done
            // because we have no more. If the sub machine was never
            // valid, then we are in an invalid state and can't increment.
            if (machine->possible () || !bool (last)) return;

            ++repetitions;

            previously_accepted += *last;

            delete machine;
            machine = new Sub {};

            last = machine->valid () ? maybe<size_t> {0} : maybe<size_t> {};
            read = 0;

            // in this case, the char was accepted and
            if (previously_accepted > prefix.size ()) return;
            // replay all characters from the last valid state until now.
            for (int i = previously_accepted; i < prefix.size (); i++) {
                ++read;
                if (!possible ()) return;

                machine->step (prefix.substr (previously_accepted, i - previously_accepted), prefix[i]);
                if (machine->valid ()) last = read;
            }
        }
    }

}

#endif
