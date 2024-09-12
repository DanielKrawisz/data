// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <limits>
#include <data/io/wait_for_enter.hpp>

namespace data {

    void wait_for_enter (std::string message) {
        do {
            std::cout << '\n' << message;
        } while (std::cin.get () != '\n');
    }

    bool get_user_yes_or_no (std::string message) {
        char q;
        do {
            std::cout << message << " (Y/N)" << std::endl;
            q = 0;
            std::cin >> q;
        } while (q != 'n' && q != 'y' && q != 'Y' && q != 'N');
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        return q == 'Y' || q == 'y';
    }

}
