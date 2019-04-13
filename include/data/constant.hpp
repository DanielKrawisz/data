#ifndef DATA_CONSTANT_HPP
#define DATA_CONSTANT_HPP

namespace data {

    template <typename X>
    struct constant {
        X Constant;
        
        template <typename Y>
        X operator()(Y) const {
            return Constant;
        }
    };

}

#endif
