// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_SYNCHRONIZED
#define DATA_TOOLS_SYNCHRONIZED

#include <shared_mutex>

namespace data {

    template <typename X> class synchronized {
        X Value;
        std::shared_mutex Mut;
    
    public:
        template <typename... Args>
        synchronized (Args... args) : Value {args...}, Mut {} {}

        template <typename fun> requires std::regular_invocable<fun, X>
        auto operator () (fun &&f) const {
            std::shared_lock<std::shared_mutex> lock {Mut};
            return f (Value);
        }

        template <typename fun, typename ...args> requires std::regular_invocable<fun, X>
        auto operator () (fun &&f) {
            std::unique_lock<std::shared_mutex> lock {Mut};
            return f (Value);
        }
    };

}

#endif
