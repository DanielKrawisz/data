#include <data/tools.hpp>

namespace data {
    list<string> split (string_view str, const string &delimiter) {
        string s {str};
        data::list<string> pieces;

        size_t pos;
        while ((pos = s.find (delimiter)) != string::npos) {
            auto token = substring (s, 0, pos);
            pieces = pieces << token;
            s.erase (0, pos + delimiter.length ());
        }

        return pieces << s;
    }

}

