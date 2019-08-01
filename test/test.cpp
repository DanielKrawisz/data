#include <data/data.hpp>
#include <data/tree/linked_tree.hpp>

std::vector<std::string> read_options(int argc, char *argv[]) {
    std::vector<std::string> options{static_cast<size_t>(argc)};
    for (int i = 0; i < argc; i ++) options[i] = std::string(argv[i]);
    return options;
}

using namespace data;

int test_main(std::vector<std::string> options) {
    linked_list<int> l{{8, 7, 6}};
    l = l + 1;
    
    int n = first(l);
    
    functional_queue<linked_list<int>> f{l};
    functional_queue<linked_list<int>> g{{3, 5, 7}};
    
    int m = first(f);
    
    auto b = options.begin();
    auto e = options.end();
    
    auto il = iterator_list<decltype(b), std::string>(b, e);
    
    linked_tree<int> tr{};
    
    return 0;
}

int main(int argc, char *argv[]){
    return test_main(read_options(argc, argv));
}
