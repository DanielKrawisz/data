// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#ifndef DATA_LOG_HPP
#define DATA_LOG_HPP

namespace data::log {
    namespace logging = boost::log;
    namespace sinks = boost::log::sinks;
    namespace src = boost::log::sources;
    namespace expr = boost::log::expressions;
    namespace attrs = boost::log::attributes;
    namespace keywords = boost::log::keywords;
    enum severity_level {
        normal,
        notification,
        warning,
        error,
        critical
    };
    std::ostream& operator<< (std::ostream& strm, severity_level level);

    typedef src::severity_channel_logger_mt <
    severity_level,     // the type of the severity level
    std::string         // the type of the channel name
    > my_logger_mt;


    BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(global_log, my_logger_mt) {
    // Specify the channel name on construction, similarly as with the channel_logger
    return
    my_logger_mt(keywords::channel = "data"
    );
}
    BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
#define DATA_LOG_CHANNEL(channel,sev) BOOST_LOG_CHANNEL_SEV((global_log::get()),(channel),(data::log::severity_level::sev))
#define DATA_LOG(sev) BOOST_LOG_SEV((global_log::get()),(data::log::severity_level::sev))
void init_logging(std::string filename);
void testLog();

}
#endif //DATA_LOG_HPP
