#include <gtest/gtest.h>
#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/encoding/HLAfixedArray.h>
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>

using ::rti1516e::HLAvariableArray;
using ::rti1516e::HLAfixedArray;

using ::rti1516e::DataElement;
using ::rti1516e::HLAASCIIchar;
using ::rti1516e::HLAbyte;
using ::rti1516e::HLAfloat64BE;
using ::rti1516e::HLAfloat64LE;
//using ::rti1516e::HLA3Byte;
using ::rti1516e::VariableLengthData;

TEST(HLAComplexeTypesTest, TestHLAvariableArray)
{
    try
    {
        HLAASCIIchar asciiChar_1('a');
        HLAbyte byte_2(8);
        HLAvariableArray hlaVariableArray(asciiChar_1);
        hlaVariableArray.addElement(byte_2);
        FAIL();
    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }

    try
    {
        HLAASCIIchar asciiChar_1('a');
        HLAASCIIchar asciiChar_2('b');
        HLAvariableArray hlaVariableArray(asciiChar_1);
        hlaVariableArray.addElement(asciiChar_1);
        hlaVariableArray.addElement(asciiChar_2);
        HLAASCIIchar asciiCharReturn(*static_cast<HLAASCIIchar*>(hlaVariableArray.get(1).clone().get()));

        ASSERT_EQ(asciiChar_2.get(), asciiCharReturn.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    try
    {
        HLAASCIIchar asciiChar_1('a');
        HLAASCIIchar asciiChar_2('b');
        HLAvariableArray hlaVariableArray(asciiChar_1);
        hlaVariableArray.addElement(asciiChar_1);
        hlaVariableArray.addElement(asciiChar_2);
        hlaVariableArray.get(2);
        FAIL() << "Index out of range is not handled";
    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }

    try
    {
        HLAfloat64BE float64BE_1_encode(1.0);
        HLAfloat64BE float64BE_2_encode(2.0);
        HLAvariableArray hlaVariableArrayEncode(float64BE_1_encode);
        hlaVariableArrayEncode.addElement(float64BE_1_encode);
        hlaVariableArrayEncode.addElement(float64BE_2_encode);
        VariableLengthData vld = hlaVariableArrayEncode.encode();

        HLAvariableArray hlaVariableArrayDecode(float64BE_1_encode);
        hlaVariableArrayDecode.decode(vld);
        HLAfloat64BE float64BE_1_decode(*static_cast<HLAfloat64BE*>(hlaVariableArrayDecode.get(0).clone().get()));
        HLAfloat64BE float64BE_2_decode(0.0);
        double val = static_cast<HLAfloat64BE*>(hlaVariableArrayDecode.get(1).clone().get())->get();
        float64BE_2_decode.set(val);

        ASSERT_EQ(float64BE_1_encode.get(), float64BE_1_decode.get());
        ASSERT_EQ(float64BE_2_encode.get(), float64BE_2_decode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }


//    HLA3Byte hla3Byte_1(16);
//    HLA3Byte hla3Byte_2(32);
//    HLA3Byte hla3Byte_3(64);
//    HLA3Byte hla3Byte_4(128);
////    HLAvariableArray hlaVariableArray1(hla3Byte_1);
////    hlaVariableArray1.addElement(hla3Byte_1);
//    HLAvariableArray hlaVariableArray2(hla3Byte_2);
//    hlaVariableArray2.addElement(hla3Byte_2);
//    hlaVariableArray2.addElement(hla3Byte_3);
//    hlaVariableArray2.addElement(hla3Byte_4);
//    VariableLengthData vld = hlaVariableArray2.encode();
//    hlaVariableArray2.decode(vld);
//    HLAvariableArray hlaVariableArray(hlaVariableArray1);
//    hlaVariableArray.addElement(hlaVariableArray1);
//    hlaVariableArray.addElement(hlaVariableArray2);
//    hlaVariableArray.encode();
}

TEST(HLAComplexeTypesTest, TestHLAfixedArray)
{
    try
    {
        HLAfloat64BE float64BE_1_encode(1.0);
        HLAfloat64BE float64BE_2_encode(2.0);
        HLAfixedArray hlaFixedArrayEncode(float64BE_1_encode, 2);
        hlaFixedArrayEncode.set(0, float64BE_1_encode);
        hlaFixedArrayEncode.set(1, float64BE_2_encode);
        VariableLengthData vld = hlaFixedArrayEncode.encode();

        HLAfixedArray hlaFixedArrayDecode(float64BE_1_encode, 2);
        hlaFixedArrayDecode.decode(vld);
        HLAfloat64BE float64BE_1_decode(*static_cast<HLAfloat64BE*>(hlaFixedArrayDecode.get(0).clone().get()));
        HLAfloat64BE float64BE_2_decode(0.0);
        double val = static_cast<HLAfloat64BE*>(hlaFixedArrayDecode.get(1).clone().get())->get();
        float64BE_2_decode.set(val);

        ASSERT_EQ(float64BE_1_encode.get(), float64BE_1_decode.get());
        ASSERT_EQ(float64BE_2_encode.get(), float64BE_2_decode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    try
    {
        HLAASCIIchar asciiChar_1_encode('a');
        HLAASCIIchar asciiChar_2_encode('b');
        HLAASCIIchar asciiChar_3_encode('c');
        HLAASCIIchar asciiChar_4_encode('d');
        HLAfixedArray hlaFixedArrayEncode(asciiChar_1_encode, 4);
        hlaFixedArrayEncode.set(0, asciiChar_1_encode);
        hlaFixedArrayEncode.set(1, asciiChar_2_encode);
        hlaFixedArrayEncode.set(2, asciiChar_3_encode);
        hlaFixedArrayEncode.set(3, asciiChar_4_encode);
        VariableLengthData vld = hlaFixedArrayEncode.encode();

        HLAfixedArray hlaFixedArrayDecode(asciiChar_1_encode, 4);
        hlaFixedArrayDecode.decode(vld);
        HLAASCIIchar asciiChar_1_decode(*static_cast<HLAASCIIchar*>(hlaFixedArrayDecode.get(0).clone().get()));
        HLAASCIIchar asciiChar_2_decode('a');
        char val = static_cast<HLAASCIIchar*>(hlaFixedArrayDecode.get(1).clone().get())->get();
        asciiChar_2_decode.set(val);
        HLAASCIIchar asciiChar_3_decode(*static_cast<HLAASCIIchar*>(hlaFixedArrayDecode.get(2).clone().get()));
        HLAASCIIchar asciiChar_4_decode(*static_cast<HLAASCIIchar*>(hlaFixedArrayDecode.get(3).clone().get()));

        ASSERT_EQ(asciiChar_1_encode.get(), asciiChar_1_decode.get());
        ASSERT_EQ(asciiChar_2_encode.get(), asciiChar_2_decode.get());
        ASSERT_EQ(asciiChar_3_encode.get(), asciiChar_3_decode.get());
        ASSERT_EQ(asciiChar_4_encode.get(), asciiChar_4_decode.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        FAIL() << e.what();
    }

    try
    {
        HLAASCIIchar asciiChar_1_encode('a');
        HLAASCIIchar asciiChar_2_encode('b');
        HLAASCIIchar asciiChar_3_encode('c');
        HLAASCIIchar asciiChar_4_encode('d');
        HLAfixedArray hlaFixedArrayEncode(asciiChar_1_encode, 2);
        hlaFixedArrayEncode.set(0, asciiChar_1_encode);
        hlaFixedArrayEncode.set(1, asciiChar_2_encode);
        hlaFixedArrayEncode.set(2, asciiChar_3_encode);
        hlaFixedArrayEncode.set(3, asciiChar_4_encode);
        VariableLengthData vld = hlaFixedArrayEncode.encode();
        FAIL() << "Index out of bound is not handled";
    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }

    try
    {
        HLAASCIIchar asciiChar_1_encode('a');
        HLAASCIIchar asciiChar_2_encode('b');
        HLAbyte byte_3_encode('c');
        HLAfixedArray hlaFixedArrayEncode(asciiChar_1_encode, 3);
        hlaFixedArrayEncode.set(0, asciiChar_1_encode);
        hlaFixedArrayEncode.set(1, asciiChar_2_encode);
        hlaFixedArrayEncode.set(2, byte_3_encode);
        VariableLengthData vld = hlaFixedArrayEncode.encode();
        FAIL() << "Different type Exception is not handled";
    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }

}
