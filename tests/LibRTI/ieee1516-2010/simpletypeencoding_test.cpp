#include <gtest/gtest.h>

#include <iostream>

//#include <libHLA/HLAtypesIEEE1516.hh>

#include <RTI/encoding/BasicDataElements.h>

using ::std::cout;
using ::std::endl;
using ::std::stringstream;

TEST(HLATypesTest, TestLittleEndianInt16)
{
    rti1516e::HLAASCIIchar test;
    test = 'a';

//    testType<HLAASCIIchar>(255, "0000:  ff 00\n");
//    testType<HLAASCIIchar>(256, "0000:  00 01\n");
}
