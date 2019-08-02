#include <RTI/time/HLAfloat64Interval.h>
#include "HLAfloat64IntervalImpl.h"
#include <RTI/time/HLAfloat64Time.h>

namespace rti1516e
{

HLAfloat64Interval::HLAfloat64Interval()
{
    _impl =  new HLAfloat64IntervalImpl();
}

HLAfloat64Interval::HLAfloat64Interval(double time)
{
    _impl = new HLAfloat64IntervalImpl(time);
}

HLAfloat64Interval::HLAfloat64Interval(LogicalTimeInterval const &other)
{
    _impl = new HLAfloat64IntervalImpl(dynamic_cast<const HLAfloat64IntervalImpl &> (other));
}

HLAfloat64Interval::HLAfloat64Interval(const HLAfloat64Interval& other)
{
    _impl = new HLAfloat64IntervalImpl(*other._impl);
}

HLAfloat64Interval::~HLAfloat64Interval () throw ()
{
    if (_impl != NULL)
        delete _impl;
    _impl = NULL;
}

void HLAfloat64Interval::setZero ()
{
    _impl->setInitial();
}

bool HLAfloat64Interval::isZero () const
{
    return _impl->isInitial();
}

void HLAfloat64Interval::setEpsilon ()
{
    _impl->setFinal();
}

bool HLAfloat64Interval::isEpsilon () const
{
    return _impl->isFinal();
}

LogicalTimeInterval& HLAfloat64Interval::operator= (LogicalTimeInterval const & value)
   throw (InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other= dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            *_impl = *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return *this;
}

LogicalTimeInterval& HLAfloat64Interval::operator+=(LogicalTimeInterval const& value)
   throw (IllegalTimeArithmetic,
          InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other= dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            *_impl += *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return *this;
}

LogicalTimeInterval& HLAfloat64Interval::operator-=(LogicalTimeInterval const & value)
   throw (IllegalTimeArithmetic,
          InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other= dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            *_impl -= *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return *this;
}

bool HLAfloat64Interval::operator> (LogicalTimeInterval const & value) const
   throw (InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other=  dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            return *_impl > *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return false;
}

bool HLAfloat64Interval::operator< (LogicalTimeInterval const & value) const
   throw (InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other=  dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            return *_impl < *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return false;
}

bool HLAfloat64Interval::operator== (LogicalTimeInterval const & value) const
   throw (InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other=  dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            return *_impl == *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return false;
}

bool HLAfloat64Interval::operator>= (LogicalTimeInterval const & value) const
   throw (InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other=  dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            return *_impl >= *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return false;
}

bool HLAfloat64Interval::operator<= (LogicalTimeInterval const & value) const
   throw (InvalidLogicalTimeInterval)
{
    try {
        const HLAfloat64Interval& other=  dynamic_cast<const HLAfloat64Interval&>(value);
        if (other._impl != NULL)
            return *_impl <= *other._impl;
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Interval");
    }
    return false;
}


void HLAfloat64Interval::setToDifference (LogicalTime const & minuend, LogicalTime const& subtrahend)
   throw (IllegalTimeArithmetic,
          InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& val0=  static_cast<const HLAfloat64Time&>(minuend);
        const HLAfloat64Time& val1=  static_cast<const HLAfloat64Time&>(subtrahend);
        if (val0._impl != NULL && val1._impl != NULL)
            this->setInterval(val0.getTime() - val1.getTime());
    } catch(const std::bad_cast& e) {
        throw InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
}


std::wstring HLAfloat64Interval::toString () const
{
    return _impl->toString();
}

std::wstring HLAfloat64Interval::implementationName() const
{
    const std::wstring implName(L"HLAfloat64Interval");
    return implName;
}

double HLAfloat64Interval::getInterval() const
{
    return _impl->getTime();
}

void HLAfloat64Interval::setInterval(double value)
{
    _impl->setTime(value);
}

HLAfloat64Interval& HLAfloat64Interval::operator=(HLAfloat64Interval const& other)
   throw (InvalidLogicalTimeInterval)
{
    *_impl = *other._impl;
    return *this;
}

VariableLengthData HLAfloat64Interval::encode() const
{
    VariableLengthData vlData;
    std::vector<Octet> buffer(encodedLength());
    encode(&buffer[0], buffer.size());
    vlData.setData(&buffer[0], buffer.size());
    return vlData;
}

size_t HLAfloat64Interval::encode(void* buffer, size_t bufferSize) const
   throw (CouldNotEncode)
{
    return _impl->encode(buffer, bufferSize);
}

void HLAfloat64Interval::decode(VariableLengthData const & encodedValue)
   throw (InternalError,
          CouldNotDecode)
{
    std::vector<Octet> buffer(encodedValue.size());
    std::memcpy(&buffer[0], encodedValue.data(), encodedValue.size());
    decode(&buffer[0], buffer.size());
}

void HLAfloat64Interval::decode(void* buffer, size_t bufferSize)
   throw (InternalError,
          CouldNotDecode)
{
    _impl->decode(buffer, bufferSize);
}

size_t HLAfloat64Interval::encodedLength() const
{
    return 8;
}

HLAfloat64Interval::operator double () const
{
    return _impl->getTime();
}

}
