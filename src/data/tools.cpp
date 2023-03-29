#include <data/tools.hpp>

namespace data {
    list<string> split (string_view str, const string &delimiter) {
        string s {str};
        data::list<std::string> pieces;

        size_t pos;
        while ((pos = s.find (delimiter)) != string::npos) {
            auto token = s.substr (0, pos);
            pieces = pieces << token;
            s.erase (0, pos + delimiter.length ());
        }

        return pieces << s;
    }

}

