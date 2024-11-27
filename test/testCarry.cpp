// Copyright (c) 2020-2024 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/arithmetic/carry.hpp>
#include "gtest/gtest.h"

namespace data {
    
    #pragma region Add With Carry

    struct AddWithCarryTestParams {
        std::string type;
        unsigned long long num1;
        unsigned long long num2;
        bool expectedCarry;
        unsigned long long expectedResult;
    };

    class AddWithCarryTypedTest : public ::testing::TestWithParam<AddWithCarryTestParams> {
protected:
    template <typename T>
    void RunTest(const AddWithCarryTestParams& params) {
        T temp;
        T num1 = static_cast<T>(params.num1);
        T num2 = static_cast<T>(params.num2);

        EXPECT_EQ(arithmetic::add_with_carry(temp, num1, num2), params.expectedCarry)
            << "Failure for type " << params.type << " arg1 " <<  params.num1 << " arg 2 " << params.num2;
        EXPECT_EQ(temp, static_cast<T>(params.expectedResult))
            << "Failure for type " << params.type;
    }
};



INSTANTIATE_TEST_SUITE_P(
    AddWithCarryTests,
    AddWithCarryTypedTest,
        ::testing::Values(
            AddWithCarryTestParams{"unsigned char",10, 20, false, 30},
            AddWithCarryTestParams{"unsigned char",std::numeric_limits<unsigned char>::max(), 1, true, 0},
            AddWithCarryTestParams{"unsigned short",10, 20, false, 30},
            AddWithCarryTestParams{"unsigned short",std::numeric_limits<unsigned short>::max(), 1, true, 0},
            AddWithCarryTestParams{"unsigned short int",10, 20, false, 30},
            AddWithCarryTestParams{"unsigned short int",std::numeric_limits<unsigned short int>::max(), 1, true, 0},
            AddWithCarryTestParams{"unsigned int",10, 20, false, 30},
            AddWithCarryTestParams{"unsigned int",std::numeric_limits<unsigned int>::max(), 1, true, 0},
            AddWithCarryTestParams{"unsigned long",10, 20, false, 30},
            AddWithCarryTestParams{"unsigned long",std::numeric_limits<unsigned long>::max(), 1, true, 0},
            AddWithCarryTestParams{"unsigned long long",10, 20, false, 30},
            AddWithCarryTestParams{"unsigned long long",std::numeric_limits<unsigned long long>::max(), 1, true, 0}
        )
);

TEST_P(AddWithCarryTypedTest, HandleAddition) {
    AddWithCarryTestParams params = GetParam();
    std::string typeName = params.type;
    if (typeName == "unsigned char") RunTest<unsigned char>(params);
    else if (typeName == "unsigned short") RunTest<unsigned short>(params);
    else if (typeName == "unsigned short int") RunTest<unsigned short int>(params);
    else if (typeName == "unsigned int") RunTest<unsigned int>(params);
    else if (typeName == "unsigned long") RunTest<unsigned long>(params);
    else if (typeName == "unsigned long long") RunTest<unsigned long long>(params);
}


TEST (CarryTest, TestAddWithCarry) {
    unsigned int temp;
    unsigned int num1=10;
    unsigned int num2=20;
    EXPECT_FALSE(arithmetic::add_with_carry(temp,num1,num2)) << "10 + 20 should not carry in an unsigned int";       
        
}
#pragma endregion

#pragma region Subtract with Carry

struct SubtractWithCarryTestParams {
        std::string type;
        unsigned long long num1;
        unsigned long long num2;
        bool expectedCarry;
        unsigned long long expectedResult;
    };
class SubtractWithCarryTypedTest : public ::testing::TestWithParam<SubtractWithCarryTestParams> {
protected:
    template <typename T>
    void RunTest(const SubtractWithCarryTestParams& params) {
        T temp;
        T num1 = static_cast<T>(params.num1);
        T num2 = static_cast<T>(params.num2);

        EXPECT_EQ(arithmetic::subtract_with_carry(temp, num1, num2), params.expectedCarry)
            << "Failure for type " << params.type << " arg1 " <<  params.num1 << " arg 2 " << params.num2;
        EXPECT_EQ(temp, static_cast<T>(params.expectedResult))
            << "Failure for type " << params.type;
    }
};



INSTANTIATE_TEST_SUITE_P(
    SubtractWithCarryTests,
    SubtractWithCarryTypedTest,
        ::testing::Values(
            SubtractWithCarryTestParams{"unsigned char",20, 10, false, 10},
            SubtractWithCarryTestParams{"unsigned char",0,1,true,std::numeric_limits<unsigned char>::max()},
            SubtractWithCarryTestParams{"unsigned short",20, 10, false, 10},
            SubtractWithCarryTestParams{"unsigned short",0,1,true,std::numeric_limits<unsigned short>::max()},
            SubtractWithCarryTestParams{"unsigned short int",20, 10, false, 10},
            SubtractWithCarryTestParams{"unsigned short int",0,1,true,std::numeric_limits<unsigned short int>::max()},
            SubtractWithCarryTestParams{"unsigned int",20, 10, false, 10},
            SubtractWithCarryTestParams{"unsigned int",0,1,true,std::numeric_limits<unsigned int>::max()},
            SubtractWithCarryTestParams{"unsigned long",20, 10, false, 10},
            SubtractWithCarryTestParams{"unsigned long",0,1,true,std::numeric_limits<unsigned long>::max()},
            SubtractWithCarryTestParams{"unsigned long long",20, 10, false, 10},
            SubtractWithCarryTestParams{"unsigned long long",0,1,true,std::numeric_limits<unsigned long long>::max()}
        )
);

TEST_P(SubtractWithCarryTypedTest, HandleSubtraction) {
    SubtractWithCarryTestParams params = GetParam();
    std::string typeName = params.type;
    if (typeName == "unsigned char") RunTest<unsigned char>(params);
    else if (typeName == "unsigned short") RunTest<unsigned short>(params);
    else if (typeName == "unsigned short int") RunTest<unsigned short int>(params);
    else if (typeName == "unsigned int") RunTest<unsigned int>(params);
    else if (typeName == "unsigned long") RunTest<unsigned long>(params);
    else if (typeName == "unsigned long long") RunTest<unsigned long long>(params);
}


TEST (CarryTest, TestSubtractWithCarry) {
    unsigned int temp;
    unsigned int num1=20;
    unsigned int num2=10;
    EXPECT_FALSE(arithmetic::subtract_with_carry(temp,num1,num2)) << "20 - 10 should not carry in an unsigned int";       
        
}
#pragma endregion
}