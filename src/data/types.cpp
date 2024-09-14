#include <data/types.hpp>

#include <chrono>

namespace data {
    random_engine& get_random_engine() {
        static random_engine Engine;
        static bool Seeded = false;
        if (!Seeded) Engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
        return Engine;
    }
    
}
