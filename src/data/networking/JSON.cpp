#include <data/networking/JSON.hpp>

void data::networking::JSON_line_connection::write(const char *data, size_t size) {
    int last_new_line = 0;
    for (int i = 0; i < size; i++) if (data[i] == '\n') {
        Stream.write(data + last_new_line, i);

        try {
            parsed(JSON::parse(Stream.str()));
        } catch (const JSON::exception &x) {
            parse_error(Stream.str());
        }

        Stream = std::stringstream {};
        last_new_line = i + 1;
    }
    
    Stream.write((const char*)(data + last_new_line), size - last_new_line);
}
