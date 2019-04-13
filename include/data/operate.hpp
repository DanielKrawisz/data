#ifndef DATA_OPERATE_HPP
#define DATA_OPERATE_HPP

namespace data {

    template <typename function, typename argument>
    struct operate {
        function Function;
        argument Argument;
                
        template <typename result, typename environment>
        result operator()(environment z) {
            return Function(z)(Argument(z));
        }
    };

}

#endif
