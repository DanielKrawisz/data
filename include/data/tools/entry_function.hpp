#ifndef DATA_TOOLS_ENTRY_FUNCTION_HPP
#define DATA_TOOLS_ENTRY_FUNCTION_HPP

#include <data/function.hpp>
#include <data/map.hpp>

namespace data {
        
    // value_function converts any function from X to Y to 
    // a function from map::entry<K, X> to map::entry<K, Y>
    template <typename key, typename function, typename input_value>
    struct value_function {
        function Function;
        
        using output_value = typename std::__invoke_result<function, input_value>::type;
        
        constexpr static function_definition<function, input_value, output_value> required{};
        
        value_function(function fun) : Function{fun} {}
        
        map::entry<key, output_value> operator()(map::entry<key, input_value> e){
            return {e.Key, Function(e.Value)};
        }
    };
        
    // entry_function converts any function from map::entry<K, X> to Y to 
    // a function from map::entry<K, X> to map::entry<K, Y>
    template <typename key, typename function, typename input_value>
    struct entry_function {
        function Function;
        
        using output_value = typename std::__invoke_result<function, map::entry<key, input_value>>::type;
        
        constexpr static function_definition<function, input_value, output_value> required{};
        
        entry_function(function fun) : Function{fun} {}
        
        map::entry<key, output_value> operator()(map::entry<key, input_value> e){
            return {e.Key, Function(e)};
        }
    };

}

#endif
