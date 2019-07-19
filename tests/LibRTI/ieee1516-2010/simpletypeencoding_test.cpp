#include <gtest/gtest.h>

#include <iostream>

#include <RTI/encoding/BasicDataElements.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/VariableLengthData.h>
#include <RTI/encoding/HLAenumerate.h>

#include <bitset>

using ::std::cout;
using ::std::endl;
using ::std::stringstream;
using ::rti1516e::HLAASCIIchar;
using ::rti1516e::HLAASCIIstring;
using ::rti1516e::HLAboolean;
using ::rti1516e::HLAbooleanEnumerate;
using ::rti1516e::HLAbyte;
using ::rti1516e::HLAfloat32BE;
using ::rti1516e::HLAfloat32LE;
using ::rti1516e::HLAfloat64BE;
using ::rti1516e::HLAfloat64LE;
using ::rti1516e::HLAinteger16LE;
using ::rti1516e::HLAinteger16BE;
using ::rti1516e::HLAinteger32LE;
using ::rti1516e::HLAinteger32BE;
using ::rti1516e::HLAinteger64LE;
using ::rti1516e::HLAinteger64BE;
using ::rti1516e::HLAoctetPairLE;
using ::rti1516e::HLAoctetPairBE;
using ::rti1516e::HLAunicodeChar;
using ::rti1516e::HLAunicodeString;
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

TEST(HLATypesTest, TestHLASCIIstring)
{
    HLAASCIIstring hlaAsciiStringTestEncode;
    hlaAsciiStringTestEncode = "abcdefg";
    try {
        VariableLengthData variableLengthData = hlaAsciiStringTestEncode.encode();
        //Take into account padding and 4 Bytes for nbElements
        size_t size = variableLengthData.size() - (hlaAsciiStringTestEncode.getEncodedLength()-1)*3 - 4;
        ASSERT_EQ(hlaAsciiStringTestEncode.getEncodedLength(), size);

        HLAASCIIstring hlaAsciiStringTestDecode;
        hlaAsciiStringTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaAsciiStringTestEncode.get(), hlaAsciiStringTestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAboolean)
{
    HLAboolean hlaBooleanTestEncode;
    hlaBooleanTestEncode = HLAbooleanEnumerate::HLAtrue;

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
    HLAfloat32BE hlaFloat32BETestEncode(12.56);
    try
    {
        VariableLengthData variableLengthData = hlaFloat32BETestEncode.encode();
        size_t size = variableLengthData.size();
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
    HLAfloat32LE hlaFloat32LETestEncode(12.56);
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

TEST(HLATypesTest, TestHLAinteger16LE)
{
    HLAinteger16LE hlaInteger16LETestEncode;
    hlaInteger16LETestEncode = 258;
    try
    {
        VariableLengthData variableLengthData = hlaInteger16LETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger16LETestEncode.getEncodedLength(), size);
#ifndef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer16 data = *static_cast<const rti1516e::Integer16*>(variableLengthData.data());
        ASSERT_EQ(hlaInteger16LETestEncode.get(), data);
#endif
        HLAinteger16LE hlaInteger16LETestDecode;
        hlaInteger16LETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger16LETestEncode.get(), hlaInteger16LETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAinteger16BE)
{
    HLAinteger16BE hlaInteger16BETestEncode;
    hlaInteger16BETestEncode = 258;
    try
    {
        VariableLengthData variableLengthData = hlaInteger16BETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger16BETestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer16 data = *static_cast<const rti1516e::Integer16*>(variableLengthData.data());
        ASSERT_EQ(hlaInteger16BETestEncode.get(), data);
#endif
        HLAinteger16BE hlaInteger16BETestDecode;
        hlaInteger16BETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger16BETestEncode.get(), hlaInteger16BETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAinteger32LE)
{
    HLAinteger32LE hlaInteger32LETestEncode;
    hlaInteger32LETestEncode = 1250000;
    try
    {
        VariableLengthData variableLengthData = hlaInteger32LETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger32LETestEncode.getEncodedLength(), size);
#ifndef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer32 data = *static_cast<const rti1516e::Integer32*>(variableLengthData.data());
        ASSERT_EQ(hlaInteger32LETestEncode.get(), data);
#endif
        HLAinteger32LE hlaInteger32LETestDecode;
        hlaInteger32LETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger32LETestEncode.get(), hlaInteger32LETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAinteger32BE)
{
    HLAinteger32BE hlaInteger32BETestEncode;
    hlaInteger32BETestEncode = 2;
    try
    {
        VariableLengthData variableLengthData = hlaInteger32BETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger32BETestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer32 data = *static_cast<const rti1516e::Integer32*>(variableLengthData.data());
        ASSERT_EQ(hlaInteger32BETestEncode.get(), data);
#endif
        HLAinteger32BE hlaInteger32BETestDecode;
        hlaInteger32BETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger32BETestEncode.get(), hlaInteger32BETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAinteger64LE)
{
    HLAinteger64LE hlaInteger64LETestEncode;
    hlaInteger64LETestEncode = 1250000256;
    try
    {
        VariableLengthData variableLengthData = hlaInteger64LETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger64LETestEncode.getEncodedLength(), size);
#ifndef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer64 data = *static_cast<const rti1516e::Integer64*>(variableLengthData.data());
        ASSERT_EQ(hlaInteger64LETestEncode.get(), data);
#endif
        HLAinteger64LE hlaInteger64LETestDecode;
        hlaInteger64LETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger64LETestEncode.get(), hlaInteger64LETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAinteger64BE)
{
    HLAinteger64BE hlaInteger64BETestEncode;
    hlaInteger64BETestEncode = 1250000256;
    try
    {
        VariableLengthData variableLengthData = hlaInteger64BETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaInteger64BETestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer64 data = *static_cast<const rti1516e::Integer64*>(variableLengthData.data());
        ASSERT_EQ(hlaInteger64BETestEncode.get(), data);
#endif
        HLAinteger64BE hlaInteger64BETestDecode;
        hlaInteger64BETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaInteger64BETestEncode.get(), hlaInteger64BETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAoctetPairLE)
{
    HLAoctetPairLE hlaoctetPairLETestEncode;
    hlaoctetPairLETestEncode = {8, 7};
    try
    {
        VariableLengthData variableLengthData = hlaoctetPairLETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaoctetPairLETestEncode.getEncodedLength(), size);
#ifndef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer8 *data = static_cast<const rti1516e::Integer8*>(variableLengthData.data());
        ASSERT_EQ(hlaoctetPairLETestEncode.get().first, data[0]);
        ASSERT_EQ(hlaoctetPairLETestEncode.get().second, data[1]);
#endif
        HLAoctetPairLE hlaoctetPairLETestDecode;
        hlaoctetPairLETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaoctetPairLETestEncode.get(), hlaoctetPairLETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAoctetPairBE)
{
    HLAoctetPairBE hlaoctetPairBETestEncode;
    hlaoctetPairBETestEncode = {8, 7};
    try
    {
        VariableLengthData variableLengthData = hlaoctetPairBETestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaoctetPairBETestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer8 *data = static_cast<const rti1516e::Integer8*>(variableLengthData.data());
        ASSERT_EQ(hlaoctetPairBETestEncode.get().first, data[0]);
        ASSERT_EQ(hlaoctetPairBETestEncode.get().first, data[1]);
#endif
        HLAoctetPairBE hlaoctetPairBETestDecode;
        hlaoctetPairBETestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaoctetPairBETestEncode.get(), hlaoctetPairBETestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAunicodeChar)
{
    std::cout << "sizeof(wchar_t)" << sizeof(wchar_t) << std::endl;
    HLAunicodeChar hlaUnicodeTestEncode;
    hlaUnicodeTestEncode = 1023;
    try
    {
        VariableLengthData variableLengthData = hlaUnicodeTestEncode.encode();
        size_t size = variableLengthData.size();
        ASSERT_EQ(hlaUnicodeTestEncode.getEncodedLength(), size);
#ifdef HOST_IS_BIG_ENDIAN
        const rti1516e::Integer8 *data = static_cast<const rti1516e::Integer8*>(variableLengthData.data());
        ASSERT_EQ(hlaoctetPairBETestEncode.get().first, data[0]);
        ASSERT_EQ(hlaoctetPairBETestEncode.get().first, data[1]);
#endif
        HLAunicodeChar hlaUnicodeTestDecode;
        hlaUnicodeTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaUnicodeTestEncode.get(), hlaUnicodeTestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}

TEST(HLATypesTest, TestHLAUnicodeString)
{
    std::wstring str(L"abcdefg");
    HLAunicodeString hlaUnicodeStringTestEncode(str);
    try {
        VariableLengthData variableLengthData = hlaUnicodeStringTestEncode.encode();
        //Take into account padding and 4 Bytes for nbElements
        size_t size = variableLengthData.size() - ((hlaUnicodeStringTestEncode.getEncodedLength()/2)-1)*2 - 4;
        ASSERT_EQ(hlaUnicodeStringTestEncode.getEncodedLength(), size);

        HLAunicodeString hlaunicodeStringTestDecode;
        hlaunicodeStringTestDecode.decode(variableLengthData);
        ASSERT_EQ(hlaUnicodeStringTestEncode.get(), hlaunicodeStringTestDecode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }
}
