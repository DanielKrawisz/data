// adapted from https://st.xorian.net/blog/2012/08/go-style-channel-in-c/ 

#ifndef DATA_TOOLS_CHANNEL
#define DATA_TOOLS_CHANNEL

#include <list>
#include <mutex>
#include <condition_variable>
#include <data/types.hpp>

namespace data::tool {
    // a golang-like communication channel between different threads.
    template <class item> class channel {
        struct inner {
            std::list<item> Queue;
            std::mutex M;
            std::condition_variable Receive;
            std::condition_variable Send;
            uint32 Size; 
            bool Closed;
            
            inner() : Size{0}, Closed{false} { }
            inner(uint32 n) : Size{n}, Closed{false} {}
            
            
            void close();
            bool closed() const;
            void put(const item &i);
            bool get(item &out, bool wait = true);
        };
        
    public:
        class to {
            ptr<inner> Inner;
            to() : Inner{} {}
            to(ptr<inner> i) : Inner{i} {}
            
        public:
            void close() {
                Inner->close();
            }
            
            bool closed() const {
                return Inner->closed();
            }
            
            void put(const item &i) {
                Inner->put(i);
            }
            
            to &operator<<(const item &i) {
                put(i);
                return *this;
            }
            
            friend class channel;
        };
        
        class from {
            ptr<inner> Inner; 
            from() : Inner{} {}
            from(ptr<inner> i) : Inner{i} {}
        public:
            bool get(item &out, bool wait = true) {
                return Inner->get(out, wait);
            }
            
            from &operator>>(item &out) {
                get(out);
                return *this;
            }
            
            friend class channel;
        };
        
        to To;
        from From;
        
    private:
        channel(ptr<inner> i) : To{i}, From{i} {}
        
    public:
        channel(uint32 size) : channel{std::make_shared<channel<item>::inner>(size)} {}
        channel() : channel{0} {}
        
        void close() {
            To.close();
        }
        
        bool closed() const {
            return To.closed();
        }
        
        void put(const item &i) {
            To.put(i);
        }
        
        bool get(item &out, bool wait = true) {
            return From.get(out, wait);
        }
        
        channel &operator<<(const item &i) {
            To << i;
            return *this;
        }
        
        channel &operator>>(item &out) {
            From.get(out);
            return *this;
        }
    };

}

#endif
