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
    io::error main (std::span<const char *const>);

    // catch_all catches everything that could be thrown. 
    // The first argument can be a callable type or a member function pointer. If it's
    // a member function pointer, the second argument must be a pointer to the object.
    template <typename fun, typename ...args> requires std::regular_invocable<fun, args...>
    io::error catch_all (fun &&f, args &&...a) noexcept {
        try {
            return std::invoke (std::forward<fun> (f), std::forward<args> (a)...);
        } catch (const method::unimplemented &m) {
            return io::error {io::error::programmer_action, m.what ()};
        } catch (const data::exception &x) {
            return io::error {io::error::code {x.Code}, std::string {x.what ()}};
        } catch (const std::exception &x) {
            return io::error {io::error::unknown, x.what ()};
        } catch (int i) {
            return io::error {io::error::code {i}};
        } catch (...) {
            return io::error {io::error::unknown};
        }
    }

}

int main (int arg_count, char **arg_values) {
    std::signal (SIGINT, data::signal_handler);
    std::signal (SIGTERM, data::signal_handler);

    data::io::error err = data::catch_all (&data::main, data::slice<const char *const> {arg_values, arg_count});

    if (bool (err)) {
        std::cout << "Program exited normally with error code " << static_cast<int> (err.Code) << "." << std::endl;
        if (err.Message) std::cout << "Error message was " << *err.Message << "." << std::endl;
        if (static_cast<int> (err.Code) <= 5 && static_cast<int> (err.Code) > 0) {
            std::cout << "General advice to resolve this error: ";

            [] (data::io::error::code code, std::ostream &o) -> std::ostream & {
                switch (code) {
                    case 0: return o << "not an error";
                    case 2: return o << "try again";
                    case 3: return o << "user should resolve his invalid action";
                    case 4: return o << "summon administrator to resolve this issue";
                    case 5: return o << "call the programmer to fix this bug";
                    default: return o << "none";
                }
            } (err.Code, std::cout) << "." << std::endl;
        }
    }

    return err.Code;

}
