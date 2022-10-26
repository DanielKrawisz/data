#include <data/networking/JSON.hpp>
 
void data::networking::json_line_session::write(const byte *data, size_t size) {
    int last_new_line = 0;
    for (int i = 0; i < size; i++) if ((const char)(data[i]) == '\n') {
        Stream.write((const char*)(data + last_new_line), i);
        try {
            parsed(json::parse(Stream.str()));
        } catch (const json::exception &x) {
            parse_error(Stream.str());
        }
        Stream = std::stringstream{};
        last_new_line = i + 1;
    }
    
    Stream.write((const char*)(data + last_new_line), size - last_new_line);
}
