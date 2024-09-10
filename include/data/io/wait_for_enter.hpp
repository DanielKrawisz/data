// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_WAIT_FOR_ENTER
#define DATA_IO_WAIT_FOR_ENTER

#include <iostream>

namespace data {

    void wait_for_enter (std::string message = "Press enter to continue...");

    bool get_user_yes_or_no (std::string message = "");

}

#endif
