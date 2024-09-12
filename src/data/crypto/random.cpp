#include <data/crypto/random.hpp>
#include <data/tools.hpp>
#include <data/string.hpp>

namespace data::crypto {

    uint32 select_index_by_weight (cross<double> weights, random &r) {
        double total = 0;
        for (double w : weights) total += w;

        double val = std::uniform_real_distribution<double> (0, total) (r);

        double accumulated = 0;
        for (uint32 i = 0; i < weights.size (); i++) {
            accumulated += weights[i];
            if (val <= accumulated) return i;
            i++;
        }

        // we shouldn't get here but just in case.
        return weights.size () - 1;
    }

    bytes user_entropy::get (size_t x) {
        Cout << UserMessageAsk << std::endl;
        // we get some extra to account for user lack of entropy.
        size_t true_size = x * 4;
        size_t size_so_far = 0;
        list<string> user_lines;
        while (true) {
            string input;
            std::getline (Cin, input);
            size_so_far += input.size ();
            user_lines <<= input;

            if (size_so_far >= true_size) {
                Cout << UserMessageConfirm << std::endl;
                bytes b {};
                b.resize (size_so_far);
                data::iterator_writer<bytes::iterator, byte> bb {b.begin (), b.end ()};
                for (const auto &str : user_lines) bb.write ((byte *) (str.data ()), str.size ());
                return b;
            }

            Cout << UserMessageAskMore << std::endl;
        }
    }

}
