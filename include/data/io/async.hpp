// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/io/multithreaded.hpp>
#include <data/async.hpp>

namespace data {
    
    boost::asio::io_context IO;

    void async_main (function<awaitable<void> ()> f, uint16 num_threads = 1) {
        for (int i = 0; i < num_threads; i++) data::spawn (IO.get_executor (), f);

        multi_main ([&IO] () {
            IO.run ();
        }, num_threads);
        
        IO.stop ();
    }

}