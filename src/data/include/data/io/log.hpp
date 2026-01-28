// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_LOG
#define DATA_IO_LOG

#include <data/types.hpp>
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

namespace data::log {
    using namespace boost::log;

    enum severity_level {
        debug,
        normal,
        note,
        warning,
        error,
        critical
    };

    struct options {
        // where to write logs. If blank, only display to the screen.
        std::string filename = "";

        // minimum level to log.
        severity_level threshold = severity_level::normal;

        // always true if filename == ""
        bool print_to_screen = true;
    };

    void init (options = {});

    /*  ----------------------------------------------------------------
        Call init () to initialize logging.

        Use

            init ({
                .filename           = file to store logs (optinal)
                .min_security_level = minimum level to log
            });

    */
}

    /*  ----------------------- Use this to log -----------------------
        Convenience macro for logging with only a severity, using the
        default channel the logger was created with ("data").
        Example:
        DATA_LOG (error) << "File not found";
    */
#define DATA_LOG(sev) \
    BOOST_LOG_SEV((data::log::global_log::get ()),(data::log::severity_level::sev)) << \
        ::data::log::indent_string (::data::log::indent::depth ())

namespace data::log {

    std::ostream &operator << (std::ostream &strm, severity_level level);

    // Define a logger type that supports:
    //   * severity
    //   * channels (std::string)
    // The “_mt” at the end means “multi-threaded”, i.e. thread-safe.
    // This logger attaches two attributes to each record:
    //     - severity
    //     - channel
    typedef sources::severity_channel_logger_mt <
        severity_level,     // the type of the severity level
        std::string         // the type of the channel name
    > my_logger_mt;

    // Define a global logger accessible everywhere through global_log::get().
    // BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT is a Boost macro that:
    //   * declares a function global_log::get()
    //   * ensures the logger is constructed exactly once
    //   * optionally initializes it with attributes
    BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(global_log, my_logger_mt) {
        // Construct the logger with the channel name "data".
        // This means every record emitted through global_log
        // will contain channel="data".
        //
        // The 'keywords::channel' keyword binds the channel attribute.
        return my_logger_mt (keywords::channel = "data");
    }

    // a utility for adding indents to logs.
    //   use indent abc {} to add indents to logs
    //   that will step once abc is destructed.
    class indent {
    public:
        explicit indent ();

        ~indent ();

        static unsigned depth ();

    private:

        // singleton to store default fill string
        static indent &instance ();
    };

    std::string inline indent_string (unsigned int indent_depth) {
        return std::string (indent_depth, ' ');
    }

// Declare an attribute keyword named "severity".
// This lets you refer to the Severity attribute in formatters and filters.
// The string "Severity" must match the name used inside Boost.Log records.
// The type is your severity_level enum.
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)

// Convenience macro for logging with a specific channel + severity.
// Example usage:
//     DATA_LOG_CHANNEL("network", warning) << "Lost connection";
// This expands to BOOST_LOG_CHANNEL_SEV(logger, channel, severity)
#define DATA_LOG_CHANNEL(channel, sev) \
    BOOST_LOG_CHANNEL_SEV((data::log::global_log::get()),(channel),(data::log::severity_level::sev)) << \
        ::data::log::indent_string (::data::log::indent::depth ())

}

#endif //DATA_LOG_HPP
