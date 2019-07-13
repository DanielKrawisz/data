//
// Created by nekosune on 13/07/19.
//

#include "data/slice.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {
    void DumpSliceToConsole(data::slice<data::byte> slice)
    {
        std::cout << std::endl;
        for (int i = 0; i < slice.size(); i++)
            std::cout << (int)slice[i] << ", ";
        std::cout << std::endl;
    }

class SliceTest : public ::testing::Test {
protected:
    void SetUp() override {
        test=std::vector<data::byte>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
        sliceTest=data::slice<data::byte>(test);
    }
    std::vector<data::byte> test;
    data::slice<data::byte> sliceTest;
};


    TEST_F(SliceTest, SliceWriteUint16Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (uint16_t)4096;
        EXPECT_THAT(test,::testing::ElementsAre(16,00,3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
        writer << (uint16_t)9023;
        EXPECT_THAT(test,::testing::ElementsAre(16,00,35, 63, 5, 6, 7, 8, 9, 10, 11, 12));
    }

    TEST_F(SliceTest, SliceWriteUint16Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (uint16_t)4096;
        EXPECT_THAT(test,::testing::ElementsAre(00,16,3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
        writer << (uint16_t)9023;
        EXPECT_THAT(test,::testing::ElementsAre(00,16,63, 35, 5, 6, 7, 8, 9, 10, 11, 12));
    }



    TEST_F(SliceTest, SliceWriteUint32Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (uint32_t)2859322050;
        EXPECT_THAT(test,::testing::ElementsAre(0xAA,0x6D,0xCA, 0xC2, 5, 6, 7, 8, 9, 10, 11, 12));
        writer << (uint32_t)2805395218;
        EXPECT_THAT(test,::testing::ElementsAre(0xAA,0x6D,0xCA, 0xC2, 0xA7, 0x36, 0xEF, 0x12, 9, 10, 11, 12));
    }

    TEST_F(SliceTest, WriteUint32Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (uint32_t)2859322050;
        EXPECT_THAT(test,::testing::ElementsAre(0xC2,0xCA,0x6D,0xAA , 5, 6, 7, 8, 9, 10, 11, 12));
        writer << (uint32_t)2805395218;
        EXPECT_THAT(test,::testing::ElementsAre(0xC2,0xCA,0x6D,0xAA , 0x12, 0xEF, 0x36, 0xA7, 9, 10, 11, 12));
    }


    TEST_F(SliceTest, SliceWriteUint64Big) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        writer << (uint64_t)2387559956438732708;
        EXPECT_THAT(test,::testing::ElementsAre(0x21,0x22,0x51, 0x21, 0x6B, 0x7A, 0x4F, 0xA4, 9, 10, 11, 12));
        EXPECT_THROW(writer << (uint64_t)17900722797183299418,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceWriteUint64Little) {
        data::slice_writer writer(sliceTest,boost::endian::order::little);
        writer << (uint64_t)2387559956438732708;
        EXPECT_THAT(test,::testing::ElementsAre(0xA4,0x4F,0x7A, 0x6B, 0x21, 0x51, 0x22, 0x21, 9, 10, 11, 12));
        EXPECT_THROW(writer << (uint64_t)17900722797183299418,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceWriteBytes) {
        data::slice_writer writer(sliceTest,boost::endian::order::big);
        data::bytes enter{0xab,0xcd,0xef,0x01};
        writer << enter;
        EXPECT_THAT(test,::testing::ElementsAre(0xab,0xcd,0xef,0x01, 5, 6, 7, 8, 9, 10, 11, 12));
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

    TEST_F(SliceTest, SliceReadUint64Big) {
        data::slice_reader reader(sliceTest,boost::endian::order::big);
        uint64_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,72623859790382856);
        EXPECT_THROW(reader >> testUint,data::end_of_stream);

    }

    TEST_F(SliceTest, SliceReadUint64Little) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        uint64_t  testUint;
        reader >> testUint;
        EXPECT_EQ(testUint,578437695752307201);
        EXPECT_THROW(reader >> testUint,data::end_of_stream);
    }

    TEST_F(SliceTest, SliceReadBytes) {
        data::slice_reader reader(sliceTest,boost::endian::order::little);
        std::vector<data::byte> testUint(4);
        reader >> testUint;
        EXPECT_THAT(testUint,::testing::ElementsAre(01,02,03,04));
    }
}