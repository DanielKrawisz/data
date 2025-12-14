#include <iomanip>
#include <data/string.hpp>
#include <data/stream.hpp>

namespace data {
    std::istream &skip_whitespace (std::istream &i) {
        while (true) {
            int c = i.peek ();
            if (c < 0 || !std::isspace (static_cast<unsigned char> (c))) break;
            i.get ();
        }

        return i;
    }

    std::istream &operator >> (std::istream &i, string &x) {
        skip_whitespace (i);
        int first = i.peek ();
        
        if (first < 0 || static_cast<char> (first) != '"') {
            i.setstate (std::ios::failbit);
            return i;
        }
        
        return i >> std::quoted (x);
    }

    string::operator bytes () const {
        bytes b (this->size ());
        std::copy (begin (), end (), b.data ());
        return b;
    }

    template <typename X>
    void inline write_to_stream (std::ostream &o, X x) {
        o << x;
    }

    template <typename X, typename Y, typename ... P>
    void inline write_to_stream (std::ostream &o, X x, Y y, P ... p) {
        write_to_stream (o, x);
        write_to_stream (o, y, p...);
    }

    template <typename X, typename ... P>
    std::string write_error_string (X x, P... p) {
        std::stringstream ss;
        write_to_stream (ss, x, p...);
        return ss.str ();
    }

    string substring (const std::string &x, int64 begin, int64 end) {
        size_t size = x.size ();
        size_t positive_begin;
        size_t positive_end;

        if (begin < 0) {
            int back_begin = static_cast<int> (size) + begin;
            if (back_begin < 0) throw out_of_range
                {write_error_string ("reading ", begin, " from the end of a string of size ", size)};
            else positive_begin = static_cast<size_t> (back_begin);
        } else if (begin > size) throw out_of_range
            {write_error_string ("reading ", begin, " from a string of size ", size)};
        else positive_begin = static_cast<size_t> (begin);

        if (end < 0) {
            int back_end = static_cast<int> (size) + end;
            if (back_end < 0) throw out_of_range
                {write_error_string ("reading ", end, " from the end of a string of size ", size)};
            else positive_end = static_cast<size_t> (back_end);
        } else if (end > size) throw out_of_range
            {write_error_string ("reading ", end, " from a string of size ", size)};
        else positive_end = static_cast<size_t> (end);

        if (positive_begin > positive_end) throw out_of_range
            {write_error_string ("attempt to read a string backwards from ", begin, " to ", end)};

        return x.substr (positive_begin, positive_end);
    }

    stack<string_view> split (string_view s, string_view delimiter) {
        stack<string_view> pieces;

        size_t begin_pos = 0;
        size_t end_pos;
        while ((end_pos = s.find (delimiter, begin_pos)) != string::npos) {
            pieces = pieces >> string_view {s.data () + begin_pos, end_pos - begin_pos};
            begin_pos = end_pos + delimiter.length ();
        }

        return reverse (pieces >> string_view {s.data () + begin_pos, s.size () - begin_pos});
    }

}

