// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/io/log.hpp"
namespace data::log {

    void init_logging (std::string filename) {
        boost::log::register_simple_formatter_factory< severity_level, char > ("Severity");
        logging::add_file_log
                (
                        keywords::file_name = filename,
                        keywords::rotation_size = 10 * 1024 * 1024,
                        keywords::time_based_rotation = sinks::file::rotation_at_time_point (0, 0, 0),
                        keywords::format = "[%TimeStamp%] [%Channel%] [%Severity%]: %Message%"
                );
        logging::add_common_attributes ();

    }

    std::ostream &operator << (std::ostream &strm, severity_level level) {
        static const char* strings[] =
                {
                        "normal",
                        "notification",
                        "warning",
                        "error",
                        "critical"
                };

        if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
            strm << strings[level];
        else
            strm << static_cast< int >(level);

        return strm;
    }



    void testLog () {
        DATA_LOG_CHANNEL ("temp", normal) << "Hello, world!";
    }




}
