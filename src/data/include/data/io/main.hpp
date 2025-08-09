// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iostream>
#include <data/io/error.hpp>
#include <data/io/arg_parser.hpp>
#include <data/io/exception.hpp>
#include <data/io/unimplemented.hpp>

namespace data {

    void signal_handler (int signal) noexcept;
    error main (const arg_parser &) noexcept;

    // catch_all catches everything that could be thrown. 
    // The first argument can be a callable type or a member function pointer. If it's
    // a member function pointer, the second argument must be a pointer to the object.
    template <typename fun, typename ...args> requires std::regular_invocable<fun, args...>
    error catch_all (fun &&f, args &&...a) {
        try {
            std::invoke (std::forward<fun> (f), std::forward<args> (a)...);
        /*
        } catch (const net::HTTP::exception &x) {
            return error {7, x.what ()};
        } catch (const JSON::exception &x) {
            return error {6, x.what ()};
        } catch (const CryptoPP::Exception &x) {
            return error {5, x.what ()};*/
        } catch (const data::method::unimplemented &x) {
            return error {6, x.what ()};
        } catch (const data::exception &x) {
            return error {5, x.what ()};
        } catch (const std::runtime_error &x) {
            return error {4, x.what ()};
        } catch (const std::logic_error &x) {
            return error {3, x.what ()};
        } catch (const std::exception &x) {
            return error {2, x.what ()};
        } catch (...) {
            return error {1};
        }

        return error {};
    }

}

int main (int arg_count, char **arg_values) {
    std::signal (SIGINT, data::signal_handler);
    std::signal (SIGTERM, data::signal_handler);

    error err = data::main (run, arg_parser {arg_count, arg_values});

    if (bool (err)) {
        if (err.Message) std::cout << "Fail code " << err.Code << ": " << *err.Message << std::endl;
        else std::cout << "Fail code " << err.Code << "." << std::endl;
    }

    return err.Code;

}