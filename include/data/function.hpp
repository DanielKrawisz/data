#ifndef DATA_FUNCTION_HPP
#define DATA_FUNCTION_HPP

namespace data {
    
    // function from x to y. 
    template<typename f, typename x, typename y> struct function_definition {
        // type f must be callable, and must take an argument of type x
        // and return an argument of type y. 
        static const y callable(const f fun, const x arg) {
            return fun(arg);
        }
    };

}

#endif
