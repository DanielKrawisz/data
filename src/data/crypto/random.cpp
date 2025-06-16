#include <data/crypto/random.hpp>
#include <data/tools.hpp>
#include <data/string.hpp>

namespace data::crypto {

    uint32 select_index_by_weight (cross<double> weights, entropy &r) {
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

    void user_entropy::read (byte *bb, size_t expected_size) {
        if (Input.size () - Position < expected_size) {
            std::copy (Input.data () + Position, Input.data () + Position + expected_size, bb);
            return;
        }

        std::copy (Input.data () + Position, Input.data () + Input.size (), bb);

        size_t required_size = expected_size - (Input.size () - Position);
        size_t size_so_far = 0;

        Cout << UserMessageAsk << std::endl;

        lazy_bytes_writer w;

        while (true) {
            string input;
            std::getline (Cin, input);
            size_so_far += input.size ();
            w.write ((byte*) input.data (), input.size ());

            if (size_so_far >= required_size) {
                Cout << UserMessageConfirm << std::endl;
                Input = bytes (w);
                std::copy (Input.data (), Input.data () + required_size, bb);
                Position = required_size;
            }

            Cout << UserMessageAskMore << std::endl;
        }
    }

}
