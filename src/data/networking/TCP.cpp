#include <data/networking/TCP.hpp>

// begin waiting for the next message asynchronously. 
void data::networking::tcp_session::wait_for_message() {
    boost::asio::async_read_until(Socket, Buffer, "\n",  
        [self = shared_from_this()](const io_error& error, size_t bytes_transferred) -> void {
            if (error) return self->handle_error(error);
            
            std::stringstream ss;
            ss << std::istream(&self->Buffer).rdbuf();
            string z = ss.str();
            self->Buffer.consume(bytes_transferred);
            try {
                self->receive(bytes_view{(const byte *)(z.data()), z.size()});
                self->wait_for_message();
            } catch (...) {
                self->Socket.close();
            }
        });
}

void data::networking::tcp_session::send(bytes_view b) {
    boost::asio::async_write(Socket, io::buffer(b), io::transfer_all(), 
        [self = shared_from_this()](const io_error& error, size_t) -> void {
            if (error) self->handle_error(error);
        });
}
