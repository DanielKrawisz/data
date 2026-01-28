// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/attributes/function.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/attributes/attribute_set.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_value_impl.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/expressions.hpp>

#include <data/io/log.hpp>
#include <data/io/unimplemented.hpp>

namespace data::log {

    // Thread-local depth counter
    thread_local unsigned IndentDepth = 0;

    void init (options o) {

        auto core = boost::log::core::get ();
        core->set_filter (severity >= o.threshold);

        // Register a simple formatter for the severity_level enum
        // This allows Boost.Log to convert severity_level values to strings automatically
        register_simple_formatter_factory<severity_level, char> ("Severity");

        if (o.filename.empty ()) {
            // Use std::cout instead of a file
            auto sink = boost::make_shared<sinks::text_ostream_backend> ();
            sink->add_stream (boost::shared_ptr<std::ostream> (&std::cout, boost::null_deleter ()));
            auto sync_sink = boost::make_shared<sinks::synchronous_sink<sinks::text_ostream_backend>> (sink);
            sync_sink->set_formatter(
                [](boost::log::record_view const& rec, boost::log::formatting_ostream& strm) {
                    // Channel
                    if (auto attr = rec.attribute_values ()["Channel"].extract<std::string>())
                        strm << "[" << attr.get() << "] ";
                    else
                        strm << "[?] ";

                    // Severity
                    if (auto sev = rec.attribute_values ()["Severity"].extract<severity_level>())
                        strm << "[" << sev.get() << "]: ";
                    else
                        strm << "[?]: ";

                    // Message
                    strm << rec[boost::log::expressions::smessage];
                }
            );

            core::get ()->add_sink (sync_sink);
        } else {

            if (o.print_to_screen) {
                throw data::method::unimplemented {"printing logs to screen and writing to file"};
            }

            // Add a file sink for logging
            add_file_log (
                keywords::file_name = o.filename,
                keywords::rotation_size = 10 * 1024 * 1024,
                keywords::time_based_rotation = sinks::file::rotation_at_time_point (0, 0, 0),
                keywords::format = "[%Channel%] [%Severity%] %Message%");
        }

        add_common_attributes ();
    }

    std::ostream &operator << (std::ostream &strm, severity_level level) {
        static const char* strings[] = {
            "normal",
            "notification",
            "warning",
            "error",
            "critical"};

        if (static_cast<std::size_t> (level) < sizeof (strings) / sizeof (*strings))
            strm << strings[level];
        else
            strm << static_cast<int> (level);

        return strm;
    }

    indent::indent () {
        ++IndentDepth;  // increment depth
    }

    indent::~indent () {
        if (IndentDepth > 0) --IndentDepth;  // decrement depth
    }

    unsigned indent::depth () {
        return IndentDepth;
    }

    // singleton to store default fill string
    indent &indent::instance () {
        static indent inst;  // default fill string
        return inst;
    }


}
