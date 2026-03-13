// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/io/main.hpp>
#include <data/synchronized.hpp>

namespace data {
    
    // Indicate globally that a shutdown request has been received.
    // Use extern std::atomic<bool> ShutdownRequested to access
    // this variable from another translation unit. 
    std::atomic<bool> ShutdownRequested {false};

    // ensure that all threads will wake up and reach a halt state.
    void shutdown () noexcept;

    void signal_handler (int signal) noexcept {
        if (signal == SIGINT || signal == SIGTERM) {
            std::cout << "shutdown signal received" << std::endl;
            ShutdownRequested = true;
            shutdown ();
        }
    }

    // surround with catch_all or your own version
    void multi_main (function<void ()> f, uint16 num_threads) {
        if (num_threads < 1) throw data::exception {} << 
            "We cannot run with zero threads. There is already one thread running to read in the input you have provided.";

        if (nome_threads > 20) throw data::exception {} << 
            "Do you really need more than 20 threads? "
            "This is an experimental feature now. "
            "Please make an issue on the github repo if you need more and we will strive to expedite this feature.";
        
        // Logic for handling exceptions thrown by threads.
        std::exception_ptr stored_exception = nullptr;
        std::mutex exception_mutex;

        std::vector<std::thread> threads {};

        auto main_loop = [&]() {
            try {
                while (!Shutdown) f ();
            // catch all exceptions 
            } catch (...) {
                std::lock_guard<std::mutex> lock (exception_mutex);
                // Capture first exception
                if (!stored_exception) stored_exception = std::current_exception (); 
                Shutdown = true;
            }
                
            shutdown ();
        };
        
        for (int i = 1; i < num_threads; i++) threads.emplace_back (main_loop);

        main_loop ();

        for (int i = 1; i < num_threads; i++) threads[i - 1].join ();

        if (stored_exception) std::rethrow_exception (stored_exception);
    
    }

}