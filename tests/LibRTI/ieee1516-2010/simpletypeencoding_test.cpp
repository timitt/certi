#include <gtest/gtest.h>

#include <iostream>

#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>
#include <libHLA/HLAtypesIEEE1516.hh>

using ::std::cout;
using ::std::endl;
using ::std::stringstream;
using ::rti1516e::HLAASCIIchar;
using ::rti1516e::HLAboolean;
using ::rti1516e::HLAbyte;
using ::rti1516e::VariableLengthData;

TEST(HLATypesTest, TestHLASCIIchar)
{
    HLAASCIIchar hlaAsciiCharTestEncode;
    hlaAsciiCharTestEncode = 'b';

    VariableLengthData variableLengthData = hlaAsciiCharTestEncode.encode();
    size_t size = variableLengthData. size();
    ASSERT_EQ(hlaAsciiCharTestEncode.getEncodedLength(), size);

    const char data = static_cast<const char*>(variableLengthData.data())[size-1];
    ASSERT_EQ(hlaAsciiCharTestEncode.get(), data);

    HLAASCIIchar hlaAsciiCharTestDecode;
    hlaAsciiCharTestDecode.decode(variableLengthData);
    ASSERT_EQ(hlaAsciiCharTestEncode.get(), hlaAsciiCharTestDecode.get());
}

TEST(HLATypesTest, TestHLAboolean)
{
    HLAboolean hlaBooleanTestEncode;
    hlaBooleanTestEncode = libhla::HLAfalse;

    VariableLengthData variableLengthData = hlaBooleanTestEncode.encode();
    size_t size = variableLengthData. size();
    ASSERT_EQ(hlaBooleanTestEncode.getEncodedLength(), size);

    const char data = static_cast<const char*>(variableLengthData.data())[size-1];
    ASSERT_EQ(hlaBooleanTestEncode.get(), data);

    HLAboolean hlaBooleanTestDecode;
    hlaBooleanTestDecode.decode(variableLengthData);
    ASSERT_EQ(hlaBooleanTestEncode.get(), hlaBooleanTestDecode.get());
}

TEST(HLATypesTest, TestHLAbytre)
{
    HLAbyte hlaByteTestEncode;
    hlaByteTestEncode = 255;

    VariableLengthData variableLengthData = hlaByteTestEncode.encode();
    size_t size = variableLengthData. size();
    ASSERT_EQ(hlaByteTestEncode.getEncodedLength(), size);

    const char data = static_cast<const char*>(variableLengthData.data())[size-1];
    ASSERT_EQ(hlaByteTestEncode.get(), data);

    HLAbyte hlaByteTestDecode;
    hlaByteTestDecode.decode(variableLengthData);
    ASSERT_EQ(hlaByteTestEncode.get(), hlaByteTestDecode.get());
}
