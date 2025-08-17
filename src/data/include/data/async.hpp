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

    template <typename X> concept is_awaitable = meta::is_awaitable<X>::value;

    // spawn a coroutine and rethrow all exceptions.
    template<typename Coroutine>
    void spawn (exec ex, Coroutine &&coro) {
        co_spawn (ex,
            std::forward<Coroutine> (coro),
            [] (std::exception_ptr eptr) {
                if (eptr) std::rethrow_exception (eptr);
            });
    }

    // sync takes an async function and arguments and runs as an ordinary sync function.
    // the first argument can be a callable type or a member function pointer. If it's
    // a member function pointer, the second argument must be a pointer to the object.
    template <typename fun, typename... args>
    requires std::regular_invocable<fun, args...> && requires (fun f, args... a) {
        { std::invoke (std::forward<fun> (f), std::forward<args> (a)...) } -> is_awaitable<>;
    } auto synced (fun &&f, args &&...a) {
        using namespace boost::asio;
        boost::asio::io_context ioc;
        return co_spawn (ioc.get_executor (),
            [&]() -> decltype (std::invoke (std::forward<fun> (f), std::forward<args> (a)...)) {
                co_return co_await std::invoke (std::forward<fun> (f), std::forward<args> (a)...);
            }, use_future  // This turns the coroutine into a std::future
        ).get ();  // Block and get the result

    }

    using milliseconds = std::chrono::milliseconds;

    // wait for a given amount of time.
    awaitable<void> sleep (std::chrono::milliseconds duration);

}

#endif

