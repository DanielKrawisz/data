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

        lazy_bytes_writer b;
        while (true) {
            string input;
            std::getline (Cin, input);
            size_so_far += input.size ();
            b.write ((byte*) input.data (), input.size ());

            if (size_so_far >= true_size) {
                Cout << UserMessageConfirm << std::endl;
                return bytes (b);
            }

            Cout << UserMessageAskMore << std::endl;
        }
    }

}
