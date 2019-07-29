#include <gtest/gtest.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/time/HLAfloat64Time.h>

using ::rti1516e::HLAfloat64Time;

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
        ASSERT_EQ(hLAfloat64TimeTestDecode.getTime(), hLAfloat64TimeTestEncode.getTime());
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
        ASSERT_EQ(hLAfloat64TimeTestDecode1.getTime(), hLAfloat64TimeTestDecode1.getTime());
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


}

