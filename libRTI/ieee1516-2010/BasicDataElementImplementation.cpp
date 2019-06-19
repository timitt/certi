#include <RTI/encoding/BasicDataElements.h>
#include "BasicDataElementImplementation.h"

namespace rti1516e
{
#define DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(EncodableDataType, SimpleDataType)                             \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation() : _value() {}                                \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation(const SimpleDataType& value)                   \
: _value(value) { }                                                                                                 \
                                                                                                                    \
EncodableDataType##Implementation::EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs)  \
: _value(rhs._value) { }                                                                                            \
                                                                                                                    \
EncodableDataType##Implementation::~EncodableDataType##Implementation() { }                                         \
                                                                                                                    \
EncodableDataType##Implementation& EncodableDataType##Implementation                                                \
::operator=(const EncodableDataType##Implementation& rhs)                                                           \
{                                                                                                                   \
    _value = rhs._value;                                                                                            \
    return *this;                                                                                                   \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType##Implementation::set(const SimpleDataType& inData)                                           \
{                                                                                                                   \
    _value = inData;                                                                                                \
}                                                                                                                   \
                                                                                                                    \
const SimpleDataType& EncodableDataType##Implementation::get() const                                                \
{                                                                                                                   \
    return _value;                                                                                                  \
}                                                                                                                   \
                                                                                                                    \
size_t EncodableDataType##Implementation::align(size_t offset, size_t alignment) const                              \
{                                                                                                                   \
    return (offset + alignment - 1) & ~(alignment - 1);                                                             \
}                                                                                                                   \
                                                                                                                    \
void EncodableDataType##Implementation::align(std::vector<Octet>& buffer, size_t alignment) const                   \
{                                                                                                                   \
    while (buffer.size() % alignment)                                                                               \
        buffer.push_back(0);                                                                                        \
}

DECLARE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAASCIIchar, char)

}
