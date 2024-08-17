// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_DISTRIBUTED_SEARCH
#define DATA_TOOLS_DISTRIBUTED_SEARCH

#include <data/iterable.hpp>
#include <data/cross.hpp>

namespace data::distributed {

    // return a future to a solution to a problem that will be solved by searching a space
    // of solutions over many threads. This function blocks but an another farther down is
    // non-blocking.
    template <typename X, std::random_access_iterator iterator> requires requires (iterator &i) {
        { *i } -> std::convertible_to<const X &> ;
    } maybe<X> search (uint32 threads, function<bool (const X &)> trial, iterator begin, iterator end);

    template <typename iterator> struct parameter_range {
        iterator Begin;
        iterator End;

        operator bool () const {
            return (End - Begin);
        }
    };

    template <typename X, std::random_access_iterator iterator> class running;

    template <typename X, std::random_access_iterator iterator>
    class searcher : std::enable_shared_from_this<searcher> {
        std::mutex Mutex {};
        bool Closed {false};
        int IterationRange {25000};
        std::promise<maybe<X>> Promise {}

        iterator Next;
        iterator End;

    public:

        // Get a new iterator range to search throuogh.
        // range will be zero if the channel is closed.
        parameter_range<iterator> poll ();

        // submit a solution.
        void solve (const X &solution);

        // close the channel.
        void close ();

        bool closed () const;

        searcher (iterator begin, iterator end) : Next {begin}, End {end} {}

        ptr<running<X, iterator>> start (uint32 threads, function<bool (const X &)> trial);
    };

    // search in the background with the option to wait for the solution.
    template <typename X, std::random_access_iterator iterator> requires requires (iterator &i) {
        { *i } -> std::convertible_to<const X &> ;
    } ptr<running<X, iterator>> inline search_in_background (uint32 threads, function<bool (const X &)> trial, iterator begin, iterator end) {
        return std::make_shared<searcher<X, iterator>> (begin, end)->start (threads, trial);
    }

    template <typename X, std::random_access_iterator iterator> struct running {
        std::future<maybe<X>> Future;
        running (uint32 threads, function<bool (const X &)> trial, std::future<maybe<X>> &&f, ptr<searcher<X, iterator>> x);
        ~running ();
    private:
        ptr<searcher<X, iterator>> Searcher;
        cross<std::thread> Workers;
    };

    template <typename X, std::random_access_iterator iterator> requires requires (iterator &i) {
        { *i } -> std::convertible_to<const X &> ;
    } maybe<X> search (uint32 threads, function<bool (const X &)> trial, iterator begin, iterator end) {
        return search_in_background (threads, trial, begin, end)->Future.get ();
    }

    template <typename X, std::random_access_iterator iterator>
    parameter_range<iterator> channel<X, iterator>::poll () {
        // no new locks can be created until this lock is destructed.
        std::unique_lock<std::mutex> lock (Mutex);
        if (Next == End) {
            if (!Closed) {
                Closed = true;
                wait_for_shutdown ();
            }

            return parameter_range<iterator> {End, End};
        }
        iterator next = Next + IterationRange;
        if (next > End) next = End;
        return parameter_range<iterator> {Next, Next = next};
    }

    template <typename X, std::random_access_iterator iterator>
    void searcher<X, iterator>::close () {
        std::unique_lock<std::mutex> lock (Mutex);
        if (!Closed) {
            Closed = true;
            Promise.fulfill ({});
            wait_for_shutdown ();
        }
    }

    template <typename X, std::random_access_iterator iterator>
    void searcher<X, iterator>::solve (const X &solution) {
        std::unique_lock<std::mutex> lock (Mutex);
        if (Closed) return;
        Closed = true;
        Promise.fulfill (solution);
        wait_for_shutdown ();
    }

    template <typename X, std::random_access_iterator iterator>
    bool searcher<X, iterator>::closed () const {
        std::unique_lock<std::mutex> lock (Mutex);
        return Closed;
    }

    template <typename X, std::random_access_iterator iterator>
    void search_thread (channel<X, iterator> &x, function<bool (const X &)> trial) {
        try {
            while (true) {
                parameter_range<iterator> r = x->poll ();
                while (bool (r)) {
                    if (trial (*r.Begin)) {
                        x->solve (*r.Begin);
                        return;
                    }
                }

                if (x->closed ()) return;
            }
        } catch (const std::exception &x) {
            std::cout << "Error " << x.what () << std::endl;
        }
    }

    template <typename X, std::random_access_iterator iterator>
    ptr<running<X, iterator>> start (uint32 threads, function<bool (const X &)> trial) {
        return std::make_shared<running> (threads, trial, Promise.get_future (), this->shared_from_this ());
    }

    template <typename X, std::random_access_iterator iterator>
    maybe<X> running<X, iterator>::~running () {
        Searcher->close ();
        for (auto &w: Workers) w.join ();
    }

    template <typename X, std::random_access_iterator iterator>
    running<X, iterator>::running (uint32 threads, function<bool (const X &)> trial, std::future<maybe<X>> &&f, ptr<searcher<X, iterator>> x):
        Future {f}, Searcher {x} {

        for (int i = 0; i < threads; i++) Workers.emplace_back (&search_thread, *x, trial);
    }

}
