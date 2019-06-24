#include "BasicDataElementImplementation.h"

namespace rti1516e
{

#define DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(EncodableDataType, SimpleDataType)                             \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation() : _data() {}                                 \
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

}
