#ifndef DATA_IO_MAIN
#define DATA_IO_MAIN

#include <data/types.hpp>
#include <data/list/linked_list.hpp>

namespace data {
    
    struct output {
        bool Error;
        string Response;
        
        output() : Error{false}, Response{} {}
        output(bool e, string r) : Error{e}, Response{r} {}
    };
    
    // function to return a list of strings
    // from program input.
    inline linked_list<string> read_main_input(int argc, char *argv[]) {
        linked_list<string> l{};
        for (int i = 0; i < argc; i++) l + string{argv[i]};
        return l;
    };
    
    template <typename f>
    int main(f fun, int argc, char *argv[]) {
        const output o = fun(read_main_input(argc, argv));
        (o.Error ? std::cerr : std::cout) << o.Response << std::endl;
        return o.Error;
    }
    
}

#endif
