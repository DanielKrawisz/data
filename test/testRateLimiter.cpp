// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <data/tools/rate_limiter.h>
#include "gtest/gtest.h"
#include <iostream>

namespace data {
    namespace tools {
        TEST(RateLimiterTest,testRateLimitLong) {
            RateLimiter limiter(3,10);
            ASSERT_EQ(limiter.getTime(),0);
            ASSERT_EQ(limiter.getTime(),0);
            ASSERT_EQ(limiter.getTime(),0);
            long time1=limiter.getTime();
            ASSERT_GT(time1,9);
            sleep(2);
            long time2=limiter.getTime();

            ASSERT_LT(time2,time1);
            sleep(time2);
            ASSERT_EQ(limiter.getTime(),0);
            sleep(30);
            ASSERT_EQ(limiter.getTime(),0);
            ASSERT_EQ(limiter.getTime(),0);
            ASSERT_EQ(limiter.getTime(),0);
        }
        TEST(RateLimiterTest,testRateLimitShort) {
            RateLimiter limiter(3, 10);
            ASSERT_EQ(limiter.getTime(), 0);
            ASSERT_EQ(limiter.getTime(), 0);
            ASSERT_EQ(limiter.getTime(), 0);
            long time1 = limiter.getTime();
            ASSERT_GT(time1, 9);
            std::cout << time1 << std::endl;
            sleep(1);
            long time2 = limiter.getTime();

            ASSERT_LT(time2, time1);
        }
    }
}