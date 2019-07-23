#include <gtest/gtest.h>
#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/encoding/HLAfixedArray.h>
#include <RTI/encoding/HLAfixedRecord.h>
#include <RTI/encoding/HLAvariantRecord.h>
#include <RTI/encoding/HLAopaqueData.h>
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>
#include <RTI/encoding/EncodingConfig.h>

using ::rti1516e::HLAvariableArray;
using ::rti1516e::HLAfixedArray;
using ::rti1516e::HLAfixedRecord;
using ::rti1516e::HLAopaqueData;
using ::rti1516e::HLAvariantRecord;

using ::rti1516e::DataElement;
using ::rti1516e::HLAASCIIchar;
using ::rti1516e::HLAbyte;
using ::rti1516e::HLAfloat64BE;
using ::rti1516e::HLAfloat64LE;
using ::rti1516e::HLAoctet;
using ::rti1516e::HLAinteger32BE;
using ::rti1516e::HLAinteger32LE;
//using ::rti1516e::HLA3Byte;
using ::rti1516e::VariableLengthData;

using ::rti1516e::Octet;

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
        HLAASCIIchar asciiCharReturn(static_cast<const HLAASCIIchar&>(hlaVariableArray.get(1)));

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
        HLAfloat64BE float64BE_1_decode(static_cast<const HLAfloat64BE&>(hlaVariableArrayDecode.get(0)));
        HLAfloat64BE float64BE_2_decode(0.0);
        double val = static_cast<const HLAfloat64BE&>(hlaVariableArrayDecode.get(1)).get();
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
        HLAfloat64BE float64BE_1_decode(static_cast<const HLAfloat64BE&>(hlaFixedArrayDecode.get(0)));
        HLAfloat64BE float64BE_2_decode(0.0);
        double val = static_cast<const HLAfloat64BE&>(hlaFixedArrayDecode.get(1)).get();
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

TEST(HLAComplexeTypesTest, TestHLAfixedRecord)
{
    try
    {
        HLAoctet octet_encode(1);
        HLAinteger32LE integer_encode(2);
        HLAfloat64BE decimal_encode(3);
        HLAfixedRecord hlafixedRecordEncode;
        hlafixedRecordEncode.appendElement(octet_encode);
        hlafixedRecordEncode.appendElement(integer_encode);
        hlafixedRecordEncode.appendElement(decimal_encode);
        VariableLengthData vld = hlafixedRecordEncode.encode();

        Octet test(0x0);
        HLAoctet octetTemp(test);
        HLAinteger32LE integerTemp(0);
        HLAfloat64BE decimalTemp(0.0);
        HLAfixedRecord hlafixedRecordDecode;
        hlafixedRecordDecode.appendElement(octetTemp);
        hlafixedRecordDecode.appendElement(integerTemp);
        hlafixedRecordDecode.appendElement(decimalTemp);
        hlafixedRecordDecode.decode(vld);
        const HLAoctet octet_decode = static_cast<const HLAoctet&>(hlafixedRecordDecode.get(0));
        const HLAinteger32LE integer_decode = static_cast<const HLAinteger32LE&>(hlafixedRecordDecode.get(1));
        const HLAfloat64BE decimal_decode = static_cast<const HLAfloat64BE&>(hlafixedRecordDecode.get(2));

        ASSERT_EQ(octet_encode, octet_decode);
        ASSERT_EQ(integer_encode, integer_decode);
        ASSERT_EQ(decimal_encode, decimal_decode);

        HLAfixedRecord hlafixedRecord2(hlafixedRecordEncode);
        const HLAoctet octet_decode2 = static_cast<const HLAoctet&>(hlafixedRecord2.get(0));
        const HLAinteger32LE integer_decode2 = static_cast<const HLAinteger32LE&>(hlafixedRecord2.get(1));
        const HLAfloat64BE decimal_decode2 = static_cast<const HLAfloat64BE&>(hlafixedRecord2.get(2));

        ASSERT_EQ(octet_encode, octet_decode2);
        ASSERT_EQ(integer_encode, integer_decode2);
        ASSERT_EQ(decimal_encode, decimal_decode2);


    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }

}

TEST(HLAComplexeTypesTest, TestHLAopaqueData)
{
    try
    {
        const size_t dataSize = 4;
        const size_t bufferSize = 4;

        Octet *bufferOctetEncode = new Octet[dataSize];
        bufferOctetEncode[0] = 1;
        bufferOctetEncode[1] = 2;
        bufferOctetEncode[2] = 3;
        bufferOctetEncode[3] = 4;
        HLAopaqueData hLAopaqueDataEncode(&bufferOctetEncode, bufferSize, dataSize);
        VariableLengthData vld = hLAopaqueDataEncode.encode();

        Octet *bufferOctetDecode = new Octet[dataSize];
        bufferOctetDecode[0] = 0;
        bufferOctetDecode[1] = 0;
        bufferOctetDecode[2] = 0;
        bufferOctetDecode[3] = 0;

        HLAopaqueData hLAopaqueDataDecode(&bufferOctetDecode, bufferSize, dataSize);
        hLAopaqueDataDecode.decode(vld);

        ASSERT_EQ(hLAopaqueDataEncode.dataLength(), 4);
        ASSERT_EQ(hLAopaqueDataDecode.dataLength(), 4);
        ASSERT_EQ(hLAopaqueDataEncode.get()[0], hLAopaqueDataDecode.get()[0]);
        ASSERT_EQ(hLAopaqueDataEncode.get()[1], hLAopaqueDataDecode.get()[1]);
        ASSERT_EQ(hLAopaqueDataEncode.get()[2], hLAopaqueDataDecode.get()[2]);
        ASSERT_EQ(hLAopaqueDataEncode.get()[3], hLAopaqueDataDecode.get()[3]);

        if(bufferOctetEncode) {
            delete []bufferOctetEncode;
            bufferOctetEncode = NULL;
        }

        if(bufferOctetDecode) {
            delete []bufferOctetDecode;
            bufferOctetDecode = NULL;
        }

        std::vector<Octet> vectorOctetEncode;
        vectorOctetEncode.push_back(1);
        vectorOctetEncode.push_back(2);
        vectorOctetEncode.push_back(3);
        vectorOctetEncode.push_back(4);

        HLAopaqueData hLAopaqueDataAutoMemoryEncode(&vectorOctetEncode[0], vectorOctetEncode.size());
        VariableLengthData vldAutoMemory = hLAopaqueDataAutoMemoryEncode.encode();

        std::vector<Octet> vectorOctetDecode;
        vectorOctetDecode.push_back(0);
        vectorOctetDecode.push_back(0);
        vectorOctetDecode.push_back(0);
        vectorOctetDecode.push_back(0);

        HLAopaqueData hLAopaqueDataAutoMemoryDecode(&vectorOctetDecode[0], vectorOctetDecode.size());
        hLAopaqueDataAutoMemoryDecode.decode(vldAutoMemory);

        ASSERT_EQ(hLAopaqueDataAutoMemoryEncode.dataLength(), 4);
        ASSERT_EQ(hLAopaqueDataAutoMemoryDecode.dataLength(), 4);
        ASSERT_EQ(hLAopaqueDataAutoMemoryEncode.get()[0], hLAopaqueDataAutoMemoryDecode.get()[0]);
        ASSERT_EQ(hLAopaqueDataAutoMemoryEncode.get()[1], hLAopaqueDataAutoMemoryDecode.get()[1]);
        ASSERT_EQ(hLAopaqueDataAutoMemoryEncode.get()[2], hLAopaqueDataAutoMemoryDecode.get()[2]);
        ASSERT_EQ(hLAopaqueDataAutoMemoryEncode.get()[3], hLAopaqueDataAutoMemoryDecode.get()[3]);
    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }

}

TEST(HLAComplexeTypesTest, HLAvariantRecord)
{
    try
    {
        HLAoctet discriminentEncoder_1(1);
        HLAoctet discriminentEncoder_2(2);

        HLAinteger32BE valueEncoder_1(10);
        HLAoctet valueEncoder_2(20);
        HLAoctet valueEncoder_3(30);

        HLAfixedRecord fixeRecordEncoder;
        fixeRecordEncoder.appendElement(valueEncoder_2);
        fixeRecordEncoder.appendElement(valueEncoder_3);

        HLAvariantRecord variantEncoder(discriminentEncoder_1);
        variantEncoder.addVariant(discriminentEncoder_1, valueEncoder_1);
        variantEncoder.addVariant(discriminentEncoder_2, fixeRecordEncoder);
        VariableLengthData vldEncodeOctets = variantEncoder.encode();
        variantEncoder.setDiscriminant(discriminentEncoder_1);
        VariableLengthData vldEncodeInteger = variantEncoder.encode();

        HLAoctet discriminentDecoder_1(1);
        HLAoctet discriminentDecoder_2(2);

        HLAinteger32BE valueDecoder_1(-1);
        HLAoctet valueDecoder_2(-1);
        HLAoctet valueDecoder_3(-1);

        HLAfixedRecord fixeRecordDecoder;
        fixeRecordDecoder.appendElement(valueDecoder_2);
        fixeRecordDecoder.appendElement(valueDecoder_3);

        HLAvariantRecord variantDecoder(discriminentDecoder_1);
        variantDecoder.addVariant(discriminentDecoder_1, valueDecoder_1);
        variantDecoder.addVariant(discriminentDecoder_2, fixeRecordDecoder);
        variantDecoder.decode(vldEncodeOctets);

        HLAfixedRecord fixeRecordDecoderCast(static_cast<const HLAfixedRecord&>(variantDecoder.getVariant()));
        valueDecoder_2 = static_cast<const HLAoctet&>(fixeRecordDecoderCast.get(0));
        valueDecoder_3 = static_cast<const HLAoctet&>(fixeRecordDecoderCast.get(1));

        variantDecoder.setDiscriminant(discriminentDecoder_1);
        variantDecoder.decode(vldEncodeInteger);

        valueDecoder_1 = static_cast<const HLAinteger32BE&>(variantDecoder.getVariant());

        ASSERT_EQ(valueEncoder_1.get(), valueDecoder_1.get());
        ASSERT_EQ(valueEncoder_2.get(), valueDecoder_2.get());
        ASSERT_EQ(valueEncoder_3.get(), valueDecoder_3.get());
    }
    catch(rti1516e::EncoderException& e)
    {
        SUCCEED() << e.what();
    }
}
