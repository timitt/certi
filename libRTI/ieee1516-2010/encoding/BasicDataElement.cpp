#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>
#include <RTI/encoding/DataElement.h>
#include "BasicDataElementImplementation.h"
#include <cstring>


namespace rti1516e
{

#define DECLARE_ENCODING_HELPER_CLASS(EncodableDataType, SimpleDataType)                                            \
                                                                                                                    \
EncodableDataType::EncodableDataType() :                                                                            \
  _impl(new EncodableDataType##Implementation)                                                                      \
{                                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType::EncodableDataType(SimpleDataType const& inData) :                                                \
  _impl(new EncodableDataType##Implementation(inData))                                                              \
{                                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType::EncodableDataType(SimpleDataType* inData) :                                                      \
  _impl(new EncodableDataType##Implementation(*inData))                                                             \
{                                                                                                                   \
    delete inData;                                                                                                  \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType::EncodableDataType(EncodableDataType const& rhs) :                                                \
  _impl(new EncodableDataType##Implementation(*rhs._impl))                                                          \
{                                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType::~EncodableDataType()                                                                             \
{                                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType& EncodableDataType::operator=(EncodableDataType const& rhs)                                       \
{                                                                                                                   \
    *_impl = *rhs._impl;                                                                                            \
    return *this;                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
std::auto_ptr<DataElement> EncodableDataType::clone() const                                                         \
{                                                                                                                   \
    return std::auto_ptr<DataElement>(new EncodableDataType(*this));                                                \
}                                                                                                                   \
                                                                                                                    \
VariableLengthData EncodableDataType::encode() const                                                                \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    VariableLengthData variableLengthData;                                                                          \
    encode(variableLengthData);                                                                                     \
    return variableLengthData;                                                                                      \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType::encode(VariableLengthData& inData) const                                                    \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    std::vector<Octet> buffer;                                                                                      \
    encodeInto(buffer);\
    inData.setData(&buffer[0], buffer.size());                                                                      \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType::encodeInto(std::vector<Octet>& buffer) const                                                \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    return _impl->encodeInto(buffer);                                                                               \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType::decode(VariableLengthData const & inData)                                                   \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    std::vector<Octet> buffer(inData.size());                                                                       \
    std::memcpy(&buffer[0], inData.data(), inData.size());                                                          \
    decodeFrom(buffer, 0);                                                                                          \
}                                                                                                                   \
                                                                                                                    \
size_t EncodableDataType::decodeFrom(std::vector<Octet> const & buffer, size_t index)                               \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    if(index >= buffer.size())                                                                                      \
        throw EncoderException(L"Index superior than buffer size");                                                 \
    return _impl->decodeFrom(buffer, index);                                                                        \
}                                                                                                                   \
                                                                                                                    \
size_t EncodableDataType::getEncodedLength() const                                                                  \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    return _impl->getEncodedLength();                                                                               \
}                                                                                                                   \
                                                                                                                    \
unsigned int EncodableDataType::getOctetBoundary () const                                                           \
{                                                                                                                   \
    return _impl->getOctetBoundary();                                                                               \
}                                                                                                                   \
                                                                                                                    \
Integer64 EncodableDataType::hash() const                                                                           \
{                                                                                                                   \
    return _impl->hash();                                                                                           \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType::setDataPointer(SimpleDataType* inData)                                                      \
  throw (EncoderException)                                                                                          \
{                                                                                                                   \
    _impl->set(*inData);                                                                                             \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType::set(SimpleDataType inData)                                                                  \
{                                                                                                                   \
    _impl->set(inData);                                                                                             \
}                                                                                                                   \
                                                                                                                    \
SimpleDataType EncodableDataType::get() const                                                                       \
{                                                                                                                   \
    return _impl->get();                                                                                            \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType& EncodableDataType::operator=(SimpleDataType rhs)                                                 \
{                                                                                                                   \
    _impl->set(rhs);                                                                                                \
    return *this;                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
EncodableDataType::operator SimpleDataType() const                                                                  \
{                                                                                                                   \
    return _impl->get();                                                                                            \
}

DECLARE_ENCODING_HELPER_CLASS(HLAASCIIchar, char)
DECLARE_ENCODING_HELPER_CLASS(HLAboolean, bool)
DECLARE_ENCODING_HELPER_CLASS(HLAbyte, Octet)
DECLARE_ENCODING_HELPER_CLASS(HLAfloat32BE, float)
DECLARE_ENCODING_HELPER_CLASS(HLAfloat32LE, float)
DECLARE_ENCODING_HELPER_CLASS(HLAfloat64BE, double)
DECLARE_ENCODING_HELPER_CLASS(HLAfloat64LE, double)
DECLARE_ENCODING_HELPER_CLASS(HLAinteger16LE, Integer16)
DECLARE_ENCODING_HELPER_CLASS(HLAinteger16BE, Integer16)
DECLARE_ENCODING_HELPER_CLASS(HLAinteger32LE, Integer32)
DECLARE_ENCODING_HELPER_CLASS(HLAinteger32BE, Integer32)
DECLARE_ENCODING_HELPER_CLASS(HLAinteger64LE, Integer64)
DECLARE_ENCODING_HELPER_CLASS(HLAinteger64BE, Integer64)
DECLARE_ENCODING_HELPER_CLASS(HLAoctetPairLE, OctetPair)
DECLARE_ENCODING_HELPER_CLASS(HLAoctetPairBE, OctetPair)
DECLARE_ENCODING_HELPER_CLASS(HLAunicodeChar, wchar_t)
//DECLARE_ENCODING_HELPER_CLASS(HLA3Byte, Integer32)
}
