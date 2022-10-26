#include <data/tools/channel.hpp>

namespace data::tool {

    template <class item> 
    void channel<item>::inner::close() {
        std::unique_lock<std::mutex> lock{M};
        Closed = true;
        Receive.notify_all();
    }

    template <class item> 
    bool channel<item>::inner::closed() const {
        std::unique_lock<std::mutex> lock{M};
        return closed;
    }

    template <class item> 
    void channel<item>::inner::put(const item &i) {
        std::unique_lock<std::mutex> lock{M};
        if(Closed) throw std::logic_error("put to closed channel");
        Queue.push_back(i);
        Receive.notify_one();
        if (Size == 0 || Queue.size() < Size) return;
        else Send.wait(lock);
    }
        
    template <class item> 
    bool channel<item>::inner::get(item &out, bool wait) {
        std::unique_lock<std::mutex> lock{M};
        
        if (Queue.empty()) {
            if (!wait || Closed) return false;
            else {
                Receive.wait(lock);
                if (Closed) return false;
            }
        }
        
        out = Queue.front();
        Queue.pop_front();
        Send.notify_one();
        return true;
    }

}
