// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

namespace data {

    void inline wait_for_enter(std::string message = "Press enter to continue...") {
        do {
            std::cout << '\n' << message;
        } while (std::cin.get() != '\n');
    }

}

#endif
