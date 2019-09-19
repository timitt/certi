#include <RTI/time/HLAinteger64Time.h>
#include "HLAinteger64TimeImpl.h"

namespace rti1516e
{


HLAinteger64Time::HLAinteger64Time()
{
    _impl = new HLAinteger64TimeImpl();
}

HLAinteger64Time::HLAinteger64Time(Integer64 value)
{
    _impl = new HLAinteger64TimeImpl(value);
}

HLAinteger64Time::HLAinteger64Time(rti1516e::LogicalTime const & value)
{
    HLAinteger64Time object = dynamic_cast<const HLAinteger64Time &> (value);
    _impl = new HLAinteger64TimeImpl(object.getTime());
}

HLAinteger64Time::HLAinteger64Time (HLAinteger64Time const & value)
{
    _impl = new HLAinteger64TimeImpl(*value._impl);
}

HLAinteger64Time::~HLAinteger64Time ()
   throw ()
{
    if (_impl != NULL)
        delete _impl;
    _impl = NULL;
}

void HLAinteger64Time::setInitial()
{
    _impl->setInitial();
}

bool HLAinteger64Time::isInitial() const
{
    return _impl->isInitial();
}

void HLAinteger64Time::setFinal()
{
    _impl->setFinal();
}

bool HLAinteger64Time::isFinal () const
{
    return _impl->isFinal();
}

rti1516e::LogicalTime& HLAinteger64Time::operator=(rti1516e::LogicalTime const & value)
   throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& other= dynamic_cast<const HLAinteger64Time&>(value);
        if (other._impl != NULL)
            *_impl = *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
    return *this;
}

rti1516e::LogicalTime& HLAinteger64Time::operator+=(rti1516e::LogicalTimeInterval const & addend)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  static_cast<const HLAinteger64Interval&>(addend);
        setTime(getTime() + other.getInterval());
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return *this;
}

rti1516e::LogicalTime& HLAinteger64Time::operator-= (rti1516e::LogicalTimeInterval const & subtrahend)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    try {
        const HLAinteger64Interval& other=  static_cast<const HLAinteger64Interval&>(subtrahend);
        setTime(getTime() - other.getInterval());
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Interval");
    }
    return *this;
}

bool HLAinteger64Time::operator>(rti1516e::LogicalTime const & value) const
   throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& other= dynamic_cast<const HLAinteger64Time&>(value);
        if (other._impl != NULL)
            return *_impl > *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
    return false;
}

bool HLAinteger64Time::operator<(rti1516e::LogicalTime const & value) const
   throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& other= dynamic_cast<const HLAinteger64Time&>(value);
        if (other._impl != NULL)
            return *_impl < *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
    return false;
}


bool HLAinteger64Time::operator==(rti1516e::LogicalTime const & value) const
   throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& other= dynamic_cast<const HLAinteger64Time&>(value);
        if (other._impl != NULL)
            return *_impl == *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
    return false;
}

bool HLAinteger64Time::operator>=(rti1516e::LogicalTime const & value) const
   throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& other= dynamic_cast<const HLAinteger64Time&>(value);
        if (other._impl != NULL)
            return *_impl >= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
    return false;
}

bool HLAinteger64Time::operator<=(rti1516e::LogicalTime const & value) const
   throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAinteger64Time& other= dynamic_cast<const HLAinteger64Time&>(value);
        if (other._impl != NULL)
            return *_impl <= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAinteger64Time");
    }
    return false;
}

rti1516e::VariableLengthData HLAinteger64Time::encode() const
{
    VariableLengthData vlData;
    std::vector<Octet> buffer(encodedLength());
    if(buffer.size() > 0) {
        encode(&buffer[0], buffer.size());
        vlData.setData(&buffer[0], buffer.size());
    }
    return vlData;
}

size_t HLAinteger64Time::encode(void* buffer, size_t bufferSize) const
   throw (rti1516e::CouldNotEncode)
{
    return _impl->encode(buffer, bufferSize);
}

size_t HLAinteger64Time::encodedLength () const
{
    return 8;
}

void HLAinteger64Time::decode(rti1516e::VariableLengthData const & VariableLengthData)
   throw (rti1516e::InternalError,
          rti1516e::CouldNotDecode)
{
    std::vector<Octet> buffer(VariableLengthData.size());
    if(buffer.size() > 0) {
        std::memcpy(&buffer[0], VariableLengthData.data(), VariableLengthData.size());
        decode(&buffer[0], buffer.size());
    }
}

void HLAinteger64Time::decode(void* buffer, size_t bufferSize)
   throw (rti1516e::InternalError,
          rti1516e::CouldNotDecode)
{
    _impl->decode(buffer, bufferSize);
}

std::wstring HLAinteger64Time::toString () const
{
    return _impl->toString();
}

std::wstring HLAinteger64Time::implementationName () const
{
    const std::wstring implName(L"HLAinteger64Time");
    return implName;
}


Integer64 HLAinteger64Time::getTime () const
{
    return _impl->getTime();
}

void HLAinteger64Time::setTime(Integer64 value)
{
    _impl->setTime(value);
}

HLAinteger64Time& HLAinteger64Time::operator=(const HLAinteger64Time& other)
   throw (rti1516e::InvalidLogicalTime)
{
    *_impl = *other._impl;
    return *this;
}

HLAinteger64Time::operator Integer64() const
{
    return _impl->operator Integer64();
}

}
