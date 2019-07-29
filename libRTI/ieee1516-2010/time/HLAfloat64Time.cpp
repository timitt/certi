#include <RTI/time/HLAfloat64Time.h>
#include <RTI/encoding/EncodingConfig.h>
#include <typeinfo>
#include <cstring>
#include "HLAfloat64TimeImpl.h"

namespace rti1516e
{

HLAfloat64Time::HLAfloat64Time()
{
    _impl = new HLAfloat64TimeImpl();
}

HLAfloat64Time::HLAfloat64Time(const double &value)
{
    _impl = new HLAfloat64TimeImpl(value);
}

HLAfloat64Time::HLAfloat64Time(const LogicalTime &value)
{
    _impl = new HLAfloat64TimeImpl(dynamic_cast<const HLAfloat64TimeImpl &> (value));
}

HLAfloat64Time::HLAfloat64Time(const HLAfloat64Time &value)
{
    _impl = new HLAfloat64TimeImpl(*value._impl);
}

HLAfloat64Time::~HLAfloat64Time() throw()
{
    if (_impl != NULL) {
        delete _impl;
        _impl = NULL;
    }
}

void HLAfloat64Time::setInitial()
{
    _impl->setInitial();
}

bool HLAfloat64Time::isInitial() const
{
    return _impl->isInitial();
}

void HLAfloat64Time::setFinal()
{
    return _impl->setFinal();
}

bool HLAfloat64Time::isFinal() const
{
    return _impl->isFinal();
}

LogicalTime &HLAfloat64Time::operator=(const LogicalTime &value)
throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& other= dynamic_cast<const HLAfloat64Time&>(value);
        if (other._impl != NULL)
            *_impl = *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Time");
    }
    return *this;
}

LogicalTime &HLAfloat64Time::operator+=(const LogicalTimeInterval &addend) //Todo: need HLAfloat64TimeInterval before
throw (rti1516e::IllegalTimeArithmetic,
       rti1516e::InvalidLogicalTimeInterval)
{

}

LogicalTime &HLAfloat64Time::operator-=(const LogicalTimeInterval &subtrahend) //Todo: need HLAfloat64TimeInterval before
throw (rti1516e::IllegalTimeArithmetic,
       rti1516e::InvalidLogicalTimeInterval)
{

}

bool HLAfloat64Time::operator>(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& other=  dynamic_cast<const HLAfloat64Time&>(value);
        if (other._impl != NULL)
            return *_impl > *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Time");
    }
    return false;
}

bool HLAfloat64Time::operator<(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& other=  dynamic_cast<const HLAfloat64Time&>(value);
        if (other._impl != NULL)
            return *_impl < *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Time");
    }
    return false;
}

bool HLAfloat64Time::operator==(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& other=  dynamic_cast<const HLAfloat64Time&>(value);
        if (other._impl != NULL)
            return *_impl == *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Time");
    }
    return false;
}

bool HLAfloat64Time::operator>=(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& other=  dynamic_cast<const HLAfloat64Time&>(value);
        if (other._impl != NULL)
            return *_impl >= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Time");
    }
    return false;
}

bool HLAfloat64Time::operator<=(const LogicalTime &value) const
throw (rti1516e::InvalidLogicalTime)
{
    try {
        const HLAfloat64Time& other=  dynamic_cast<const HLAfloat64Time&>(value);
        if (other._impl != NULL)
            return *_impl <= *other._impl;
    } catch(const std::bad_cast& e) {
        throw rti1516e::InvalidLogicalTime(L"Cannot convert LogicalTime to HLAfloat64Time");
    }
    return false;
}

VariableLengthData HLAfloat64Time::encode() const
{
    VariableLengthData vlData;
    std::vector<Octet> buffer(encodedLength());
    encode(&buffer[0], buffer.size());
    vlData.setData(&buffer[0], buffer.size());
    return vlData;
}

size_t HLAfloat64Time::encode(void *buffer, size_t bufferSize) const
throw (rti1516e::CouldNotEncode)
{
    return _impl->encode(buffer, bufferSize);
}

size_t HLAfloat64Time::encodedLength() const
{
    return 8;
}

void HLAfloat64Time::decode(const VariableLengthData &VariableLengthData)
throw (rti1516e::InternalError,
       rti1516e::CouldNotDecode)
{
    std::vector<Octet> buffer(VariableLengthData.size());
    std::memcpy(&buffer[0], VariableLengthData.data(), VariableLengthData.size());
    decode(&buffer[0], buffer.size());
}

void HLAfloat64Time::decode(void *buffer, size_t bufferSize)
throw (rti1516e::InternalError,
       rti1516e::CouldNotDecode)
{
    _impl->decode(buffer, bufferSize);
}

std::__cxx11::wstring HLAfloat64Time::toString() const
{
    return _impl->toString();
}

std::__cxx11::wstring HLAfloat64Time::implementationName() const
{
    const std::wstring implName(L"HLAfloat64Time");
    return implName;
}

double HLAfloat64Time::getTime() const
{
    return _impl->getTime();
}

void HLAfloat64Time::setTime(double value)
{
    _impl->setTime(value);
}

HLAfloat64Time &HLAfloat64Time::operator=(const HLAfloat64Time &other)
throw (rti1516e::InvalidLogicalTime)
{
    *_impl = *other._impl;
    return *this;
}

HLAfloat64Time::operator double() const
{
    return _impl->operator double();
}

}
