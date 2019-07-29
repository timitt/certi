#include "HLAfloat64TimeImpl.h"
#include <thread>
#include <iostream>
#include <cstring>

namespace rti1516e
{

HLAfloat64TimeImpl::HLAfloat64TimeImpl() : _time(0), _zero(0)
{

}

HLAfloat64TimeImpl::HLAfloat64TimeImpl(const double &value): _time(value), _zero(0)
{

}

HLAfloat64TimeImpl::HLAfloat64TimeImpl(const HLAfloat64TimeImpl &other) : _time(other._time), _zero(other._zero),
    _epsilon(other._epsilon), _positiveInfinity(other._positiveInfinity)
{
}


void HLAfloat64TimeImpl::HLAfloat64TimeImpl::setInitial()
{
    _time = 0;
    _zero = 0;
}

bool HLAfloat64TimeImpl::isInitial() const
{
    return (_time == _zero);
}

void HLAfloat64TimeImpl::HLAfloat64TimeImpl::setFinal()
{
    _time = _positiveInfinity;
    _zero = 0;
}

bool HLAfloat64TimeImpl::isFinal() const
{
    return (_time == _positiveInfinity);
}

double HLAfloat64TimeImpl::getTime() const
{
    return _time;
}

void HLAfloat64TimeImpl::setTime(double value)
{
    _time = value;
}

std::__cxx11::wstring HLAfloat64TimeImpl::toString() const
{
    return std::to_wstring(_time);
}

HLAfloat64TimeImpl &HLAfloat64TimeImpl::operator=(const HLAfloat64TimeImpl &other)
throw (rti1516e::InvalidLogicalTime)
{
    _time = other._time;
    _zero = other._zero;
    return *this;
}

bool HLAfloat64TimeImpl::operator>(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time > other._time;
}

bool HLAfloat64TimeImpl::operator<(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time < other._time;
}

bool HLAfloat64TimeImpl::operator==(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time == other._time;
}

bool HLAfloat64TimeImpl::operator>=(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time >= other._time;
}

bool HLAfloat64TimeImpl::operator<=(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time <= other._time;
}

HLAfloat64TimeImpl::operator double() const
{
    return _time;
}

size_t HLAfloat64TimeImpl::encode(void *buffer, size_t bufferSize) const
throw (rti1516e::CouldNotEncode)
{
    if (bufferSize < 8)
        throw rti1516e::CouldNotEncode(L"Buffer is too small");
    rti1516e::HLAfloat64BE encoder(_time);
    std::vector<Octet> bytes;
    encoder.encodeInto(bytes);
    std::memcpy(buffer, &bytes[0], bytes.size());
    return encoder.getEncodedLength();
}

void HLAfloat64TimeImpl::decode(void *buffer, size_t bufferSize)
throw (rti1516e::InternalError,
       rti1516e::CouldNotDecode)
{
    rti1516e::HLAfloat64BE decoder;
    std::vector<Octet> bytes;
    bytes.resize(bufferSize);
    std::memcpy(&bytes[0], buffer, bufferSize);
    decoder.decodeFrom(bytes, 0);
    _time = decoder.get();
}

}
