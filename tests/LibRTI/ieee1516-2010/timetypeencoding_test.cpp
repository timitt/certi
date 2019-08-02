#include <gtest/gtest.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/time/HLAfloat64Time.h>
#include <RTI/time/HLAfloat64TimeFactory.h>
#include <RTI/time/HLAfloat64Interval.h>
#include <RTI/time/HLAinteger64Time.h>
#include <RTI/time/HLAinteger64Interval.h>
#include <RTI/time/HLAinteger64TimeFactory.h>

using ::rti1516e::HLAfloat64Time;
using ::rti1516e::HLAfloat64TimeFactory;
using ::rti1516e::HLAfloat64Interval;
using ::rti1516e::HLAinteger64Time;
using ::rti1516e::HLAinteger64Interval;
using ::rti1516e::HLAinteger64TimeFactory;

TEST(HLATimeTypesTest, TestHLAfloat64Time)
{
    HLAfloat64Time *hlaFloat64Time = new HLAfloat64Time();

    std::cout << std::endl;

    delete hlaFloat64Time;

    HLAfloat64Time hLAfloat64TimeTestEncode;
    hLAfloat64TimeTestEncode = 12.56;

    try
    {
        rti1516e::VariableLengthData variableLengthData = hLAfloat64TimeTestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hLAfloat64TimeTestEncode.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAfloat64Time hLAfloat64TimeTestDecode;
        hLAfloat64TimeTestDecode.decode(variableLengthData);
        ASSERT_FLOAT_EQ(hLAfloat64TimeTestDecode.getTime(), hLAfloat64TimeTestEncode.getTime());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAfloat64Time hLAfloat64TimeTestEncode1;
    hLAfloat64TimeTestEncode1 = 5.47;
    char tab[9];
    void *ptr = &tab;
    try
    {
        size_t size = hLAfloat64TimeTestEncode1.encode(ptr, 9);
        ASSERT_EQ(hLAfloat64TimeTestEncode1.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAfloat64Time hLAfloat64TimeTestDecode1;
        hLAfloat64TimeTestDecode1.decode(ptr, size);
        ASSERT_FLOAT_EQ(hLAfloat64TimeTestDecode1.getTime(), hLAfloat64TimeTestDecode1.getTime());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAfloat64Time hLAfloat64TimeTestEncode2;
    hLAfloat64TimeTestEncode2 = 5.47;
    char tab3[4];
    void *ptr3 = tab3;
    ASSERT_THROW(hLAfloat64TimeTestEncode2.encode(ptr3, 4), rti1516e::CouldNotEncode);

    HLAfloat64Interval hlaFloat64Interval(15.478);
    HLAfloat64Time hLAfloat64TimeTestADD(10);

    hLAfloat64TimeTestADD += hlaFloat64Interval;
    ASSERT_FLOAT_EQ(hLAfloat64TimeTestADD.getTime(), 25.478);

    hLAfloat64TimeTestADD -= hlaFloat64Interval;
    ASSERT_FLOAT_EQ(hLAfloat64TimeTestADD.getTime(), 10);

}

TEST(HLATimeTypesTest, TestHLAfloat64Interval)
{
    HLAfloat64Interval *hlaFloat64Interval = new HLAfloat64Interval();

    std::cout << std::endl;

    delete hlaFloat64Interval;

    HLAfloat64Interval hlaFloat64IntervalTestEncode;
    hlaFloat64IntervalTestEncode = 458.4;

    try
    {
        rti1516e::VariableLengthData variableLengthData = hlaFloat64IntervalTestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaFloat64IntervalTestEncode.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAfloat64Interval hlaFloat64IntervalTestDecode;
        hlaFloat64IntervalTestDecode.decode(variableLengthData);
        ASSERT_FLOAT_EQ(hlaFloat64IntervalTestDecode.getInterval(), hlaFloat64IntervalTestDecode.getInterval());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAfloat64Interval hlaFloat64IntervalTestEncode1;
    hlaFloat64IntervalTestEncode1 = 9.54;
    char tab[9];
    void *ptr = &tab;
    try
    {
        size_t size = hlaFloat64IntervalTestEncode1.encode(ptr, 9);
        ASSERT_EQ(hlaFloat64IntervalTestEncode1.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAfloat64Interval hlaFloat64IntervalTestDecode1;
        hlaFloat64IntervalTestDecode1.decode(ptr, size);
        ASSERT_FLOAT_EQ(hlaFloat64IntervalTestDecode1.getInterval(), hlaFloat64IntervalTestDecode1.getInterval());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAfloat64Interval hlaFloat64IntervalTestEncode2;
    hlaFloat64IntervalTestEncode2 = 7.58;
    char tab3[4];
    void *ptr3 = tab3;
    ASSERT_THROW(hlaFloat64IntervalTestEncode2.encode(ptr3, 4), rti1516e::CouldNotEncode);

    HLAfloat64Interval hlaFloat64IntervalTestDifferences;

    HLAfloat64Time hLAfloat64Time0(458.56);
    HLAfloat64Time hLAfloat64Time1(448.56);

    hlaFloat64IntervalTestDifferences.setToDifference(hLAfloat64Time0, hLAfloat64Time0);
    ASSERT_FLOAT_EQ(hlaFloat64IntervalTestDifferences.getInterval(), 0);
    hlaFloat64IntervalTestDifferences.setToDifference(hLAfloat64Time0, hLAfloat64Time1);
    ASSERT_FLOAT_EQ(hlaFloat64IntervalTestDifferences.getInterval(), 10);
    hLAfloat64Time1.setTime(0);
    hlaFloat64IntervalTestDifferences.setToDifference(hLAfloat64Time0, hLAfloat64Time1);
    ASSERT_FLOAT_EQ(hlaFloat64IntervalTestDifferences.getInterval(), 458.56);
    hlaFloat64IntervalTestDifferences.setToDifference(hLAfloat64Time1, hLAfloat64Time0);
    ASSERT_FLOAT_EQ(hlaFloat64IntervalTestDifferences.getInterval(), -458.56);

}

TEST(HLATimeTypesTest, TestHLAInteger64Time)
{
    HLAinteger64Time *hlaInteger64Time = new HLAinteger64Time();

    std::cout << std::endl;

    delete hlaInteger64Time;

    HLAinteger64Time hlaInteger64TimeTestEncode;
    hlaInteger64TimeTestEncode = 12;

    try
    {
        rti1516e::VariableLengthData variableLengthData = hlaInteger64TimeTestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger64TimeTestEncode.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAinteger64Time hLAinteger64TimeTestDecode;
        hLAinteger64TimeTestDecode.decode(variableLengthData);
        ASSERT_EQ(hLAinteger64TimeTestDecode.getTime(), hlaInteger64TimeTestEncode.getTime());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAinteger64Time hlaInteger64TimeTestEncode1;
    hlaInteger64TimeTestEncode1 = 5;
    char tab[9];
    void *ptr = &tab;
    try
    {
        size_t size = hlaInteger64TimeTestEncode1.encode(ptr, 9);
        ASSERT_EQ(hlaInteger64TimeTestEncode1.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAinteger64Time hLAinteger64TimeTestDecode1;
        hLAinteger64TimeTestDecode1.decode(ptr, size);
        ASSERT_EQ(hLAinteger64TimeTestDecode1.getTime(), hLAinteger64TimeTestDecode1.getTime());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAinteger64Time hlaInteger64TimeTestEncode2;
    hlaInteger64TimeTestEncode2 = 5;
    char tab3[4];
    void *ptr3 = tab3;
    ASSERT_THROW(hlaInteger64TimeTestEncode2.encode(ptr3, 4), rti1516e::CouldNotEncode);

    HLAinteger64Interval hlainteger64Interval(15);
    HLAinteger64Time hLAinteger64TimeTestADD(10);

    hLAinteger64TimeTestADD += hlainteger64Interval;
    ASSERT_EQ(hLAinteger64TimeTestADD.getTime(), 25);

    hLAinteger64TimeTestADD -= hlainteger64Interval;
    ASSERT_EQ(hLAinteger64TimeTestADD.getTime(), 10);

}

TEST(HLATimeTypesTest, TestHLAinteger64Interval)
{
    HLAinteger64Interval *hlaInteger64Interval = new HLAinteger64Interval();

    std::cout << std::endl;

    delete hlaInteger64Interval;

    HLAinteger64Interval hlaInteger64IntervalTestEncode;
    hlaInteger64IntervalTestEncode = 458;

    try
    {
        rti1516e::VariableLengthData variableLengthData = hlaInteger64IntervalTestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger64IntervalTestEncode.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAinteger64Interval hlaInteger64IntervalTestDecode;
        hlaInteger64IntervalTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger64IntervalTestDecode.getInterval(), hlaInteger64IntervalTestEncode.getInterval());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAinteger64Interval hlaInteger64IntervalTestEncode1;
    hlaInteger64IntervalTestEncode1 = 9;
    char tab[9];
    void *ptr = &tab;
    try
    {
        size_t size = hlaInteger64IntervalTestEncode1.encode(ptr, 9);
        ASSERT_EQ(hlaInteger64IntervalTestEncode1.encodedLength(), size);

#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAinteger64Interval hlaInteger64IntervalTestDecode1;
        hlaInteger64IntervalTestDecode1.decode(ptr, size);
        ASSERT_EQ(hlaInteger64IntervalTestDecode1.getInterval(), hlaInteger64IntervalTestEncode1.getInterval());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    HLAinteger64Interval hlaInteger64IntervalTestEncode2;
    hlaInteger64IntervalTestEncode2 = 7;
    char tab3[4];
    void *ptr3 = tab3;
    ASSERT_THROW(hlaInteger64IntervalTestEncode2.encode(ptr3, 4), rti1516e::CouldNotEncode);

    HLAinteger64Interval hlaInteger64IntervalTestDifferences;

    HLAinteger64Time hLAInteger64Time0(458);
    HLAinteger64Time hLAInteger64Time1(448);

    hlaInteger64IntervalTestDifferences.setToDifference(hLAInteger64Time0, hLAInteger64Time0);
    ASSERT_EQ(hlaInteger64IntervalTestDifferences.getInterval(), 0);
    hlaInteger64IntervalTestDifferences.setToDifference(hLAInteger64Time0, hLAInteger64Time1);
    ASSERT_EQ(hlaInteger64IntervalTestDifferences.getInterval(), 10);
    hLAInteger64Time1.setTime(0);
    hlaInteger64IntervalTestDifferences.setToDifference(hLAInteger64Time0, hLAInteger64Time1);
    ASSERT_EQ(hlaInteger64IntervalTestDifferences.getInterval(), 458);
    hlaInteger64IntervalTestDifferences.setToDifference(hLAInteger64Time1, hLAInteger64Time0);
    ASSERT_EQ(hlaInteger64IntervalTestDifferences.getInterval(), -458);

}

TEST(HLATimeTypesTest, TestHLAfloat64TimeFactory)
{
    HLAfloat64TimeFactory factory;
    std::auto_ptr<HLAfloat64Time> time;
    time = factory.makeLogicalTime(1);

    ASSERT_EQ(time->getTime(), 1);

    std::auto_ptr<rti1516e::LogicalTime> time_1 = factory.makeFinal();
    ASSERT_TRUE(time_1->isFinal());
    std::auto_ptr<rti1516e::LogicalTimeInterval> time_2 = factory.makeEpsilon();
    ASSERT_TRUE(time_2->isEpsilon());
    std::auto_ptr<rti1516e::LogicalTimeInterval> time_3 = factory.makeZero();
    ASSERT_TRUE(time_3->isZero());

    HLAfloat64Time hLAfloat64TimeTestEncode;
    hLAfloat64TimeTestEncode = 157.54;
    rti1516e::VariableLengthData variableLengthData = hLAfloat64TimeTestEncode.encode();

    std::auto_ptr<rti1516e::LogicalTime> time_4 = factory.decodeLogicalTime(variableLengthData);
    ASSERT_EQ(time_4->toString(), hLAfloat64TimeTestEncode.toString());

    HLAfloat64Time hLAfloat64TimeTestEncode1;
    hLAfloat64TimeTestEncode1 = 47986.48;
    char tab[9];
    void *ptr = &tab;
    hLAfloat64TimeTestEncode1.encode(tab, 9);

    std::auto_ptr<rti1516e::LogicalTime> time_5 = factory.decodeLogicalTime(ptr, 9);
    ASSERT_EQ(time_5->toString(), hLAfloat64TimeTestEncode1.toString());


    HLAfloat64Interval hLAfloat64IntervalestEncode3;
    hLAfloat64IntervalestEncode3 = 788.448;
    rti1516e::VariableLengthData variableLengthData3 = hLAfloat64IntervalestEncode3.encode();

    std::auto_ptr<rti1516e::LogicalTimeInterval> time_6 = factory.decodeLogicalTimeInterval(variableLengthData3);
    ASSERT_EQ(time_6->toString(), hLAfloat64IntervalestEncode3.toString());

    HLAfloat64Interval hLAfloat64IntervalestEncode4;
    hLAfloat64IntervalestEncode4 = 687.484;
    char tab1[9];
    void *ptr1 = &tab;
    hLAfloat64IntervalestEncode4.encode(ptr1, 9);

    std::auto_ptr<rti1516e::LogicalTimeInterval> time_7 = factory.decodeLogicalTimeInterval(ptr1, 9);
    ASSERT_EQ(time_7->toString(), hLAfloat64IntervalestEncode4.toString());
}

TEST(HLATimeTypesTest, TestHLAintegerTimeFactory)
{
    HLAinteger64TimeFactory factory;
    std::auto_ptr<HLAinteger64Time> time;
    time = factory.makeLogicalTime(1);

    ASSERT_EQ(time->getTime(), 1);

    std::auto_ptr<rti1516e::LogicalTime> time_1 = factory.makeFinal();
    ASSERT_TRUE(time_1->isFinal());
    std::auto_ptr<rti1516e::LogicalTimeInterval> time_2 = factory.makeEpsilon();
    ASSERT_TRUE(time_2->isEpsilon());
    std::auto_ptr<rti1516e::LogicalTimeInterval> time_3 = factory.makeZero();
    ASSERT_TRUE(time_3->isZero());

    HLAinteger64Time hLAinteger64TimeTestEncode;
    hLAinteger64TimeTestEncode = 157.54;
    rti1516e::VariableLengthData variableLengthData = hLAinteger64TimeTestEncode.encode();

    std::auto_ptr<rti1516e::LogicalTime> time_4 = factory.decodeLogicalTime(variableLengthData);
    ASSERT_EQ(time_4->toString(), hLAinteger64TimeTestEncode.toString());

    HLAinteger64Time hLAinteger64TimeTestEncode1;
    hLAinteger64TimeTestEncode1 = 47986.48;
    char tab[9];
    void *ptr = &tab;
    hLAinteger64TimeTestEncode1.encode(tab, 9);

    std::auto_ptr<rti1516e::LogicalTime> time_5 = factory.decodeLogicalTime(ptr, 9);
    ASSERT_EQ(time_5->toString(), hLAinteger64TimeTestEncode1.toString());


    HLAinteger64Interval hLAinteger64IntervalestEncode3;
    hLAinteger64IntervalestEncode3 = 788.448;
    rti1516e::VariableLengthData variableLengthData3 = hLAinteger64IntervalestEncode3.encode();

    std::auto_ptr<rti1516e::LogicalTimeInterval> time_6 = factory.decodeLogicalTimeInterval(variableLengthData3);
    ASSERT_EQ(time_6->toString(), hLAinteger64IntervalestEncode3.toString());

    HLAinteger64Interval hLAinteger64IntervalestEncode4;
    hLAinteger64IntervalestEncode4 = 687.484;
    char tab1[9];
    void *ptr1 = &tab;
    hLAinteger64IntervalestEncode4.encode(ptr1, 9);

    std::auto_ptr<rti1516e::LogicalTimeInterval> time_7 = factory.decodeLogicalTimeInterval(ptr1, 9);
    ASSERT_EQ(time_7->toString(), hLAinteger64IntervalestEncode4.toString());
}
