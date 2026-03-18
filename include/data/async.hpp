// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ASYNC
#define DATA_ASYNC
#include <iostream>
#include <functional>
#include <chrono>
#include <data/concepts.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/detached.hpp>

namespace data {

    // we use awaitable for all coroutines.
    template <typename X> using awaitable = boost::asio::awaitable<X>;

    using exec = boost::asio::any_io_executor;

    namespace meta {
        template <typename X>
        struct is_awaitable : std::false_type {};

        template <typename X>
        struct is_awaitable<awaitable<X>> : std::true_type {};
    }

    template <typename X> concept Awaitable = meta::is_awaitable<X>::value;

    // spawn a coroutine and rethrow all exceptions.
    template<typename Coroutine>
    void spawn (exec ex, Coroutine &&coro) {
        co_spawn (ex,
            std::forward<Coroutine> (coro),
            [] (std::exception_ptr eptr) {
                if (eptr) std::rethrow_exception (eptr);
            });
    }

    // synced turns an async function and makes it work synchronously.

    // to be more precise, synced can take any function that returns an
    // awaitable type and waits for it to complete.

    // synced can take either a function or a member function pointer.
    // in the second case, the second argument must be a pointer to the
    // object with the member function.

    // the pattern
    //    synced ([&] {return some_expression; });
    // works and that's usually a lot easier than getting a member function
    // pointer and a this pointer.
    template <typename fun, typename... args>
    requires std::regular_invocable<fun, args...> && requires (fun f, args... a) {
        { std::invoke (std::forward<fun> (f), std::forward<args> (a)...) } -> Awaitable<>;
    } auto synced (fun &&f, args &&...a);

    using millisecond = std::chrono::milliseconds;

    // wait for a given amount of time.
    awaitable<void> sleep (std::chrono::milliseconds duration);

    template <typename fun, typename... args>
    requires std::regular_invocable<fun, args...> && requires (fun f, args... a) {
        { std::invoke (std::forward<fun> (f), std::forward<args> (a)...) } -> Awaitable<>;
    } auto synced (fun &&f, args &&...a) {
        using namespace boost::asio;

        // Deduce the awaitable type returned by invoking f(args...)
        using awaitable_t = decltype (std::invoke (std::forward<fun> (f), std::forward<args> (a)...));

        // Extract the value type that the awaitable produces when co_awaited
        using result_t = typename awaitable_t::value_type;

        boost::asio::io_context ioc;

        // We use std::promise/std::future instead of asio::use_future because:
        // - use_future requires the result type to be default-constructible
        // - promise allows us to support non-default-constructible types
        std::promise<result_t> p;
        auto fut = p.get_future ();

        // Spawn a coroutine onto the io_context executor.
        // This coroutine will:
        //   1. invoke the async function
        //   2. co_await its result
        //   3. forward the result (or exception) into the promise
        co_spawn (
            ioc.get_executor (),
                  [&]() -> awaitable<void> {
                      try {
                          if constexpr (std::is_void_v<result_t>) {
                              // If the async function returns void, just await it
                              // and signal completion via set_value()
                              co_await std::invoke (std::forward<fun> (f), std::forward<args> (a)...);
                              p.set_value ();
                          } else {
                              // Otherwise, await the result and store it in the promise
                              p.set_value (co_await std::invoke (std::forward<fun> (f), std::forward<args> (a)...));
                          }
                      } catch (...) {
                          // Propagate exceptions across the async → sync boundary
                          p.set_exception (std::current_exception ());
                      }
                  },
                  detached // We do not use asio completion tokens; promise handles result
        );

        // Run the event loop until all work (our coroutine) is complete.
        // This blocks the current thread, making the function synchronous.
        ioc.run ();

        // Extract the result from the future.
        // This will rethrow any exception set above.
        return fut.get ();
    }
}

#endif

