#include <gtest/gtest.h>

#include <iostream>

#include <RTI/encoding/BasicDataElements.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/VariableLengthData.h>
#include <libRTI/ieee1516-2010/HLAtypesIEEE1516.hh>

using ::std::cout;
using ::std::endl;
using ::std::stringstream;
using ::rti1516e::HLAASCIIchar;
using ::rti1516e::HLAboolean;
using ::rti1516e::HLAbyte;
using ::rti1516e::HLAfloat32BE;
using ::rti1516e::HLAfloat32LE;
using ::rti1516e::HLAfloat64BE;
using ::rti1516e::HLAfloat64LE;
using ::rti1516e::VariableLengthData;

TEST(HLATypesTest, TestHLASCIIchar)
{
    HLAASCIIchar hlaAsciiCharTestEncode;
    hlaAsciiCharTestEncode = 'c';
    try {
        VariableLengthData variableLengthData = hlaAsciiCharTestEncode.encode();
        size_t size = variableLengthData. size();
        ASSERT_EQ(hlaAsciiCharTestEncode.getEncodedLength(), size);

        const char data = static_cast<const char*>(variableLengthData.data())[size-1];
        ASSERT_EQ(hlaAsciiCharTestEncode.get(), data);

        HLAASCIIchar hlaAsciiCharTestDecode;
        hlaAsciiCharTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaAsciiCharTestEncode.get(), hlaAsciiCharTestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAboolean)
{
    HLAboolean hlaBooleanTestEncode;
    hlaBooleanTestEncode = ::libhla::HLAfalse;

    try
    {
        VariableLengthData variableLengthData = hlaBooleanTestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaBooleanTestEncode.getEncodedLength(), size);

        const bool data = static_cast<const bool*>(variableLengthData.data())[size-1];
        ASSERT_EQ(hlaBooleanTestEncode.get(), data);

        HLAboolean hlaBooleanTestDecode;
        hlaBooleanTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaBooleanTestEncode.get(), hlaBooleanTestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAbyte)
{
    HLAbyte hlaByteTestEncode;
    hlaByteTestEncode = -12;
    try
    {
        VariableLengthData variableLengthData = hlaByteTestEncode.encode();
        size_t size = variableLengthData. size();
        ASSERT_EQ(hlaByteTestEncode.getEncodedLength(), size);

        const char data = static_cast<const char*>(variableLengthData.data())[size-1];
        ASSERT_EQ(hlaByteTestEncode.get(), data);

        HLAbyte hlaByteTestDecode;
        hlaByteTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaByteTestEncode.get(), hlaByteTestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAfloat32BE)
{
    HLAfloat32BE hlaFloat32BETestEncode;
    hlaFloat32BETestEncode = 12.56;
    try
    {
        VariableLengthData variableLengthData = hlaFloat32BETestEncode.encode();
        size_t size = variableLengthData. size();
        ASSERT_EQ(hlaFloat32BETestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const float data = *static_cast<const float*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAfloat32BE hlaFloat32BETestDecode;
        hlaFloat32BETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaFloat32BETestEncode.get(), hlaFloat32BETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAfloat32LE)
{
    HLAfloat32LE hlaFloat32LETestEncode;
    hlaFloat32LETestEncode = 12.56;
    try
    {
        VariableLengthData variableLengthData = hlaFloat32LETestEncode.encode();
        size_t size = variableLengthData. size();
        ASSERT_EQ(hlaFloat32LETestEncode.getEncodedLength(), size);
#ifndef HOST_IS_BIG_ENDIAN
        const float data = *static_cast<const float*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32LETestEncode.get(), data);
#endif
        HLAfloat32LE hlaFloat32LETestDecode;
        hlaFloat32LETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaFloat32LETestEncode.get(), hlaFloat32LETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAfloat64BE)
{
    HLAfloat64BE hlaFloat64BETestEncode;
    hlaFloat64BETestEncode = 12.56;
    try
    {
        VariableLengthData variableLengthData = hlaFloat64BETestEncode.encode();
        size_t size = variableLengthData. size();
        ASSERT_EQ(hlaFloat64BETestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat32BETestEncode.get(), data);
#endif
        HLAfloat64BE hlaFloat64BETestDecode;
        hlaFloat64BETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaFloat64BETestEncode.get(), hlaFloat64BETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAfloat64LE)
{
    HLAfloat64LE hlaFloat64LETestEncode;
    hlaFloat64LETestEncode = 12.56;
    try
    {
        VariableLengthData variableLengthData = hlaFloat64LETestEncode.encode();
        size_t size = variableLengthData. size();
        ASSERT_EQ(hlaFloat64LETestEncode.getEncodedLength(), size);
#ifndef HOST_IS_BIG_ENDIAN
        const double data = *static_cast<const double*>(variableLengthData.data());
        ASSERT_EQ(hlaFloat64LETestEncode.get(), data);
#endif
        HLAfloat64LE hlaFloat64LETestDecode;
        hlaFloat64LETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaFloat64LETestEncode.get(), hlaFloat64LETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}
