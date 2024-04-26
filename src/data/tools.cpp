#include <data/tools.hpp>

namespace data {
    list<string_view> split (const string_view &s, const string &delimiter) {
        data::list<string_view> pieces;

        size_t begin_pos = 0;
        size_t end_pos;
        while ((end_pos = s.find (delimiter, begin_pos)) != string::npos) {
            pieces = pieces << string_view {s.data () + begin_pos, end_pos - begin_pos};
            begin_pos = end_pos + delimiter.length ();
        }

        return pieces << string_view {s.data () + begin_pos, s.size () - begin_pos};
    }

}

