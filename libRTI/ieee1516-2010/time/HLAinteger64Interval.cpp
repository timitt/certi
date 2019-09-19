#include <RTI/time/HLAinteger64Interval.h>
#include "HLAinteger64IntervalImpl.h"
#include <RTI/time/HLAinteger64Time.h>

namespace rti1516e
{

HLAinteger64Interval::HLAinteger64Interval()
{
    _impl =  new HLAinteger64IntervalImpl();
}

HLAinteger64Interval::HLAinteger64Interval(Integer64 time)
{
    _impl = new HLAinteger64IntervalImpl(time);
}

HLAinteger64Interval::HLAinteger64Interval(rti1516e::LogicalTimeInterval const &other)
{
    _impl = new HLAinteger64IntervalImpl(dynamic_cast<const HLAinteger64IntervalImpl &> (other));
}

HLAinteger64Interval::HLAinteger64Interval(const HLAinteger64Interval& other)
{
    _impl = new HLAinteger64IntervalImpl(*other._impl);
}

HLAinteger64Interval::~HLAinteger64Interval () throw ()
{
    if (_impl != NULL)
        delete _impl;
    _impl = NULL;
}

void HLAinteger64Interval::setZero ()
{
    _impl->setInitial();
}

bool HLAinteger64Interval::isZero () const
{
    return _impl->isInitial();
}

void HLAinteger64Interval::setEpsilon ()
{
    _impl->setFinal();
}

bool HLAinteger64Interval::isEpsilon () const
{
    return _impl->isFinal();
}

rti1516e::LogicalTimeInterval& HLAinteger64Interval::operator= (rti1516e::LogicalTimeInterval const & value)
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other= dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            *_impl = *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return *this;
}

rti1516e::LogicalTimeInterval& HLAinteger64Interval::operator+=(rti1516e::LogicalTimeInterval const& value)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other= dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            *_impl += *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return *this;
}

rti1516e::LogicalTimeInterval& HLAinteger64Interval::operator-= (rti1516e::LogicalTimeInterval const & value)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other= dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            *_impl -= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return *this;
}

bool HLAinteger64Interval::operator> (rti1516e::LogicalTimeInterval const & value) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            return *_impl > *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return false;
}

bool HLAinteger64Interval::operator< (rti1516e::LogicalTimeInterval const & value) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            return *_impl < *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return false;
}

bool HLAinteger64Interval::operator== (rti1516e::LogicalTimeInterval const & value) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            return *_impl == *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return false;
}

bool HLAinteger64Interval::operator>= (rti1516e::LogicalTimeInterval const & value) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            return *_impl >= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return false;
}

bool HLAinteger64Interval::operator<= (rti1516e::LogicalTimeInterval const & value) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  dynamic_cast<const HLAinteger64Interval&>(value);
        if (other._impl != NULL)
            return *_impl <= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return false;
}

void HLAinteger64Interval::setToDifference (rti1516e::LogicalTime const & minuend, rti1516e::LogicalTime const& subtrahend)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& val0=  static_cast<const HLAinteger64Time&>(minuend);
        const HLAinteger64Time& val1=  static_cast<const HLAinteger64Time&>(subtrahend);
        if (val0._impl != NULL && val1._impl != NULL)
            this->setInterval(val0.getTime() - val1.getTime());
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
}


std::wstring HLAinteger64Interval::toString () const
{
    return _impl->toString();
}

std::wstring HLAinteger64Interval::implementationName() const
{
    const std::wstring implName(L"HLAinteger64Interval");
    return implName;
}

Integer64 HLAinteger64Interval::getInterval() const
{
    return _impl->getTime();
}

void HLAinteger64Interval::setInterval(Integer64 value)
{
    _impl->setTime(value);
}

HLAinteger64Interval& HLAinteger64Interval::operator=(const HLAinteger64Interval& other)
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    *_impl = *other._impl;
    return *this;
}

rti1516e::VariableLengthData HLAinteger64Interval::encode () const
{
    VariableLengthData vlData;
    std::vector<Octet> buffer(encodedLength());
    if(buffer.size() > 0) {
        encode(&buffer[0], buffer.size());
        vlData.setData(&buffer[0], buffer.size());
    }
    return vlData;
}

size_t HLAinteger64Interval::encode(void* buffer, size_t bufferSize) const
   throw (rti1516e::CouldNotEncode)
{
    return _impl->encode(buffer, bufferSize);
}

void HLAinteger64Interval::decode(rti1516e::VariableLengthData const & encodedValue)
   throw (rti1516e::InternalError,
          rti1516e::CouldNotDecode)
{
    std::vector<Octet> buffer(encodedValue.size());
    if(buffer.size() > 0) {
        std::memcpy(&buffer[0], encodedValue.data(), encodedValue.size());
        decode(&buffer[0], buffer.size());
    }
}

void HLAinteger64Interval::decode(void* buffer, size_t bufferSize)
   throw (rti1516e::InternalError,
          rti1516e::CouldNotDecode)
{
    _impl->decode(buffer, bufferSize);
}

size_t HLAinteger64Interval::encodedLength() const
{
    return 8;
}


}
