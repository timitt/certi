#include <gtest/gtest.h>
#include <RTI/time/HLAfloat64Time.h>

using ::rti1516e::HLAfloat64Time;

TEST(HLATimeTypesTest, TestHLAfloat64Time)
{
    HLAfloat64Time *hlaFloat64Time = new HLAfloat64Time();

    std::cout << std::endl;

    delete hlaFloat64Time;
}
