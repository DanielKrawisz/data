#ifndef DATA_IO_MAIN_INPUT
#define DATA_IO_MAIN_INPUT

#include <data/types.hpp>

namespace data {
    
    // function to return a list of strings
    // from program input.
    template <typename list>
    inline list read_main_input(int argc, char *argv[]) {
        list l{};
        for (int i = 0; i < argc; i++) l + string{argv[i]};
        return l;
    };
    
}

#endif
