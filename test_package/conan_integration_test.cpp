#include "data/log/log.hpp"

int main() {
    data::log::init_logging("log_%N.log");
    data::log::testLog();
}
