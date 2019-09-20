#include "HLAinteger64.h"

namespace rti1516e
{

//\brief Default constructor of class HLAinteger64
HLAinteger64::HLAinteger64() : _time(0) , _zero(0)
{

}

//\brief Constructor of class HLAinteger64 with a specified time
//@param time The time value that need to be set
HLAinteger64::HLAinteger64(const Integer64& time) : _time(time), _zero(0)
{

}

//\brief Copy constructor of class HLAinteger64
//@param other The HLAinteger64 object which have to be copied
HLAinteger64::HLAinteger64(const HLAinteger64 &other) : _time(other._time), _zero(other._zero),
    _epsilon(other._epsilon), _positiveInfinity(other._positiveInfinity)
{

}

//\brief Set the time to the initial value
void HLAinteger64::setInitial()
{
    _time = _zero;
}

//\brief compare is the is the same as the initial value
//@return return True if time and initial time are equals, False if not
bool HLAinteger64::isInitial() const
{
    return _time == _zero;
}

//\brief Set the time to the final value
void HLAinteger64::setFinal()
{
    _time = _epsilon;
}

//\brief Compare is the is the same as the final value
//@return return True if time and final time are equals, False if not
bool HLAinteger64::isFinal() const
{
    return _time == _epsilon;
}

//\brief Get HLAinteger64 the time
//@return the time
Integer64 HLAinteger64::getTime() const
{
    return _time;
}

//\brief Set the HLAinteger64 time
//@param the time too be set
void HLAinteger64::setTime(Integer64 value)
{
    _time = value;
}

//\brief String representation of the HLAinteger64
//@return the time in string format
std::__cxx11::wstring HLAinteger64::toString() const
{
    return std::to_wstring(_time);
}

//\brief encode the time to a Bytes buffer
//@param buffer The buffer wich will contain the encoded value
//@param bufferSize The buffer size
//@return The number of Byte encoded
size_t HLAinteger64::encode(void *buffer, size_t bufferSize) const
throw (rti1516e::CouldNotEncode)
{
    if (bufferSize < 8)
        throw rti1516e::CouldNotEncode(L"Buffer is too small");
    rti1516e::HLAinteger64BE encoder(_time);
    std::vector<Octet> bytes;
    encoder.encodeInto(bytes);
    std::memcpy(buffer, &bytes[0], bytes.size());
    return encoder.getEncodedLength();
}

//\brief decode elements from a Byte buffer and set the time to the current object
//@param buffer The buffer wich will contain the buffer to decode
//@param bufferSize The start index to decode
void HLAinteger64::decode(void *buffer, size_t bufferSize)
throw (rti1516e::InternalError,
       rti1516e::CouldNotDecode)
{
    rti1516e::HLAinteger64BE decoder;
    std::vector<Octet> bytes;
    bytes.resize(bufferSize);
    std::memcpy(&bytes[0], buffer, bufferSize);
    decoder.decodeFrom(bytes, 0);
    _time = decoder.get();
}

}
