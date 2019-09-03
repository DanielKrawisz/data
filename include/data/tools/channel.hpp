// adapted from https://st.xorian.net/blog/2012/08/go-style-channel-in-c/ 

#ifndef DATA_TOOLS_CHANNEL
#define DATA_TOOLS_CHANNEL

#include <list>
#include <mutex>
#include <condition_variable>
#include <data/types.hpp>

namespace data {
    // a golang-like communication channel between different threads.
    template<class item>
    class channel {
    private:
        std::list<item> queue;
        std::mutex m;
        std::condition_variable cv;
        uint32 max_size;
        bool closed;
    public:
        channel() : closed(false) { }
        void close() {
            std::unique_lock<std::mutex> lock(m);
            closed = true;
            cv.notify_all();
        }
        bool is_closed() {
            std::unique_lock<std::mutex> lock(m);
            return closed;
        }
        void put(const item &i) {
            std::unique_lock<std::mutex> lock(m);
            if(closed)
            throw std::logic_error("put to closed channel");
            queue.push_back(i);
            cv.notify_one();
        }
        bool get(item &out, bool wait = true) {
            std::unique_lock<std::mutex> lock(m);
            if(wait) cv.wait(lock, [&](){ return closed || !queue.empty(); });
            if(queue.empty())
            return false;
            out = queue.front();
            queue.pop_front();
            return true;
        }
    };

}

#endif
