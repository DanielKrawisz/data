// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_WAIT_FOR_ENTER
#define DATA_IO_WAIT_FOR_ENTER

#include <iostream>
#include <optional>
#include "../maybe.hpp"

namespace data {
    /// @brief Waits for user to press enter, printing message again if anything other is pressed
    /// @param message message to print
    /// @warning Will cause an infinite loop if used in a non-terminal enviroment
    void wait_for_enter (std::string message = "Press enter to continue...");


    /// @brief Waits for user to respond to a message with yes or no
    /// @param message question to ask user
    /// @param default_value default value to return if enter pressed or no terminal
    /// @return true if yes, false otherwise.
    bool get_user_yes_or_no (std::string message ,maybe<bool> default_value=false);

    /// @brief Gets a password from the user
    /// @param message message to ask user for password
    /// @param mask masking character to use
    /// @return password returned
    std::string get_user_password (std::string message, char mask='*');

    /// @brief reads a decimal number from the user
    /// @param question question to ask the user.
    /// @return number returned as double
    double read_decimal (const std::string &question);

}

#endif
