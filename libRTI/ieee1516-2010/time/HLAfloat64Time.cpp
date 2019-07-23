#include <RTI/time/HLAfloat64Time.h>
#include "HLAfloat64TimeImpl.h"

namespace rti1516e
{

HLAfloat64Time::HLAfloat64Time()
{

}

HLAfloat64Time::HLAfloat64Time(const double &value)
{

}

HLAfloat64Time::HLAfloat64Time(const LogicalTime &value)
{

}

HLAfloat64Time::HLAfloat64Time(const HLAfloat64Time &value)
{

}

HLAfloat64Time::~HLAfloat64Time()
{

}

void HLAfloat64Time::setInitial()
{

}

bool HLAfloat64Time::isInitial() const
{

}

void HLAfloat64Time::setFinal()
{

}

bool HLAfloat64Time::isFinal() const
{

}

LogicalTime &HLAfloat64Time::operator=(const LogicalTime &value)
throw (rti1516e::InvalidLogicalTime)
{

}

LogicalTime &HLAfloat64Time::operator+=(const LogicalTimeInterval &addend)
throw (rti1516e::IllegalTimeArithmetic,
       rti1516e::InvalidLogicalTimeInterval)
{

}

LogicalTime &HLAfloat64Time::operator-=(const LogicalTimeInterval &subtrahend)
throw (rti1516e::IllegalTimeArithmetic,
       rti1516e::InvalidLogicalTimeInterval)
{

}

bool HLAfloat64Time::operator>(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{

}

bool HLAfloat64Time::operator<(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{

}

bool HLAfloat64Time::operator==(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{

}

bool HLAfloat64Time::operator>=(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{

}

bool HLAfloat64Time::operator<=(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{

}

VariableLengthData HLAfloat64Time::encode() const
{

}

size_t HLAfloat64Time::encode(void *buffer, size_t bufferSize) const
throw (rti1516e::CouldNotEncode)
{

}

size_t HLAfloat64Time::encodedLength() const
{

}

void HLAfloat64Time::decode(const VariableLengthData &VariableLengthData)
throw (rti1516e::InternalError,
       rti1516e::CouldNotDecode)
{

}

void HLAfloat64Time::decode(void *buffer, size_t bufferSize)
throw (rti1516e::InternalError,
       rti1516e::CouldNotDecode)
{

}

std::__cxx11::wstring HLAfloat64Time::toString() const
{

}

std::__cxx11::wstring HLAfloat64Time::implementationName() const
{

}

double HLAfloat64Time::getTime() const
{

}

void HLAfloat64Time::setTime(double value)
{

}

HLAfloat64Time &HLAfloat64Time::operator=(const HLAfloat64Time &)
throw (rti1516e::InvalidLogicalTime)
{

}

rti1516e::HLAfloat64Time::operator double() const
{

}

}
