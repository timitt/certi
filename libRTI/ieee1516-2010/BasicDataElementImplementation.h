//#include <RTI/encoding/BasicDataElements.h>
#include <RTI/VariableLengthData.h>
#include <RTI/encoding/DataElement.h>
#include <cstring>

#define DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(EncodableDataType, SimpleDataType, encoder)                     \
                                                                                                                    \
    class RTI_EXPORT EncodableDataType##Implementation {                                                            \
    public:                                                                                                         \
        EncodableDataType##Implementation();                                                                        \
        EncodableDataType##Implementation(const SimpleDataType& value);                                             \
        EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs);                            \
        virtual ~EncodableDataType##Implementation();                                                               \
                                                                                                                    \
        EncodableDataType##Implementation& operator=(const EncodableDataType##Implementation& rhs);                 \
                                                                                                                    \
        encoder                                                                                                     \
                                                                                                                    \
        void set(const SimpleDataType& inData);                                                                     \
                                                                                                                    \
        const SimpleDataType& get() const;                                                                          \
                                                                                                                    \
        size_t align(size_t offset, size_t alignment) const;                                                        \
                                                                                                                    \
        void align(std::vector<Octet>& buffer, size_t alignment) const;                                             \
                                                                                                                    \
        SimpleDataType _value;                                                                                        \
    };                                                                                                              \
                                                                                                                    \


namespace rti1516e
{
    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAASCIIchar, char,
    size_t decodeFrom(std::vector<Octet> const & buffer, size_t index)
    {
      _value = buffer[index];
      return index + 1;
    }

    void encodeInto(std::vector<Octet>& buffer) const
    {
      buffer.push_back(_value);
    }

    size_t getEncodedLength() const
    {
      return 1;
    }

    unsigned int getOctetBoundary() const
    {
      return 1;
    }

    Integer64 hash() const
    {
      return Integer64(_value);
    }
    )
}
