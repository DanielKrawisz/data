#include <data/random.hpp>

#include <data/tools/lazy_writer.hpp>
#include <data/string.hpp>

#include <chrono>

namespace data::random {

    uint32 select_index_by_weight (cross<double> weights, source &r) {
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

        // if the amount of bytes we need is less than what we have, provide it.
        if (Input.size () - Position >= expected_size) {
            std::copy (Input.data () + Position, Input.data () + Position + expected_size, bb);
            Position += expected_size;
            return;
        }

        // povide the bytes we already have.
        std::copy (Input.data () + Position, Input.data () + Input.size (), bb);

        size_t required_size = expected_size - (Input.size () - Position);
        size_t size_so_far = 0;

        // ask the user for bytes.
        Cout << UserMessageAsk << std::endl;

        {
            lazy_bytes_writer w {Input};

            while (true) {
                string input;
                std::getline (Cin, input);
                size_so_far += input.size ();
                w.write ((byte*) input.data (), input.size ());

                if (size_so_far >= required_size) break;

                Cout << UserMessageAskMore << std::endl;
            }
        }

        Cout << UserMessageConfirm << std::endl;
        std::copy (Input.data (), Input.data () + required_size, bb);
        Position = required_size;

    }
    
}
