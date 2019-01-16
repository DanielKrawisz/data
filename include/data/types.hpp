#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <vector>
#include <memory>
#include <string>

namespace data {
    
    template<typename X>
    using sequence = const std::vector<X>;

    using byte = uint8_t;

    template<typename X>
    using ptr = const std::shared_ptr<X>;

    using string = const std::string;
    
    using uint = unsigned long long int;
    
    template <typename X>
    inline X plus(X a, X b) {
        return a + b;
    }

}

#endif
