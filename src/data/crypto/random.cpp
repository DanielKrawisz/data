#include <data/crypto/random.hpp>

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


}
