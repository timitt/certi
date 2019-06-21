#include <gtest/gtest.h>

#include <iostream>

#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>

using ::std::cout;
using ::std::endl;
using ::std::stringstream;

using ::rti1516e::HLAASCIIchar;
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
