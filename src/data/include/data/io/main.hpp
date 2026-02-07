// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/io/error.hpp>
#include <data/io/exception.hpp>
#include <data/io/unimplemented.hpp>
#include <data/io/log.hpp>

#include <span>
#include <csignal>

namespace data {

    void signal_handler (int signal) noexcept;
    io::error main (std::span<const char *const>) noexcept;

    // catch_all catches everything that could be thrown. 
    // The first argument can be a callable type or a member function pointer. If it's
    // a member function pointer, the second argument must be a pointer to the object.
    template <typename fun, typename ...args> requires std::regular_invocable<fun, args...>
    io::error catch_all (fun &&f, args &&...a) {
        try {
            return std::invoke (std::forward<fun> (f), std::forward<args> (a)...);
        } catch (const method::unimplemented &m) {
            return io::error {io::error::programmer_action, m.what ()};
        } catch (const data::exception &x) {
            return io::error {io::error::code {x.Code}, std::string {x.what ()}};
        } catch (const std::exception &x) {
            return io::error {io::error::unknown, x.what ()};
        } catch (...) {
            return io::error {io::error::unknown};
        }

        return io::error {};
    }

}

int main (int arg_count, char **arg_values) {
    std::signal (SIGINT, data::signal_handler);
    std::signal (SIGTERM, data::signal_handler);

    data::io::error err = data::catch_all (&data::main, data::slice<const char *const> {arg_values, arg_count});

    if (bool (err)) DATA_LOG (error) << "Program exit with " << err;

    return err.Code;

}
