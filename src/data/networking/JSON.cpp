#include <data/networking/JSON.hpp>

#include <sstream>

namespace data {

    ptr<writer<char>> JSON_line_parser (function<void (const JSON &)> handle, function<void (parse_error)> errors) {
        struct parser : writer<char> {
            function<void (const JSON &)> Handler;
            function<void (parse_error)> ErrorHandler;

            std::stringstream Stream;

            parser (function<void (const JSON &)> handle, function<void (parse_error)> errors): Handler {handle}, ErrorHandler {errors}, Stream {} {}

            void write(const char *data, size_t size) {
                int last_new_line = 0;

                for (int i = 0; i < size; i++) if (data[i] == '\n') {
                    Stream.write(data + last_new_line, i);

                    try {
                        Handler(JSON::parse (Stream.str ()));
                    } catch (const JSON::exception &x) {
                        ErrorHandler (parse_error {x.what()});
                    }

                    Stream = std::stringstream {};
                    last_new_line = i + 1;
                }

                Stream.write((const char*)(data + last_new_line), size - last_new_line);
            }
        };

        return std::static_pointer_cast<writer<char>> (std::make_shared<parser> (handle, errors));
    }
}

