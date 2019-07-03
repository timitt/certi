#include "BasicDataElementImplementation.h"



namespace rti1516e
{

#define DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(EncodableDataType, SimpleDataType)                             \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation()                                              \
: _data() {}                                                                                                        \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation(const SimpleDataType& value)                   \
: _data(value) { }                                                                                                  \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs)  \
: _data(rhs.get()) { }                                                                                              \
                                                                                                                    \
EncodableDataType##Implementation::~EncodableDataType##Implementation() { }                                         \
                                                                                                                    \
EncodableDataType##Implementation& EncodableDataType##Implementation                                                \
::operator=(const EncodableDataType##Implementation& rhs)                                                           \
{                                                                                                                   \
    _data = rhs.get();                                                                                              \
    return *this;                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType##Implementation::set(const SimpleDataType& inData)                                           \
{                                                                                                                   \
    _data = inData;                                                                                                 \
}                                                                                                                   \
                                                                                                                    \
const SimpleDataType& EncodableDataType##Implementation::get() const                                                \
{                                                                                                                   \
    return _data;                                                                                                   \
}                                                                                                                   \
                                                                                                                    \

DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAASCIIchar, char)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAboolean, bool)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAbyte, Octet)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat32BE, float)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat32LE, float)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat64BE, double)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat64LE, double)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger16LE, Integer16)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger16BE, Integer16)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger32LE, Integer32)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger32BE, Integer32)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger64LE, Integer64)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger64BE, Integer64)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAoctetPairLE, OctetPair)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAoctetPairBE, OctetPair)
DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAunicodeChar, wchar_t)

}
