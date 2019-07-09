#include <gtest/gtest.h>
#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>

using ::rti1516e::HLAvariableArray;

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
        HLAASCIIchar asciiCharReturn(*static_cast<HLAASCIIchar*>(hlaVariableArray.get(1).clone().release()));

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


//    int P = 0;
//    int V = 8;
//    int R = (4+P)%V;
//    std::cout << "Reste : " << R << std::endl;
//    P += R == 0 ? 0:(V-R);
//    std::cout << "P : " << P << std::endl;
//    R = (4+P)%V;
//    std::cout << "Reste : " << R << std::endl;

//    HLAfloat64BE float64BE_1(1.0);
//    HLAfloat64BE float64BE_2(2.0);
////    float64BE_1.encode();
//    HLA3Byte hla3Byte_1(16);
//    HLA3Byte hla3Byte_2(32);
//    HLA3Byte hla3Byte_3(64);
//    HLA3Byte hla3Byte_4(128);
////    hla3Byte_1.encode();
//    HLAvariableArray hlaVariableArray1(hla3Byte_1);
//    hlaVariableArray1.addElement(hla3Byte_1);
//    HLAvariableArray hlaVariableArray2(hla3Byte_2);
//    hlaVariableArray2.addElement(hla3Byte_2);
//    hlaVariableArray2.addElement(hla3Byte_3);
//    hlaVariableArray2.addElement(hla3Byte_4);
//    hlaVariableArray2.encode();
////    HLAvariableArray hlaVariableArray(hlaVariableArray1);
////    hlaVariableArray.addElement(hlaVariableArray1);
////    hlaVariableArray.addElement(hlaVariableArray2);
////    hlaVariableArray.encode();

}
