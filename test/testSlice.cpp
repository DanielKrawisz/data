//
// Created by nekosune on 13/07/19.
//

#include "data/slice.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {
class SliceTest : public ::testing::Test {
protected:
    void SetUp() override {
        test= std::vector<data::byte>();
        for(int i=0;i<20;i++)
            test.push_back(i+1);
        sliceTest=data::slice<data::byte>::make(test);
    }

    void TearDown() override {
        std::cout << "Tearing down" << std::endl;
    }

    std::vector<data::byte> test;
    data::slice<data::byte> sliceTest;
};
    TEST_F(SliceTest, SliceWriteUint16Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (uint16_t)4096;
        EXPECT_THAT(test,::testing::ElementsAre(0x10,00,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (uint16_t)9023;
        EXPECT_THAT(test,::testing::ElementsAre(0x10,00,0x23, 0x3F, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }

    TEST_F(SliceTest, SliceWriteUint16Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (uint16_t)4096;
        EXPECT_THAT(test,::testing::ElementsAre(00,0x10,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (uint16_t)9023;
        EXPECT_THAT(test,::testing::ElementsAre(00,0x10,0x3F, 0x23, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }

    TEST_F(SliceTest, SliceWriteInt16Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (int16_t)4096;
        EXPECT_THAT(test,::testing::ElementsAre(0x10,00,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (int16_t)-9023;
        EXPECT_THAT(test,::testing::ElementsAre(0x10,00,0xDC, 0xC1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }

    TEST_F(SliceTest, SliceWriteInt16Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (int16_t)4096;
        EXPECT_THAT(test,::testing::ElementsAre(00,0x10,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (int16_t)-9023;
        EXPECT_THAT(test,::testing::ElementsAre(00,0x10,0xC1, 0xDC, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }

    TEST_F(SliceTest, SliceWriteUint32Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (uint32_t)2859322050;
        EXPECT_THAT(test,::testing::ElementsAre(0xAA,0x6D,0xCA, 0xC2, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (uint32_t)2805395218;
        EXPECT_THAT(test,::testing::ElementsAre(0xAA,0x6D,0xCA, 0xC2, 0xA7, 0x36, 0xEF, 0x12, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }

    TEST_F(SliceTest, WriteUint32Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (uint32_t)2859322050;
        EXPECT_THAT(test,::testing::ElementsAre(0xC2,0xCA,0x6D,0xAA , 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (uint32_t)2805395218;
        EXPECT_THAT(test,::testing::ElementsAre(0xC2,0xCA,0x6D,0xAA , 0x12, 0xEF, 0x36, 0xA7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }

    TEST_F(SliceTest, SliceWriteInt32Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        std::cout << "Got here 1" << std::endl;
        writer << (int32_t)2859322050;
        std::cout << "Got here 2" << std::endl;
        EXPECT_THAT(test,::testing::ElementsAre(0xAA,0x6D,0xCA, 0xC2, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        std::cout << "Got here 3" << std::endl;
        writer << (int32_t)-280539521;
        std::cout << "Got here 4" << std::endl;
        EXPECT_THAT(test,::testing::ElementsAre(0xAA,0x6D,0xCA, 0xC2, 0xEF, 0x47, 0x4E, 0x7F, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        std::cout << "Got here 5" << std::endl;
    }

    TEST_F(SliceTest, WriteInt32Little) {
        std::cout << "Got here 6" << std::endl;
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (int32_t)2859322050;
        EXPECT_THAT(test,::testing::ElementsAre(0xC2,0xCA,0x6D,0xAA , 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (int32_t)-280539521;
        EXPECT_THAT(test,::testing::ElementsAre(0xC2,0xCA,0x6D,0xAA , 0x7F, 0x4E, 0x47, 0xEF, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }


    TEST_F(SliceTest, SliceWriteUint64Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (uint64_t)2387559956438732708;
        EXPECT_THAT(test,::testing::ElementsAre(0x21,0x22,0x51, 0x21, 0x6B, 0x7A, 0x4F, 0xA4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (uint64_t)17900722797183299418;
        EXPECT_THAT(test,::testing::ElementsAre(0x21,0x22,0x51, 0x21, 0x6B, 0x7A, 0x4F, 0xA4, 0xF8, 0x6C, 0x24, 0x8A, 0xBC, 0xD8, 0xEF, 0x5A, 17, 18, 19, 20));

        EXPECT_THROW(writer << (uint64_t)17906342797856292348,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceWriteUint64Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (uint64_t)2387559956438732708;
        EXPECT_THAT(test,::testing::ElementsAre(0xA4,0x4F,0x7A, 0x6B, 0x21, 0x51, 0x22, 0x21, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (uint64_t)17900722797183299418;
        EXPECT_THAT(test,::testing::ElementsAre(0xA4,0x4F,0x7A, 0x6B, 0x21, 0x51, 0x22, 0x21, 0x5A, 0xEF, 0xD8, 0xBC, 0x8A, 0x24, 0x6C, 0xF8, 17, 18, 19, 20));

        EXPECT_THROW(writer << (uint64_t)17906342797856292348,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceWriteInt64Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (int64_t)2387559956438732708;
        EXPECT_THAT(test,::testing::ElementsAre(0x21,0x22,0x51, 0x21, 0x6B, 0x7A, 0x4F, 0xA4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (int64_t)-1790072279718329941;
        EXPECT_THAT(test,::testing::ElementsAre(0x21,0x22,0x51, 0x21, 0x6B, 0x7A, 0x4F, 0xA4, 0xE7, 0x28, 0x62, 0xBE, 0xED, 0x1D, 0x81, 0xAB, 17, 18, 19, 20));

        EXPECT_THROW(writer << (int64_t)17906342797856292348,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceWriteInt64Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (int64_t)2387559956438732708;
        EXPECT_THAT(test,::testing::ElementsAre(0xA4,0x4F,0x7A, 0x6B, 0x21, 0x51, 0x22, 0x21, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
        writer << (int64_t)-1790072279718329941;
        EXPECT_THAT(test,::testing::ElementsAre(0xA4,0x4F,0x7A, 0x6B, 0x21, 0x51, 0x22, 0x21, 0xAB, 0x81, 0x1D, 0xED, 0xBE, 0x62, 0x28, 0xE7, 17, 18, 19, 20));

        EXPECT_THROW(writer << (int64_t)17906342797856292348,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceWriteBytes) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        data::bytes enter{0xab,0xcd,0xef,0x01};
        writer << enter;
        EXPECT_THAT(test,::testing::ElementsAre(0xab,0xcd,0xef,0x01, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20));
    }


    TEST_F(SliceTest, SliceReadUint16Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        uint16_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,258);
        reader >> testUint;
        EXPECT_EQ(testUint,772);
    }

    TEST_F(SliceTest, SliceReadUint16Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        uint16_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,513);
        reader >> testUint;
        EXPECT_EQ(testUint,1027);
    }

    TEST_F(SliceTest, SliceReadInt16Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        int16_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,258);
        reader >> testUint;
        EXPECT_EQ(testUint,772);
    }

    TEST_F(SliceTest, SliceReadInt16Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        int16_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,513);
        reader >> testUint;
        EXPECT_EQ(testUint,1027);
    }

    TEST_F(SliceTest, SliceReadUint32Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        uint32_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,16909060);
        reader >> testUint;
        EXPECT_EQ(testUint,84281096);
    }

    TEST_F(SliceTest, SliceReadUint32Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        uint32_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,67305985);
        reader >> testUint;
        EXPECT_EQ(testUint,134678021);
    }


    TEST_F(SliceTest, SliceReadInt32Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        int32_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,16909060);
        reader >> testUint;
        EXPECT_EQ(testUint,84281096);
    }

    TEST_F(SliceTest, SliceReadInt32Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        int32_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,67305985);
        reader >> testUint;
        EXPECT_EQ(testUint,134678021);
    }

    TEST_F(SliceTest, SliceReadUint64Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        uint64_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,72623859790382856);
        reader >> testUint;
        EXPECT_EQ(testUint,651345242494996240);
        EXPECT_THROW(reader >> testUint,data::end_of_stream);

    }

    TEST_F(SliceTest, SliceReadUint64Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        uint64_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,578437695752307201);
        reader >> testUint;
        EXPECT_EQ(testUint,1157159078456920585);
        EXPECT_THROW(reader >> testUint,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceReadInt64Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        int64_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,72623859790382856);
        reader >> testUint;
        EXPECT_EQ(testUint,651345242494996240);
        EXPECT_THROW(reader >> testUint,data::end_of_stream);

    }

    TEST_F(SliceTest, SliceReadInt64Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        int64_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,578437695752307201);
        reader >> testUint;
        EXPECT_EQ(testUint,1157159078456920585);
        EXPECT_THROW(reader >> testUint,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceReadBytes) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        std::vector<data::byte> testUint(4);
        reader >> testUint;
        EXPECT_THAT(testUint,::testing::ElementsAre(01,02,03,04));
    }
}