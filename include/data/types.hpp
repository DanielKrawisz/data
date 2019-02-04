#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <vector>
#include <memory>
#include <string>

namespace data {
    
    template<typename X>
    using vector = const std::vector<X>;

    using byte = uint8_t;

    template<typename X>
    using ptr = std::shared_ptr<X>;

    using string = const std::string;
    
    using uint = unsigned long long int;

}

#endif
