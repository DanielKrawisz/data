// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UTILTIME_H
#define BITCOIN_UTILTIME_H
#ifdef _WIN32
#include <boost/date_time/posix_time/posix_time.hpp>
#endif
#ifdef linux
#include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include <cstdint>
#include <sstream>
#include <string>

/**
 * GetTimeMicros() and GetTimeMillis() both return the system time, but in
 * different units. GetTime() returns the sytem time in seconds, but also
 * supports mocktime, where the time can be specified by the user, eg for
 * testing (eg with the setmocktime rpc, or -mocktime argument).
 *
 * TODO: Rework these functions to be type-safe (so that we don't inadvertently
 * compare numbers with different units, or compare a mocktime to system time).
 */

int64_t GetTime ();
int64_t GetTimeMillis ();
int64_t GetTimeMicros ();
// Like GetTime(), but not mockable
int64_t GetSystemTimeInSeconds ();
int64_t GetLogTimeMicros ();
void SetMockTime (int64_t nMockTimeIn);
void MilliSleep (int64_t n);

std::string DateTimeStrFormat (const char *pszFormat, int64_t nTime);

class DateTimeFormatter {
    std::locale locale_;

public:
    explicit DateTimeFormatter (const char* format);

    std::ostringstream operator () (int64_t nTime) const;
};

#endif // BITCOIN_UTILTIME_H
