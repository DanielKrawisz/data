// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <limits>
#include <data/io/wait_for_enter.hpp>
#include <unistd.h>
#include <stdio.h>

namespace data {

    void wait_for_enter (std::string message) {
        do {
            std::cout << '\n' << message;
        } while (std::cin.get () != '\n');
    }

    std::optional<bool> get_user_yes_or_no (std::string message ,std::string default_value="Y") {
            std::string input;
            std::cout << message << " (Y/N)" << std::endl;
            std::getline(std::cin,input);
            if(input.length()>0){
                char answer=std::tolower(input[0]);
                if(answer=='y' || answer=='n')
                    return answer=='y';
                return {};
            }
            if(default_value!="")
                return std::tolower(default_value[0])=='y';
            return {};
    }

}
