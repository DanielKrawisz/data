// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <limits>
#include <data/io/wait_for_enter.hpp>

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

namespace data {

    void wait_for_enter (std::string message) {
        do {
            std::cout << '\n' << message;
        } while (std::cin.get () != '\n');
    }

    bool get_user_yes_or_no (std::string message, maybe<bool> default_value) {
                std::string input;
                while (true) {
                    std::cout << message << " (";
                    std::cout << ((default_value.has_value () && default_value.value ()) ? "Y":"y");
                    std::cout << "/";
                    std::cout << ((default_value.has_value () && !default_value.value ()) ? "N":"n");
                    std::cout << ")" << std::endl;
                    std::getline (std::cin,input);

                    if (input.length () > 0){
                        char answer = std::tolower (input[0]);
                        if (answer =='y' || answer =='n')
                            return answer=='y';
                        continue;
                    }

                    if (default_value.has_value ())
                        return default_value.value ();
                }
        }

    std::string get_user_password (std::string message, char mask) {
        struct termios term, original;
        std::string password;

        // Get current terminal settings
        if (tcgetattr (STDIN_FILENO, &term) != 0)
            return "";

        original = term;
        // Unset ECHO flag
        term.c_lflag &= ~ECHO & ~ICANON;
        if (tcsetattr (STDIN_FILENO, TCSANOW, &term) != 0)
            return "";
        // Prompt for password
        std::cout << message;

        char c;

        while (true) {
            c = getchar ();
            if (c!='\n') break;
            std::cout << mask;
            password.push_back (c);
        }

        std::cout << std::endl;
        // Set ECHO flag
        tcsetattr (STDIN_FILENO, TCSANOW, &original);
        return password;
    }

    double read_decimal (const std::string &question) {
        double value;
        while (true) {
            std::cout << question << " ";
            std::cin >> value;

            // Check if input was valid
            if (std::cin.fail ()) {
                std::cin.clear ();  // Clear error flag
                std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n'); // Discard invalid input
                std::cout << "Invalid input. Please enter a decimal number.\n";
            } else {
                std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n'); // Clear buffer
                return value;
            }
        }
    }
}
