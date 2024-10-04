// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_WAIT_FOR_ENTER
#define DATA_IO_WAIT_FOR_ENTER

#include <iostream>
#include <optional>

namespace data {
    /// @brief Waits for user to press enter, printing message again if anything other is pressed
    /// @param message message to print
    /// @warning Will cause an infinite loop if used in a non-terminal enviroment
    void wait_for_enter (std::string message = "Press enter to continue...");


    /// @brief Waits for user to respond to a message with yes or no
    /// @param message question to ask user
    /// @param default_value default value to return if enter pressed or no terminal
    /// @return true if yes, false otherwise, or no value if nothing was chosen and no default
    std::optional<bool> get_user_yes_or_no (std::string message ,std::string default_value="Y");

}

#endif
