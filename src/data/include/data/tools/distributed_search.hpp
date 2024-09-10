// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_DISTRIBUTED_SEARCH
#define DATA_TOOLS_DISTRIBUTED_SEARCH

#include <data/iterable.hpp>
#include <data/cross.hpp>
#include <data/function.hpp>
#include <future>

namespace data::distributed {

    using std::future;
    using std::promise;
    using std::mutex;
    using std::thread;
    using lock = std::unique_lock<mutex>;

    template <typename X, typename it> using test = function<maybe<X> (const it &)>;

    // return a future to a solution to a problem that will be solved by searching a space
    // of solutions over many threads. This function blocks but an another farther down is
    // non-blocking.
    template <typename X, std::forward_iterator it>
    maybe<X> search (uint32 threads, test<X, it> trial, it begin, it end, int iteration_range = 10000);

    template <typename it> struct parameter_range {
        it Begin;
        it End;

        operator bool () const {
            return (Begin != End);
        }
    };

    template <typename X, std::forward_iterator it> class running;

    template <typename X, std::forward_iterator it>
    class searcher : std::enable_shared_from_this<searcher<X, it>> {
    private:
        mutable mutex Mutex {};
        bool Closed {false};
        bool Solved {false};
        promise<X> Promise {};

        it Next;
        it End;
        int IterationRange;

    public:
        // Get a new iterator range to search throuogh.
        // range will be zero if the channel is closed.
        parameter_range<it> get ();

        // submit a solution.
        void solve (const X &solution);

        // close the channel.
        void close ();

        bool closed () const;

        bool solved () const;

        searcher (it begin, it end, int iteration_range = 10000) :
            Next {begin}, End {end}, IterationRange {iteration_range} {}

        friend class running<X, it>;
    };

    // search in the background with the option to wait for the solution.
    template <typename X, std::forward_iterator it>
    ptr<running<X, it>> inline search_in_background
        (uint32 threads, test<X, it> trial, it begin, it end, int iteration_range = 10000) {
        auto xx = std::make_shared<searcher<X, it>> (begin, end, iteration_range);
        return std::make_shared<running<X, it>> (threads, trial, xx);
    }

    template <typename X, std::forward_iterator it> struct running {
        running (uint32 threads, test<X, it> trial, ptr<searcher<X, it>> x);
        ~running ();
        maybe<X> wait ();
    private:
        ptr<searcher<X, it>> Searcher;
        cross<thread> Workers;
        future<X> Future;
    };

    template <typename X, std::forward_iterator it>
    maybe<X> inline search (uint32 threads, test<X, it> trial, it begin, it end, int iteration_range) {
        return search_in_background (threads, trial, begin, end, iteration_range)->wait ();
    }

    template <typename X, std::forward_iterator it>
    parameter_range<it> searcher<X, it>::get () {
        // no new locks can be created until this lock is destructed.
        lock _ (Mutex);

        if (Next == End) {
            if (!Closed) Closed = true;

            return parameter_range<it> {End, End};
        }

        it next = Next;
        for (int i = 0; i < IterationRange; i++) {
            next++;
            if (next == End) break;
        }

        return parameter_range<it> {Next, Next = next};
    }

    template <typename X, std::forward_iterator it>
    void searcher<X, it>::close () {
        lock _ (Mutex);

        if (!Closed) {
            Closed = true;
            Promise.set_value ({});
        }
    }

    template <typename X, std::forward_iterator it>
    void searcher<X, it>::solve (const X &solution) {
        lock _ (Mutex);
        if (Closed) return;
        Closed = true;
        Solved = true;
        Promise.set_value (solution);
    }

    template <typename X, std::forward_iterator it>
    bool inline searcher<X, it>::closed () const {
        lock _ (Mutex);
        return Closed;
    }

    template <typename X, std::forward_iterator it>
    bool inline searcher<X, it>::solved () const {
        lock _ (Mutex);
        return Solved;
    }

    template <typename X, std::forward_iterator it>
    void search_thread (searcher<X, it> *x, test<X, it> trial, uint32 index) {
        try {
            while (true) {
                parameter_range<it> r = x->get ();
                while (bool (r)) {
                    auto solution = trial (r.Begin);
                    if (bool (solution)) {
                        x->solve (*solution);
                        return;
                    }
                    r.Begin++;
                }

                if (x->closed ()) return;
            }
        } catch (const std::exception &z) {
            std::cout << "Error " << z.what () << std::endl;
        } catch (...) {
            std::cout << "unknown error caught" << std::endl;
        }
    }

    template <typename X, std::forward_iterator it>
    maybe<X> running<X, it>::wait () {
        for (auto &w: Workers) w.join ();
        if (Searcher->solved ()) return Future.get ();
        return {};
    }

    template <typename X, std::forward_iterator it>
    inline running<X, it>::~running () {
        Searcher->close ();
        for (auto &w: Workers) w.join ();
    }

    template <typename X, std::forward_iterator it>
    running<X, it>::running
        (uint32 threads, test<X, it> trial, ptr<searcher<X, it>> x):
        Future {x->Promise.get_future ()}, Searcher {x} {

        for (int i = 0; i < threads; i++) Workers.emplace_back (search_thread<X, it>, x.get (), trial, i);
    }

}

#endif

