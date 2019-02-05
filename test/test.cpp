#include <data/list/linked_list.hpp>
#include <data/queue/functional_queue.hpp>

std::vector<std::string> read_options(int argc, char *argv[]) {
    std::vector<std::string> options{static_cast<size_t>(argc)};
    for (int i = 0; i < argc; i ++) options[i] = std::string(argv[i]);
    return options;
}

using namespace data;

int test_main(std::vector<std::string> options) {
    linked_list<int> l{};
    l = l + 1;
    
    int n = first(l);
    
    functional_queue<linked_list<int>> f{l};
    
    int m = first(f);
    
    auto b = options.begin();
    auto e = options.end();
    
    auto il = iterator_list<decltype(b)>(b, e);
    
    return 0;
}

int main(int argc, char *argv[]){
    return test_main(read_options(argc, argv));
}
